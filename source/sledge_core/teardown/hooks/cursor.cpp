#include "teardown/hooks.h"
#include "teardown/offsets.h"

#include "globals.h"

#include <windef.h>
#include <WinUser.h>
#include <processthreadsapi.h>
#include <detours.h>

typedef void (*tSetCursorCaptured) (void* pWindow, bool bCaptured);
tSetCursorCaptured _SetCursorCaptured;

void hSetCursorCaptured(void* pWindow, bool bCaptured) {
	if (bCaptured && GetForegroundWindow() == reinterpret_cast<HWND>(g_hWnd))
		return _SetCursorCaptured(pWindow, bCaptured);

	_SetCursorCaptured(pWindow, false);
}

void Teardown::Hooks::SetCursorCaptured::Hook() {
	_SetCursorCaptured = reinterpret_cast<tSetCursorCaptured>(g_BaseAddress + g_Offsets["SetCursorCaptured"]);

	LogVerbose("SetCursorCaptured: {}", reinterpret_cast<void*>(_SetCursorCaptured));

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&_SetCursorCaptured, hSetCursorCaptured);
	DetourTransactionCommit();
}

void Teardown::Hooks::SetCursorCaptured::Unhook() {
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&_SetCursorCaptured, hSetCursorCaptured);
	DetourTransactionCommit();
}