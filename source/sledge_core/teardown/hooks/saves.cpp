#include "teardown/hooks.h"
#include "teardown/offsets.h"

#include "net/sledgelib.h"

#include <windef.h>
#include <processthreadsapi.h>
#include <detours.h>

typedef void (*tQuickSaveFunc) (void* pScene);
tQuickSaveFunc _LoadQuickSave;
tQuickSaveFunc _WriteQuickSave;

void hLoadQuickSave(void* pScene) {
	_LoadQuickSave(pScene);
	SledgeLib::CallbackInterface->QuickLoad();
}

void hWriteQuickSave(void* pScene) {
	_WriteQuickSave(pScene);
	SledgeLib::CallbackInterface->QuickSave();
}

void Teardown::Hooks::Saves::Hook() {
	_LoadQuickSave = reinterpret_cast<tQuickSaveFunc>(g_BaseAddress + g_Offsets["LoadQuickSave"]);
	_WriteQuickSave = reinterpret_cast<tQuickSaveFunc>(g_BaseAddress + g_Offsets["WriteQuickSave"]);

	LogVerbose("LoadQuickSave: {}", reinterpret_cast<void*>(_LoadQuickSave));
	LogVerbose("WriteQuickSave: {}", reinterpret_cast<void*>(_WriteQuickSave));

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&_LoadQuickSave, hLoadQuickSave);
	DetourAttach(&_WriteQuickSave, hWriteQuickSave);
	DetourTransactionCommit();
}

void Teardown::Hooks::Saves::Unhook() {
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&_LoadQuickSave, hLoadQuickSave);
	DetourDetach(&_WriteQuickSave, hWriteQuickSave);
	DetourTransactionCommit();
}