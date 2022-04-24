#include "teardown/hooks.h"
#include "teardown/offsets.h"

#include "net/sledgelib.h"

#include "util/log.h"

#include <minwindef.h>

#include <windef.h>
#include <processthreadsapi.h>
#include <detours.h>

#include "globals.h"

typedef void (*tUpdatePlayer) (void* pPlayer, float fTimeStep);
tUpdatePlayer _UpdatePlayer;

void hUpdatePlayer(void* pPlayer, float fTimeStep) {
	SledgeLib::CallbackInterface->PrePlayerUpdate(fTimeStep);
	_UpdatePlayer(pPlayer, fTimeStep);
	SledgeLib::CallbackInterface->PostPlayerUpdate(fTimeStep);
}

void Teardown::Hooks::UpdatePlayer::Hook() {
	_UpdatePlayer = reinterpret_cast<tUpdatePlayer>(g_BaseAddress + g_Offsets["UpdatePlayer"]);

	LogVerbose("UpdatePlayer: {}", reinterpret_cast<void*>(_UpdatePlayer));

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&_UpdatePlayer, hUpdatePlayer);
	DetourTransactionCommit();
}

void Teardown::Hooks::UpdatePlayer::Unhook() {
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&_UpdatePlayer, hUpdatePlayer);
	DetourTransactionCommit();
}