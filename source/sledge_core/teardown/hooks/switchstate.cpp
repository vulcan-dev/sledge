#include "teardown/hooks.h"
#include "teardown/offsets.h"
#include "teardown/enums.h"

#include "net/sledgelib.h"

#include "util/log.h"

#include <minwindef.h>

#include <windef.h>
#include <processthreadsapi.h>
#include <detours.h>

#include "globals.h"

typedef void (*tSwitchState) (void* pGame, EGameState iState);
tSwitchState SwitchState;

void hSwitchState(void* pGame, EGameState State) {
	if (State == EGameState::Splash && g_SkipSplash)
		return SwitchState(pGame, EGameState::Menu);

	SwitchState(pGame, State);

	SledgeLib::CallbackInterface->StateChange(static_cast<unsigned int>(State));
}

void Teardown::Hooks::HookSwitchState() {
	SwitchState = reinterpret_cast<tSwitchState>(reinterpret_cast<DWORD64>(g_hTeardown) + g_Addresses["SwitchGameState"]);

	LogVerbose("SwitchState: {}", reinterpret_cast<void*>(SwitchState));
	
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&SwitchState, hSwitchState);
	DetourTransactionCommit();
}

void Teardown::Hooks::UnhookSwitchState() {
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&SwitchState, hSwitchState);
	DetourTransactionCommit();
}