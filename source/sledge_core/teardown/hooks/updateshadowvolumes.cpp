#include "teardown/hooks.h"
#include "teardown/offsets.h"

#include <windef.h>
#include <processthreadsapi.h>
#include <detours.h>

#include "globals.h"

typedef void (*tUpdateShadowVolumes) (class Scene* pScene);
tUpdateShadowVolumes _UpdateShadowVolumes;

bool bSwitch = true;
void hUpdateShadowVolumes(class Scene* pScene) {
	if (bSwitch)
		_UpdateShadowVolumes(pScene);
	bSwitch = !bSwitch;
}

void Teardown::Hooks::UpdateShadowVolumes::Hook() {
	_UpdateShadowVolumes = reinterpret_cast<tUpdateShadowVolumes>(g_BaseAddress + g_Offsets["UpdateShadowVolumes"]);

	LogVerbose("UpdateShadowVolumes: {}", reinterpret_cast<void*>(_UpdateShadowVolumes));

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&_UpdateShadowVolumes, hUpdateShadowVolumes);
	DetourTransactionCommit();
}

void Teardown::Hooks::UpdateShadowVolumes::Unhook() {
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&_UpdateShadowVolumes, hUpdateShadowVolumes);
	DetourTransactionCommit();
}