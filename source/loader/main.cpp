#include <windef.h>

#include <string>
#include <regex>
#include <filesystem>
#include <fstream>

#include <synchapi.h>
#include <handleapi.h>
#include <processthreadsapi.h>
#include <processenv.h>
#include <winreg.h>

#include <detours.h>

/*
	to-do: add warnings for errors / a good looking loader ui
*/

// from: winerror.h
#define ERROR_SUCCESS 0L

// from: WinBase.h
#define CREATE_DEFAULT_ERROR_MODE 0x04000000
#define CREATE_SUSPENDED 0x00000004
#define INFINITE 0xFFFFFFFF

#define MAX_PATH_PLUS_ONE 261

std::string GetTeardownPath() {
	HKEY SteamKey;
	char cSteamPath[MAX_PATH_PLUS_ONE];

	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\WOW6432Node\\Valve\\Steam", 0, KEY_QUERY_VALUE, &SteamKey) == ERROR_SUCCESS) {
		DWORD dwLen = MAX_PATH;
		if (RegQueryValueExA(SteamKey, "InstallPath", 0, 0, reinterpret_cast<LPBYTE>(&cSteamPath), &dwLen) == ERROR_SUCCESS)
			cSteamPath[dwLen] = '\0';
		else
			return "";
		RegCloseKey(SteamKey);
	}
	else
		return "";

	std::string sSteamPath = std::string(cSteamPath);
	if (sSteamPath.empty())
		return "";

	std::string sTeardownPath = sSteamPath + "\\steamapps\\common\\Teardown";
	if (std::filesystem::exists(sTeardownPath))
		return sTeardownPath + "\\teardown.exe";;

	std::string sConfigPath = sSteamPath + "\\steamapps\\libraryfolders.vdf";

	std::ifstream ConfigFile(sConfigPath);
	if (!ConfigFile.is_open())
		return "";

	std::string sConfigContent = std::string(std::istreambuf_iterator<char>(ConfigFile), std::istreambuf_iterator<char>());
	std::regex DirRegex("\"[^\"]+\"[\\s]+\"([^\"]+)\"\\n", std::regex::ECMAScript);

	std::regex_iterator MatchesBegin = std::sregex_iterator(sConfigContent.begin(), sConfigContent.end(), DirRegex);

	for (std::sregex_iterator Match = MatchesBegin; Match != std::sregex_iterator(); ++Match)
	{
		sTeardownPath = (*Match)[1].str() + "\\steamapps\\common\\Teardown";

		if (std::filesystem::exists(sTeardownPath)) {
			sTeardownPath.replace(sTeardownPath.find("\\\\"), 2, "\\");
			return sTeardownPath + "\\teardown.exe";
		}
	}

	return "";
}

int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	char cCurrPath[MAX_PATH_PLUS_ONE];
	GetCurrentDirectoryA(MAX_PATH, cCurrPath);

	std::string sInstallDir = GetTeardownPath();

	if (sInstallDir.empty())
		return 0;

	char cExePath[MAX_PATH] = {};
	if (std::filesystem::exists(sInstallDir + "\\teardown.unpacked.exe"))
		sprintf(cExePath, "%s\\teardown.unpacked.exe", sInstallDir.c_str());
	else if (std::filesystem::exists(sInstallDir + "\\teardown.exe")) 
		sprintf(cExePath, "%s\\teardown.exe", sInstallDir.c_str());
	else
		return 0;

	PROCESS_INFORMATION ProcInfo;
	STARTUPINFOA StartupInfo;

	ZeroMemory(&ProcInfo, sizeof(ProcInfo));
	ZeroMemory(&StartupInfo, sizeof(StartupInfo));
	StartupInfo.cb = sizeof(StartupInfo);

	if (!CreateProcessA(NULL, const_cast<LPSTR>(cExePath), NULL, NULL, TRUE, CREATE_DEFAULT_ERROR_MODE | CREATE_SUSPENDED, NULL, sInstallDir.c_str(), &StartupInfo, &ProcInfo))
		return 0;

	char cDLLPath[MAX_PATH];
	sprintf(cDLLPath, "%s\\%s", cCurrPath, "auburn.dll");
	const char* cDLLPath2 = cDLLPath;

	if (!std::filesystem::exists(cDLLPath)) 
		return 0;

	if (!DetourUpdateProcessWithDll(ProcInfo.hProcess, &cDLLPath2, 1))
		return 0;

	ResumeThread(ProcInfo.hThread);

	WaitForSingleObject(ProcInfo.hProcess, INFINITE);

	CloseHandle(ProcInfo.hProcess);
	CloseHandle(ProcInfo.hThread);

	return 1;
}