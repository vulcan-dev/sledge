#include "sledge/loader.h"
#include "sledge/hooks.h"
#include "sledge/misc/nethost.h"
#include "sledge/misc/steam.h"

#include "teardown/functions.h"
#include "teardown/hooks.h"

#include "util/log.h"
#include "sledgelib.h"

#include <string>

#include <libloaderapi.h>
#include <consoleapi.h>
#include <processenv.h>

#include "globals.h"

// from: WinBase.h
#define STD_OUTPUT_HANDLE ((DWORD)-11)

bool bLateInitCalled = false;
bool bLateLateInitAlreadyCalled = false;

/*
	Init:
		Called the moment the DLL is injected
*/
void Loader::Init(void* hModule) {
	AllocConsole();

	freopen("CONOUT$", "w", stdout);

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	// enable escape sequences
	DWORD dwMode = 0;
	GetConsoleMode(hConsole, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hConsole, dwMode);

	LogInfo("Slegde Mod Loader - {} build", g_Build);

	g_Module = GetModuleHandle(NULL);

	char cModuleName[MAX_PATH];
	GetModuleFileNameA(reinterpret_cast<HMODULE>(hModule), cModuleName, MAX_PATH);
	
	std::string sModuleName(cModuleName);
	std::string sModulePath = sModuleName.substr(0, sModuleName.find_last_of('\\'));
	memcpy(g_ModulePath, sModulePath.c_str(), sModulePath.length());
	g_ModulePath[sModulePath.length()] = '\0';
	
	if (!Steam::Init()) {
		LogError("failed to initialize steam");
		return;
	}

	bLateInitCalled = true;
	LogVerbose("getting func addresses");
	Teardown::GetFunctionAddresses();
	LogVerbose("hooking game cctor");
	Teardown::Hooks::Game();
	LogVerbose("hooking callback functions");
	Teardown::Hooks::CallbackHooks();
	LogVerbose("hooking log function");
	Teardown::Hooks::Log();
}

/*
	LateInit:
		At this point CGame has been created, and all the classes within it as well (CScene, CEditor, CPlayer, etc).
		(primarily used for loading libraries / mods)
*/
void Loader::LateInit() {
	LogVerbose("Hooking wndproc");
	Sledge::Hooks::Wnd();

	if (bLateLateInitAlreadyCalled)
		return;
	bLateLateInitAlreadyCalled = true;

	LogVerbose("Hooking swapbuffers");
	Sledge::Hooks::SB();

	LogVerbose("hooking active window check");
	Teardown::Hooks::ActiveWindow();

	LogVerbose("hooking cursor capture function");
	Teardown::Hooks::Cursor();

	LogVerbose("setting up hostfxr");
	if (!NetHost::Init()) {
		LogError("failed to init hostfxr");
		return;
	}

	LogVerbose("loading sledgelib");
	if (!SledgeLib::Load()) {
		LogError("failed to load sledgelib");
		return;
	}
}