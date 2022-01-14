#include "sledge/hooks.h"
#include "sledge/ui.h"

#include "util/log.h"

#include <windef.h>
#include <wingdi.h>
#include <libloaderapi.h>
#include <processthreadsapi.h>

#include <detours.h>

#include <mutex>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "globals.h"

std::once_flag fUIInitialized;

typedef BOOL (*twglSwapBuffers)(_In_ HDC hDc);
twglSwapBuffers wglSwapBuffers;

double dLastUIUpdateTime;
void InitUI() {
	glfwInit();
	glewInit();
	dLastUIUpdateTime = glfwGetTime();
	CSledgeUI::CreateInstance();
}

BOOL hwglSwapBuffers(_In_ HDC hDc) {
	if (!g_WindowReady)
		return wglSwapBuffers(hDc);

	std::call_once(fUIInitialized, InitUI);

	double dDelta = glfwGetTime() - dLastUIUpdateTime;
	if (dDelta >= 1 / UI_REFRESH_RATE) {
		CSledgeUI::Instance()->Update();
		CSledgeUI::Instance()->Draw();
	}
	
	return wglSwapBuffers(hDc);
}

void Sledge::Hooks::SB() {
	HMODULE OpenGL = GetModuleHandle("C:\\Windows\\System32\\opengl32.dll");
	if (!OpenGL)
		return;

	wglSwapBuffers = (twglSwapBuffers)GetProcAddress(OpenGL, "wglSwapBuffers");
	LogVerbose("wglSwapBuffers: {0}", reinterpret_cast<void*>(wglSwapBuffers));

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)wglSwapBuffers, hwglSwapBuffers);
	DetourTransactionCommit();
}