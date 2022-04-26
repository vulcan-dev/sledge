#include "globals.h"

#include "teardown/hooks.h"
#include "teardown/offsets.h"

#include <windef.h>
#include <processthreadsapi.h>
#include <detours.h>

typedef bool (*tIsWindowForegroundWindow) (void*);
tIsWindowForegroundWindow _IsWindowForegroundWindow;

bool hIsWindowForegroundWindow(void*) { return true; }

void Teardown::Hooks::IsWindowForegroundWindow::Hook() {
	_IsWindowForegroundWindow = reinterpret_cast<tIsWindowForegroundWindow>(g_BaseAddress + g_Offsets["IsWindowForegroundWindow"]);

	LogVerbose("IsWindowForegroundWindow: {}", reinterpret_cast<void*>(_IsWindowForegroundWindow));

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&_IsWindowForegroundWindow, hIsWindowForegroundWindow);
	DetourTransactionCommit();
}

void Teardown::Hooks::IsWindowForegroundWindow::Unhook() {
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&_IsWindowForegroundWindow, hIsWindowForegroundWindow);
	DetourTransactionCommit();
}