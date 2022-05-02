#include "teardown/hooks.h"
#include "teardown/offsets.h"

#include "globals.h"

#include <windef.h>
#include <processthreadsapi.h>
#include <detours.h>

typedef void (*tUpdateToolBodies) (void*, float);
tUpdateToolBodies _UpdateToolBodies;

void hUpdateToolBodies(void*, float) {}

void Teardown::Hooks::UpdateTools::Hook() {
	_UpdateToolBodies = reinterpret_cast<tUpdateToolBodies>(g_BaseAddress + g_Offsets["UpdateToolBodies"]);

	LogVerbose("UpdateToolBodies: {}", reinterpret_cast<void*>(_UpdateToolBodies));

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&_UpdateToolBodies, hUpdateToolBodies);
	DetourTransactionCommit();
}

void Teardown::Hooks::UpdateTools::Unhook() {
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&_UpdateToolBodies, hUpdateToolBodies);
	DetourTransactionCommit();
}