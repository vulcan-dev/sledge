#include "teardown/hooks.h"
#include "teardown/signatures.h"

#include "util/memory.h"
#include "util/log.h"

#include <windef.h>
#include <processthreadsapi.h>
#include <detours.h>

#include "sledge/ui.h"

#include "globals.h"

typedef void (*tApplyDisplay) (void* pDevice, int iDisplayMode, int iDisplayResolution);
tApplyDisplay ApplyDisplay;

void hApplyDisplay(void* pDevice, int iDisplayMode, int iDisplayResolution) {
	g_WindowReady = false;
	CSledgeUI::Instance()->Reset();
	ApplyDisplay(pDevice, iDisplayMode, iDisplayResolution);
}

void Teardown::Hooks::Display() {
	ApplyDisplay = reinterpret_cast<tApplyDisplay>(Memory::dwFindPattern(Signatures::ApplyDisplay));

	LogVerbose("ApplyDisplay: {}", reinterpret_cast<void*>(ApplyDisplay));
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&ApplyDisplay, hApplyDisplay);
	DetourTransactionCommit();
}