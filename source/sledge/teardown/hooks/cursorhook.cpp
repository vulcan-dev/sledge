#include "teardown/hooks.h"
#include "teardown/signatures.h"

#include "globals.h"

#include "util/memory.h"
#include "util/log.h"

#include <windef.h>
#include <WinUser.h>
#include <processthreadsapi.h>
#include <detours.h>

typedef void (*tSetCursorCaptured) (void* pWindow, bool bCaptured);
tSetCursorCaptured SetCursorCaptured;

void hSetCursorCaptured(void* pWindow, bool bCaptured) {
	if (bCaptured && GetForegroundWindow() == reinterpret_cast<HWND>(g_hWnd)) {
		SetCursorCaptured(pWindow, bCaptured);
		return;
	}
	SetCursorCaptured(pWindow, false);
}

void Teardown::Hooks::Cursor() {
	SetCursorCaptured = reinterpret_cast<tSetCursorCaptured>(Memory::dwFindPattern(Signatures::SetCursorCaptured));

	LogVerbose("SetCursorCaptured: {}", reinterpret_cast<void*>(SetCursorCaptured));
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&SetCursorCaptured, hSetCursorCaptured);
	DetourTransactionCommit();
}