#include "sledge/ui.h"
#include "util/log.h"
#include "globals.h"

#include <AppCore/Platform.h>

#include <string>

CSledgeUI* CSledgeUI::m_Instance = 0;

/*
	to-do:
		implement custom filesystem for ultralight
		implement destructor
		make platform things like logger and driver be outside of this, to allow for multiple instances of CSledgeUI
*/
CSledgeUI::CSledgeUI() {
	m_Driver = new CGPUDriver();

	ultralight::Config Config;
	Config.animation_timer_delay = 1 / REFRESH_RATE;
	Config.scroll_timer_delay = 1 / REFRESH_RATE;

	// force repaint on debug builds to facilitate troubleshooting on renderdoc
	#ifdef _DEBUG
		Config.force_repaint = true;
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

CSledgeUI::~CSledgeUI() {
	//delete m_Driver;
	//m_Renderer->PurgeMemory();
	//m_Renderer->Release();
}

void CSledgeUI::Update() {
	WebContainers::CallFutures();

	m_Renderer->Update();
	m_Renderer->Render();

	m_Driver->DrawCommandList();
}

void CSledgeUI::Draw() {
	for (CWebContainer* Container : WebContainers::RegisteredContainers) { Container->Draw(); }
}

void CSledgeUI::LogMessage(ultralight::LogLevel, const ultralight::String16& Message) {
	ultralight::String String(Message);
	LogVerbose("Ultralight log: {}", String.utf8().data());
}