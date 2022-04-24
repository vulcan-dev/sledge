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
tUpdate _Update;

void hUpdate(void* pGame, void* pDevice) {
	SledgeLib::CallbackInterface->PreUpdate();
	_Update(pGame, pDevice);
	SledgeLib::CallbackInterface->PostUpdate();
}

void Teardown::Hooks::Update::Hook() {
	_Update = reinterpret_cast<tUpdate>(g_BaseAddress + g_Offsets["Update"]);

	LogVerbose("Update: {}", reinterpret_cast<void*>(_Update));

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&_Update, hUpdate);
	DetourTransactionCommit();
}

void Teardown::Hooks::Update::Unhook() {
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&_Update, hUpdate);
	DetourTransactionCommit();
}