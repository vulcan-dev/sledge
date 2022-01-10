#include "sledge/hooks.h"
#include "sledge/ui.h"

#include "util/log.h"

#include <GL/glew.h>

#include <windef.h>
#include <wingdi.h>
#include <libloaderapi.h>
#include <processthreadsapi.h>

#include <detours.h>

#include <mutex>

std::once_flag fUIInitialized;

HGLRC TDContext; // Context where the game draws everything
HGLRC ULContext; // Context where ultralight's drawcalls occur
HGLRC UIContext; // Context where the UI is drawn

typedef BOOL (*twglSwapBuffers)(_In_ HDC hDc);
twglSwapBuffers wglSwapBuffers;

CSledgeUI* UI;

void InitUI(HDC hDc) {
	TDContext = wglGetCurrentContext();
	ULContext = wglCreateContext(hDc);
	UIContext = wglCreateContext(hDc);

	wglShareLists(ULContext, UIContext); // share textures from ultralight context to the ui context

	UI = new CSledgeUI();
}

BOOL hwglSwapBuffers(_In_ HDC hDc) {
	std::call_once(fUIInitialized, InitUI, hDc);

	wglMakeCurrent(hDc, ULContext);
	UI->Update();

	wglMakeCurrent(hDc, UIContext);
	UI->Draw();

	wglMakeCurrent(hDc, TDContext);

	return wglSwapBuffers(hDc);
}

void Sledge::Hooks::SB() {
	HMODULE OpenGL = GetModuleHandle("C:\\Windows\\System32\\opengl32.dll");
	if (!OpenGL)
		return;

	glewInit();

	wglSwapBuffers = (twglSwapBuffers)GetProcAddress(OpenGL, "wglSwapBuffers");
	LogVerbose("wglSwapBuffers: {0}", reinterpret_cast<void*>(wglSwapBuffers));

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)wglSwapBuffers, hwglSwapBuffers);
	DetourTransactionCommit();
}