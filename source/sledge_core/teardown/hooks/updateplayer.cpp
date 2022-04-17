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
tUpdatePlayer UpdatePlayer;

void hUpdatePlayer(void* pPlayer, float fTimeStep) {
	SledgeLib::CallbackInterface->PrePlayerUpdate(fTimeStep);
	UpdatePlayer(pPlayer, fTimeStep);
	SledgeLib::CallbackInterface->PostPlayerUpdate(fTimeStep);
}

void Teardown::Hooks::HookUpdatePlayer() {
	UpdatePlayer = reinterpret_cast<tUpdatePlayer>(g_BaseAddress + g_Offsets["UpdatePlayer"]);

	LogVerbose("UpdatePlayer: {}", reinterpret_cast<void*>(UpdatePlayer));

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&UpdatePlayer, hUpdatePlayer);
	DetourTransactionCommit();
}

void Teardown::Hooks::UnhookUpdatePlayer() {
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&UpdatePlayer, hUpdatePlayer);
	DetourTransactionCommit();
}