#include "dumper.h"
#include "signatures.h"
#include "util/memory.h"

#include <string>
#include <sstream>
#include <fstream>

#include <processthreadsapi.h>
#include <libloaderapi.h>

#include <detours.h>

// from: WinUser.h
#define MB_OK                       0x00000000L
#define MB_ICONERROR				0x00000010L

typedef int (*tMessageBoxA) (void* hWnd, const char* lpText, const char* lpCaption, unsigned intuType);
tMessageBoxA MessageBoxA;

typedef void* (*tGameConstructor) (void* pMem, void* pDevice);
tGameConstructor GameConstructor;

std::string sModulePath;

void Dumper::Run(void* hMod) {
	/*
		get MessageBoxA
	*/
	HMODULE hUser32 = LoadLibraryA("user32.dll");
	if (!hUser32)
		return;
	MessageBoxA = reinterpret_cast<tMessageBoxA>(GetProcAddress(hUser32, "MessageBoxA"));

	/*
		get current module path (for writing result)
	*/
	char cModuleName[MAX_PATH];
	GetModuleFileNameA(reinterpret_cast<HMODULE>(hMod), cModuleName, MAX_PATH);

	std::string sModuleName(cModuleName);
	sModulePath = sModuleName.substr(0, sModuleName.find_last_of('\\'));

	/*
		scan and write addresses
	*/
	DWORD64 dwBaseAddr = reinterpret_cast<DWORD64>(GetModuleHandleA(NULL));

	std::stringstream ssResult;
	ssResult << "#pragma once" << std::endl << "#include <unordered_map>" << std::endl << "#include <string>" << std::endl << std::endl;
	ssResult << "inline std::unordered_map<std::string, unsigned __int64> g_Offsets = {" << std::endl;

	for (std::pair<std::string, CSignature*> SigPair : g_SigMap) {
		DWORD64 dwSig = Memory::dwFindPattern(*SigPair.second);

		if (dwSig == 0) {
			std::string ssError("Failed to find signature: " + SigPair.first);
			MessageBoxA(NULL, ssError.c_str(), "Dump failed!", MB_ICONERROR | MB_OK);
			exit(0);
		}
		ssResult << "\t {\"" << SigPair.first << "\", 0x" << reinterpret_cast<void*>(dwSig - dwBaseAddr) << " }," << std::endl;
	}

	ssResult << "};";

	std::ofstream ResultFile(sModulePath + "\\offsets.h");
	ResultFile << ssResult.str();
	ResultFile.close();

	MessageBoxA(0, "Saved addresses to dump.txt", "Dump successful", MB_OK);

	exit(0);
}