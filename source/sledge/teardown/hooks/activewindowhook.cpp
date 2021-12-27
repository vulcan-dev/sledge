#include "teardown/hooks.h"
#include "teardown/signatures.h"

#include "util/memory.h"
#include "util/log.h"

#include <windef.h>
#include <processthreadsapi.h>
#include <detours.h>

bool hIsWindowActive(void*) {
	return true;
}

void Teardown::Hooks::ActiveWindow() {
	DWORD64 dwIsWindowActive = Memory::dwFindPattern(Signatures::IsWindowActive);

	LogVerbose("IsWindowActive: {}", reinterpret_cast<void*>(dwIsWindowActive));
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(LPVOID&)dwIsWindowActive, hIsWindowActive);
	DetourTransactionCommit();
}