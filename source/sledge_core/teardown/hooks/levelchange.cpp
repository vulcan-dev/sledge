#include "teardown/hooks.h"
#include "teardown/offsets.h"
#include "teardown/types/small_string.h"
#include "teardown/classes/game.h"

#include "net/sledgelib.h"

#include "util/log.h"

#include <windef.h>
#include <processthreadsapi.h>
#include <detours.h>

#include "globals.h"

typedef void (*tChangeLevel) (Game* pGame, small_string* ssLevelId, bool bPassThrough);
tChangeLevel ChangeLevel;

void hChangeLevel(Game* pGame, small_string* ssLevelId, bool bPassThrough) {
	ChangeLevel(pGame, ssLevelId, bPassThrough);
	SledgeLib::CallbackInterface->LevelLoad(pGame->m_LevelPath.c_str());
}

void Teardown::Hooks::LevelChange::Hook() {
	ChangeLevel = reinterpret_cast<tChangeLevel>(g_BaseAddress + g_Offsets["ChangeLevel"]);

	LogVerbose("ChangeLevel: {}", reinterpret_cast<void*>(ChangeLevel));

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&ChangeLevel, hChangeLevel);
	DetourTransactionCommit();
}

void Teardown::Hooks::LevelChange::Unhook() {
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&ChangeLevel, hChangeLevel);
	DetourTransactionCommit();
}