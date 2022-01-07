#include "sledge/hooks.h"
#include "util/log.h"

#include <windef.h>
#include <libloaderapi.h>
#include <processthreadsapi.h>
#include <detours.h>

typedef BOOL(*twglSwapBuffers)(_In_ HDC hDc);
twglSwapBuffers wglSwapBuffers;

void hwglSwapBuffers(_In_ HDC hDc) {
	wglSwapBuffers(hDc);
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