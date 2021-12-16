#include "teardown.h"
#include "ui/window.h"

#include <string>
#include <filesystem>
#include <fstream>
#include <regex>

#include <windef.h>
#include <winreg.h>

#include <handleapi.h>
#include <synchapi.h>
#include <processthreadsapi.h>
#include <processenv.h>

#include <detours.h>

// from: winerror.h
#define ERROR_SUCCESS 0L

// from: WinBase.h
#define CREATE_DEFAULT_ERROR_MODE 0x04000000
#define CREATE_SUSPENDED 0x00000004
#define INFINITE 0xFFFFFFFF

void Teardown::Launch() {
	const char* cTeardownPath = Teardown::GetPath();

	char cCurrentPath[MAX_PATH];
	GetCurrentDirectoryA(MAX_PATH, cCurrentPath);

	char cDLLPath[MAX_PATH];
	sprintf(cDLLPath, "%s\\%s", cCurrentPath, "sledge.dll");
	const char* cDLLPath2 = cDLLPath;

	if (!std::filesystem::exists(cDLLPath))
		return;

	char cExePath[MAX_PATH];
	sprintf(cExePath, "%s\\%s", cTeardownPath, "teardown.exe.unpacked.exe");

	if (!std::filesystem::exists(cExePath))
		sprintf(cExePath, "%s\\%s", cTeardownPath, "teardown.exe");

	PROCESS_INFORMATION ProcInfo;
	STARTUPINFOA StartupInfo;

	ZeroMemory(&ProcInfo, sizeof(ProcInfo));
	ZeroMemory(&StartupInfo, sizeof(StartupInfo));


	if (!CreateProcessA(NULL, const_cast<LPSTR>(cExePath), NULL, NULL, TRUE, CREATE_DEFAULT_ERROR_MODE | CREATE_SUSPENDED, NULL, cTeardownPath, &StartupInfo, &ProcInfo))
		return;

	if (!DetourUpdateProcessWithDll(ProcInfo.hProcess, &cDLLPath2, 1))
		return;

	ResumeThread(ProcInfo.hThread);

	Window::Close();

	WaitForSingleObject(ProcInfo.hProcess, INFINITE);

	CloseHandle(ProcInfo.hProcess);
	CloseHandle(ProcInfo.hThread);
}

const char* Teardown::GetPath() {
	char cSteamPath[MAX_PATH];
	HKEY SteamKey;

	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\WOW6432Node\\Valve\\Steam", 0, KEY_QUERY_VALUE, &SteamKey) == ERROR_SUCCESS) {
		DWORD dwLen = MAX_PATH;
		if (RegQueryValueExA(SteamKey, "InstallPath", 0, 0, reinterpret_cast<LPBYTE>(&cSteamPath), &dwLen) == ERROR_SUCCESS)
			cSteamPath[dwLen] = '\0';
		else
			return 0;

		RegCloseKey(SteamKey);
	}
	else
		return 0;

	std::string sSteamPath = std::string(cSteamPath);
	if (sSteamPath.empty())
		return 0;

	char* cTeardownPath = new char[MAX_PATH];

	std::string sTeardownPath = sSteamPath + "\\steamapps\\common\\Teardown";
	if (std::filesystem::exists(sTeardownPath + "\\teardown.exe")) {
		memcpy(cTeardownPath, sTeardownPath.c_str(), MAX_PATH);
		return cTeardownPath;
	}
	std::ifstream ConfigFile(sSteamPath + "\\steamapps\\libraryfolders.vdf");
	if (!ConfigFile.is_open())
		return 0;

	std::string sConfigContent = std::string(std::istreambuf_iterator<char>(ConfigFile), std::istreambuf_iterator<char>());
	std::regex DirRegex("\"[^\"]+\"[\\s]+\"([^\"]+)\"\\n", std::regex::ECMAScript);

	std::regex_iterator SteamDirectories = std::sregex_iterator(sConfigContent.begin(), sConfigContent.end(), DirRegex);


	for (std::sregex_iterator Match = SteamDirectories; Match != std::sregex_iterator(); ++Match)
	{
		sTeardownPath = (*Match)[1].str() + "\\steamapps\\common\\Teardown";

		if (std::filesystem::exists(sTeardownPath)) {
			sTeardownPath.replace(sTeardownPath.find("\\\\"), 2, "\\");

			if (std::filesystem::exists(sTeardownPath + "\\teardown.exe")) {
				memcpy(cTeardownPath, sTeardownPath.c_str(), MAX_PATH);
				return cTeardownPath;
			}
		}
	}
	return 0;
}