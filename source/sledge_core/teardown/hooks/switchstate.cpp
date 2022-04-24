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
tSwitchState _SwitchState;

void hSwitchState(void* pGame, EGameState State) {
	if (State == EGameState::Splash && g_SkipSplash)
		return _SwitchState(pGame, EGameState::Menu);
	_SwitchState(pGame, State);

	SledgeLib::CallbackInterface->StateChange(static_cast<unsigned int>(State));
}

void Teardown::Hooks::SwitchState::Hook() {
	_SwitchState = reinterpret_cast<tSwitchState>(g_BaseAddress + g_Offsets["SwitchGameState"]);

	LogVerbose("SwitchState: {}", reinterpret_cast<void*>(_SwitchState));
	
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&_SwitchState, hSwitchState);
	DetourTransactionCommit();
}

void Teardown::Hooks::SwitchState::Unhook() {
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&_SwitchState, hSwitchState);
	DetourTransactionCommit();
}