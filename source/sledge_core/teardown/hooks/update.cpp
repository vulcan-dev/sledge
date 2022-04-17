#include "teardown/hooks.h"
#include "teardown/offsets.h"

#include "net/sledgelib.h"

#include "util/log.h"

#include <minwindef.h>

#include <windef.h>
#include <processthreadsapi.h>
#include <detours.h>

#include "globals.h"

typedef void (*tUpdate) (void* pGame, void* pDevice);
tUpdate Update;

void hUpdate(void* pGame, void* pDevice) {
	SledgeLib::CallbackInterface->PreUpdate();
	Update(pGame, pDevice);
	SledgeLib::CallbackInterface->PostUpdate();
}

void Teardown::Hooks::HookUpdate() {
	Update = reinterpret_cast<tUpdate>(g_BaseAddress + g_Offsets["Update"]);

	LogVerbose("Update: {}", reinterpret_cast<void*>(Update));

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&Update, hUpdate);
	DetourTransactionCommit();
}

void Teardown::Hooks::UnhookUpdate() {
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&Update, hUpdate);
	DetourTransactionCommit();
}