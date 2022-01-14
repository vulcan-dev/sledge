#include "sledge/ui.h"
#include "util/log.h"
#include "globals.h"

#include <AppCore/Platform.h>
#include <string>

CSledgeUI* CSledgeUI::m_Instance = nullptr;

CSledgeUI::CSledgeUI() {
	m_Driver = new CGPUDriver();

	ultralight::Config Config;
	Config.animation_timer_delay = 1 / UI_REFRESH_RATE;
	Config.scroll_timer_delay = 1 / UI_REFRESH_RATE;
	#ifdef _DEBUG
		Config.force_repaint = true; // repaint on debug for easier troubleshooting
	#else
		Config.force_repaint = false;
	#endif

	std::string sFSPath(g_ModulePath);
	sFSPath.append("\\ui");

	ultralight::Platform& Platform = ultralight::Platform::instance();
	Platform.set_gpu_driver(m_Driver);
	Platform.set_config(Config);
	Platform.set_file_system(ultralight::GetPlatformFileSystem(sFSPath.c_str()));
	Platform.set_font_loader(ultralight::GetPlatformFontLoader());
	Platform.set_logger(this);

	m_Renderer = ultralight::Renderer::Create();
}

CSledgeUI::~CSledgeUI() {};

GLint iReadFBO, iPrevVertexArray;

void CSledgeUI::Update() {
	glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &iReadFBO);
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &iPrevVertexArray);

	WebContainers::InvokeFutures();

	m_Renderer->Update();
	m_Renderer->Render();

	m_Driver->DrawCommandList();
}

void CSledgeUI::Reset() {
	m_Renderer->PurgeMemory();
	m_Driver->Clear();

	std::thread ContainerWaiterThread(WebContainers::BeginReset);
	ContainerWaiterThread.detach();

	std::thread WindowWaiterThread([] {
		while (!g_WindowReady) { std::this_thread::sleep_for(std::chrono::milliseconds(50)); }

		std::shared_future Reset = std::async(std::launch::deferred, [] {
				WebContainers::EndReset();

				WebContainers::RegisteredMutex.lock();
				for (auto Pair = WebContainers::RegisteredContainers.begin(); Pair != WebContainers::RegisteredContainers.end(); Pair++) { Pair->second->Reload(); }
				WebContainers::RegisteredMutex.unlock();

			});
		WebContainers::AddFuture(&Reset);
		WebContainers::WaitForFutures();
	});
	WindowWaiterThread.detach();

}

void CSledgeUI::Draw() {
	WebContainers::Draw();
	glBindFramebuffer(GL_READ_FRAMEBUFFER, iReadFBO);
	glBindVertexArray(iPrevVertexArray);
}

void CSledgeUI::LogMessage(ultralight::LogLevel, const ultralight::String16& Message) {
	ultralight::String String(Message);
	LogVerbose("Ultralight log: {}", String.utf8().data());
}