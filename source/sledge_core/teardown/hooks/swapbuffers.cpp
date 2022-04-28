#include "teardown/hooks.h"
#include "teardown/offsets.h"

#include <windef.h>
#include <libloaderapi.h>
#include <processthreadsapi.h>
#include <detours.h>

#include <mutex>

#include <GL/glew.h>

#include "globals.h"

typedef bool (*twglSwapBuffers)(_In_ HDC hDc);
twglSwapBuffers _wglSwapBuffers;

std::once_flag fInitialized;

void Init() {
	glewInit();
}

bool hwglSwapBuffers(_In_ HDC hDc) {
	std::call_once(fInitialized, Init);

	return _wglSwapBuffers(hDc);
}

void Teardown::Hooks::SwapBuffers::Hook() {
	HMODULE OpenGL = GetModuleHandle("C:\\Windows\\System32\\opengl32.dll");
	if (!OpenGL)
		return;

	_wglSwapBuffers = reinterpret_cast<twglSwapBuffers>(GetProcAddress(OpenGL, "wglSwapBuffers"));
	LogVerbose("wglSwapBuffers: {0}", reinterpret_cast<void*>(_wglSwapBuffers));

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&_wglSwapBuffers, hwglSwapBuffers);
	DetourTransactionCommit();
}

void Teardown::Hooks::SwapBuffers::Unhook() {
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&_wglSwapBuffers, hwglSwapBuffers);
	DetourTransactionCommit();
}