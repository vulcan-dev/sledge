#include "loader.h"
#include "globals.h"

#include "sledge/vr.h"

#include "net/sledgelib.h"
#include "net/nethost.h"

#include "teardown/hooks.h"
#include "teardown/functions.h"

#include <processenv.h>

#include <memoryapi.h>
#include <consoleapi.h>
#include <libloaderapi.h>

// from: WinBase.h
#define STD_OUTPUT_HANDLE ((DWORD)-11)

// from: WinUser.h
#define MB_ICONERROR 0x00000010L
#define MB_OK 0x00000000L


void ReportErrorAndUnload(const char* cError) {
	/*
		write the final error to the log
	*/
	LogError("Fatal error ocurred during Loader::Init - {}", cError);

	/*
		close the console
	*/
	#ifndef  _DEBUG
		fclose(stdout);
		FreeConsole();
	#endif

	Loader::Shutdown();

	/*
		display message box
	*/
	typedef int (*tMessageBoxA)(void* hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType);

	HMODULE hUser32 = LoadLibraryA("user32.dll");
	if (!hUser32)
		return;

	tMessageBoxA MessageBoxA = reinterpret_cast<tMessageBoxA>(GetProcAddress(hUser32, "MessageBoxA"));
	if (!MessageBoxA)
		return;
	MessageBoxA(0, cError, "Sledge failed to load", MB_ICONERROR | MB_OK);

	/*
		unload dll
	*/
	Loader::Shutdown();
	FreeLibrary(reinterpret_cast<HMODULE>(g_hSledge));
}

/*
	called when the DLL is attached
*/
void Loader::Init(void* hModule) {
	/*
		store module handles for sledge and teardown
		get teardown's base address for handling memory later
	*/
	g_hSledge = hModule;
	g_hTeardown = GetModuleHandle(NULL);
	g_BaseAddress = reinterpret_cast<DWORD64>(g_hTeardown);

	/*
		get and store module path
	*/
	char cModuleName[MAX_PATH];
	GetModuleFileNameA(reinterpret_cast<HMODULE>(hModule), cModuleName, MAX_PATH);
	std::string sModuleName(cModuleName);
	std::string sModulePath = sModuleName.substr(0, sModuleName.find_last_of('\\'));

	memcpy(g_ModulePath, sModulePath.c_str(), sModulePath.length());
	g_ModulePath[sModulePath.length()] = '\0';

	/*
		initialize logger
	*/
	std::string sLogPath = sModulePath + "\\..\\logs\\";
	LogInit("sledge", sLogPath.c_str());

	/*
		create console and send CONOUT$
	*/
	AllocConsole();
	if (!freopen("CONOUT$", "w", stdout)) {
		ReportErrorAndUnload("Error ocurred while calling freopen.");
		return;
	}

	/*
		enable ANSI escape sequences
	*/
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	GetConsoleMode(hConsole, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hConsole, dwMode);

	LogInfo("Slegde Mod Loader - {} build", g_Build);

	/*
		parse command line arguments
	*/
	char* cCMDLine = GetCommandLineA();
	if (strstr(cCMDLine, "-nosplash"))
		g_SkipSplash = true;

	if (strstr(cCMDLine, "-vr"))
		g_VR = true;

	/*
		initialize everything
	*/
	Teardown::GetFunctionAddresses();

	if (!NetHost::Init())
		ReportErrorAndUnload("HostFxr failed to load");

	Teardown::ApplyHooks();

	if (g_VR)
		if (!SledgeVR::Init())
			ReportErrorAndUnload("OpenVR failed to load");

	if (!SledgeLib::Load())
		ReportErrorAndUnload("SledgeLib failed to load");
}

/*
	called when the DLL is detached
*/
void Loader::Shutdown() {
	SledgeLib::Shutdown();
	Teardown::UndoHooks();
	SledgeVR::Shutdown();
}

/*
	called once game is instantiated
*/
void Loader::GameInit() {
	if (!SledgeLib::Init())
		ReportErrorAndUnload("SledgeLib failed to init");
}