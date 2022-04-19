	#include "teardown/hooks.h"
#include "teardown/offsets.h"
#include "teardown/classes/scriptcore.h"

#include "sledge/luahelpers.h"
#include "sledge/loader.h"

#include "util/log.h"

#include <minwindef.h>

#include <windef.h>
#include <processthreadsapi.h>
#include <detours.h>

#include "globals.h"

typedef void (*tRegisterLuaFunctions) (ScriptCore* pSC);
tRegisterLuaFunctions RegisterLuaFunctions;


void hRegisterLuaFunctions(ScriptCore* pSC) {
	RegisterLuaFunctions(pSC);
	Sledge::LuaHelpers::RegisterLuaFunctions(pSC);
}

void Teardown::Hooks::HookRegisterLuaFunctions() {
	RegisterLuaFunctions = reinterpret_cast<tRegisterLuaFunctions>(g_BaseAddress + g_Offsets["ScriptCore::RegisterLuaFunctions"]);

	LogVerbose("GameCCtor: {}", reinterpret_cast<void*>(RegisterLuaFunctions));

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&RegisterLuaFunctions, hRegisterLuaFunctions);
	DetourTransactionCommit();
}

void Teardown::Hooks::UnhookRegisterLuaFunctions() {
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&RegisterLuaFunctions, hRegisterLuaFunctions);
	DetourTransactionCommit();
}