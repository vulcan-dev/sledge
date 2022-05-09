#include "teardown/hooks.h"
#include "teardown/offsets.h"
#include "teardown/classes/script.h"

#include "sledge/luahelpers.h"
#include "sledge/loader.h"

#include "util/log.h"

#include <windef.h>
#include <processthreadsapi.h>
#include <detours.h>

#include "globals.h"

typedef void (*tRegisterLuaFunctions) (ScriptCore* pSC);
tRegisterLuaFunctions _RegisterLuaFunctions;

void hRegisterLuaFunctions(ScriptCore* pSC) {
	_RegisterLuaFunctions(pSC);
	Sledge::LuaHelpers::RegisterLuaFunctions(pSC);
}

void Teardown::Hooks::RegisterLuaFunctions::Hook() {
	_RegisterLuaFunctions = reinterpret_cast<tRegisterLuaFunctions>(g_BaseAddress + g_Offsets["ScriptCore::RegisterLuaFunctions"]);

	LogVerbose("GameCCtor: {}", reinterpret_cast<void*>(_RegisterLuaFunctions));

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&_RegisterLuaFunctions, hRegisterLuaFunctions);
	DetourTransactionCommit();
}

void Teardown::Hooks::RegisterLuaFunctions::Unhook() {
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&_RegisterLuaFunctions, hRegisterLuaFunctions);
	DetourTransactionCommit();
}