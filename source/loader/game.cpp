#include "game.h"
#include "globals.h"

#include <string>
#include <filesystem>
#include <fstream>
#include <regex>

#include <windef.h>
#include <winreg.h>

#include <processenv.h>
#include <processthreadsapi.h>
#include <handleapi.h>
#include <synchapi.h>

#include <detours.h>

#define MAX_PATH_PLUS_ONE 261

// from: winerror.h
#define ERROR_SUCCESS 0L

// from: WinBase.h
#define CREATE_DEFAULT_ERROR_MODE 0x04000000
#define CREATE_SUSPENDED 0x00000004
#define INFINITE 0xFFFFFFFF

std::string sInstallDir;

bool Game::Launch() {
	if (!g_TeardownPath)
		return 0;

	char cCurrPath[MAX_PATH] = {};
	GetCurrentDirectoryA(MAX_PATH, cCurrPath);

	PROCESS_INFORMATION ProcInfo;
	STARTUPINFOA StartupInfo;

	ZeroMemory(&ProcInfo, sizeof(ProcInfo));
	ZeroMemory(&StartupInfo, sizeof(StartupInfo));
	StartupInfo.cb = sizeof(StartupInfo);

	if (!CreateProcessA(NULL, const_cast<LPSTR>(g_TeardownPath), NULL, NULL, TRUE, CREATE_DEFAULT_ERROR_MODE | CREATE_SUSPENDED, NULL, sInstallDir.c_str(), &StartupInfo, &ProcInfo))
		return 0;

	char cDLLPath[MAX_PATH];
	sprintf(cDLLPath, "%s\\%s", cCurrPath, "sledge.dll");
	const char* cDLLPath2 = cDLLPath;

	if (!std::filesystem::exists(cDLLPath))
		return 0;

	if (!DetourUpdateProcessWithDll(ProcInfo.hProcess, &cDLLPath2, 1))
		return 0;

	ResumeThread(ProcInfo.hThread);

	//WaitForSingleObject(ProcInfo.hProcess, INFINITE);

	//CloseHandle(ProcInfo.hProcess);
	//CloseHandle(ProcInfo.hThread);

	return 1;
}

bool Game::GetTeardownPath() {
	char cSteamPath[MAX_PATH_PLUS_ONE];
	HKEY SteamKey;

	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\WOW6432Node\\Valve\\Steam", 0, KEY_QUERY_VALUE, &SteamKey) == ERROR_SUCCESS) {
		DWORD dwLen = MAX_PATH;
		if (RegQueryValueExA(SteamKey, "InstallPath", 0, 0, reinterpret_cast<LPBYTE>(&cSteamPath), &dwLen) == ERROR_SUCCESS)
			cSteamPath[dwLen] = '\0';
		else
			return false;

		RegCloseKey(SteamKey);
	}
	else
		return false;

	std::string sSteamPath = std::string(cSteamPath);
	if (sSteamPath.empty())
		return false;

	std::string sTeardownPath = sSteamPath + "\\steamapps\\common\\Teardown";
	if (std::filesystem::exists(sTeardownPath + "\\teardown.exe.unpacked.exe")) {
		memcpy(g_TeardownPath, (sTeardownPath + "\\teardown.exe.unpacked.exe").c_str(), MAX_PATH_PLUS_ONE);
		sInstallDir = sTeardownPath;
		return true;
	} else if(std::filesystem::exists(sTeardownPath + "\\teardown.exe")) {
		memcpy(g_TeardownPath, (sTeardownPath + "\\teardown.exe").c_str(), MAX_PATH_PLUS_ONE);
		sInstallDir = sTeardownPath;
		return true;
	}


	std::string sConfigPath = sSteamPath + "\\steamapps\\libraryfolders.vdf";

	std::ifstream ConfigFile(sConfigPath);
	if (!ConfigFile.is_open())
		return false;

	std::string sConfigContent = std::string(std::istreambuf_iterator<char>(ConfigFile), std::istreambuf_iterator<char>());
	std::regex DirRegex("\"[^\"]+\"[\\s]+\"([^\"]+)\"\\n", std::regex::ECMAScript);

	std::regex_iterator MatchesBegin = std::sregex_iterator(sConfigContent.begin(), sConfigContent.end(), DirRegex);

	for (std::sregex_iterator Match = MatchesBegin; Match != std::sregex_iterator(); ++Match)
	{
		sTeardownPath = (*Match)[1].str() + "\\steamapps\\common\\Teardown";

		if (std::filesystem::exists(sTeardownPath)) {
			sTeardownPath.replace(sTeardownPath.find("\\\\"), 2, "\\");

			if (std::filesystem::exists(sTeardownPath + "\\teardown.exe.unpacked.exe")) {
				memcpy(g_TeardownPath, (sTeardownPath + "\\teardown.exe.unpacked.exe").c_str(), MAX_PATH_PLUS_ONE);
				sInstallDir = sTeardownPath;
				return true;
			}
			else if (std::filesystem::exists(sTeardownPath + "\\teardown.exe")) {
				memcpy(g_TeardownPath, (sTeardownPath + "\\teardown.exe").c_str(), MAX_PATH_PLUS_ONE);
				sInstallDir = sTeardownPath;
				return true;
			}
		}
	}

	return false;
}