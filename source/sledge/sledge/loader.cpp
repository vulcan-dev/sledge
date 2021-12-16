#include "sledge/loader.h"
#include "sledge/hooks.h"

#include "teardown/functions.h"
#include "teardown/hooks.h"

#include "util/log.h"
#include "misc/nethost.h"
#include "sledgelib.h"

#include <string>

#include <libloaderapi.h>
#include <consoleapi.h>
#include <processenv.h>

#include "globals.h"

// from: WinBase.h
#define STD_OUTPUT_HANDLE ((DWORD)-11)

void Loader::Init(void* hModule) {
	AllocConsole();

	// it's either we ignore the return of freopen, or we define a FILE* and get an error for not using it lol
	freopen("CONOUT$", "w", stdout);

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	// enable escape sequences
	DWORD dwMode = 0;
	GetConsoleMode(hConsole, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hConsole, dwMode);

	LogInfo("slegde mod loader - {} build", g_Build);

	g_Module = GetModuleHandle(NULL);

	char cModuleName[MAX_PATH];
	GetModuleFileNameA(reinterpret_cast<HMODULE>(hModule), cModuleName, MAX_PATH);

	std::string sModuleName(cModuleName);
	std::string sModulePath = sModuleName.substr(0, sModuleName.find_last_of('\\'));
	memcpy(g_ModulePath, sModulePath.c_str(), sModulePath.length());
	g_ModulePath[sModulePath.length()] = '\0';

	LogInfo("setting up hostfxr");
	if (!NetHost::Init()) {
		LogError("failed to init hostfxr");
		return;
	}

	LogInfo("loading sledgelib");
	if (!SledgeLib::Load()) {
		LogError("failed to load sledgelib");
		return;
	}

	LogInfo("hooking cw");
	if (!Sledge::Hooks::CW()) {
		LogError("failed to hook cw");
		return;
	}
}

/*
	LateInit:
		At this point, SteamStub is done unpacking, everything is loaded in memory, but nothing is initialized yet.
		(useful for hooking, finding sigs, etc)
*/
void Loader::LateInit() {
	Teardown::GetFunctionAddresses();
	Teardown::Hooks::Game();
}

/*
	LateLateInit:
		At this point CGame has been created, and all the classes within it as well (CScene, CEditor, CPlayer, etc).
		(useful for loading libraries / mods)
*/
void Loader::LateLateInit() {

}