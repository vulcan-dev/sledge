#include "teardown.h"
#include "steam.h"

#include <string>
#include <filesystem>
#include <fstream>
#include <regex>

#include <windef.h>
#include <winreg.h>
#include <WinUser.h>
#include <WinBase.h>

#include <detours.h>

// from: winerror.h
#define ERROR_SUCCESS 0L

const char* Teardown::GetPath() {
	/*
		get steam's install dir
	*/
	char cSteamPath[MAX_PATH];
	HKEY SteamKey;

	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\WOW6432Node\\Valve\\Steam", 0, KEY_QUERY_VALUE, &SteamKey) == ERROR_SUCCESS) {
		DWORD dwLen = MAX_PATH;
		if (RegQueryValueExA(SteamKey, "InstallPath", 0, 0, reinterpret_cast<LPBYTE>(&cSteamPath), &dwLen) == ERROR_SUCCESS)
			cSteamPath[dwLen - 1] = '\0';
		else
			return 0;

		RegCloseKey(SteamKey);
	}
	else
		return 0;

	std::string sSteamPath = std::string(cSteamPath);
	if (sSteamPath.empty())
		return 0;

	/*
		try the default teardown install dir
	*/
	char* cTeardownPath = new char[MAX_PATH];

	std::string sTeardownPath = sSteamPath + "\\steamapps\\common\\Teardown";
	if (std::filesystem::exists(sTeardownPath + "\\teardown.exe")) {
		memcpy(cTeardownPath, sTeardownPath.c_str(), MAX_PATH);
		return cTeardownPath;
	}

	/*
		get all library folders
	*/
	std::ifstream ConfigFile(sSteamPath + "\\steamapps\\libraryfolders.vdf");
	if (!ConfigFile.is_open())
		return 0;

	std::string sConfigContent = std::string(std::istreambuf_iterator<char>(ConfigFile), std::istreambuf_iterator<char>());
	std::regex DirRegex("\"[^\"]+\"[\\s]+\"([^\"]+)\"\\n", std::regex::ECMAScript);

	std::regex_iterator LibraryFolders = std::sregex_iterator(sConfigContent.begin(), sConfigContent.end(), DirRegex);

	/*
		iterate through all library folders and check if teardown is there
	*/
	for (std::sregex_iterator Match = LibraryFolders; Match != std::sregex_iterator(); ++Match)
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

void Teardown::Launch() {

	/*
		get teardown's install dir, check if exe exists
	*/
	const char* cTeardownPath = Teardown::GetPath();

	if (cTeardownPath == 0) {
		MessageBoxA(NULL, "Unable to find Teardown installation", "Error", MB_ICONERROR | MB_OK);
		return;
	}

	char cCurrentPath[MAX_PATH];
	GetCurrentDirectoryA(MAX_PATH, cCurrentPath);

	char cExePath[MAX_PATH];
	sprintf(cExePath, "%s\\%s", cTeardownPath, "teardown.exe");
	if (!std::filesystem::exists(cExePath)) {
		MessageBoxA(NULL, "Unable to find teardown.exe", "Error", MB_ICONERROR | MB_OK);
		return;
	}

	/*
		get path of dlls
	*/

	char cDLLPath[MAX_PATH];
	sprintf(cDLLPath, "%s\\%s", cCurrentPath, "\\bin\\sledge_core.dll");
	const char* cDLLPath2 = cDLLPath;

	if (!std::filesystem::exists(cDLLPath)) {
		MessageBoxA(NULL, "Unable to find sledge_core.dll", "Error", MB_ICONERROR | MB_OK);
		return;
	}

	/*
		set up path variables before starting teardown
	*/
	char cUltralightDir[MAX_PATH];
	sprintf(cUltralightDir, "%s\\ultralight", cCurrentPath);
	SetDllDirectory(cUltralightDir);

	SetEnvironmentVariableA("SteamAppId", "1167630");

	/*
		parse command line arguments
	*/
	std::string sTeardownCmdLine(cExePath);

	char* cCMDLine = GetCommandLine();
	if (strstr(cCMDLine, "-nosplash"))
		sTeardownCmdLine = sTeardownCmdLine + " -nosplash";

	bool bWriteDump = false;
	if (strstr(cCMDLine, "-dump"))
		bWriteDump = true;

	/*
		create suspended process
	*/
	PROCESS_INFORMATION ProcInfo;
	STARTUPINFOA StartupInfo;

	ZeroMemory(&ProcInfo, sizeof(ProcInfo));
	ZeroMemory(&StartupInfo, sizeof(StartupInfo));

	if (!CreateProcessA(NULL, const_cast<LPSTR>(sTeardownCmdLine.c_str()), NULL, NULL, TRUE, CREATE_DEFAULT_ERROR_MODE | CREATE_SUSPENDED, NULL, cTeardownPath, &StartupInfo, &ProcInfo)) {
		MessageBoxA(NULL, "CreateProcessA failed when starting teardown.unpacked.exe", "Error", MB_ICONERROR | MB_OK);
		return;
	}

	/*
		read teardown.exe into buffer
	*/
	FILE* TeardownExe = fopen(cExePath, "rb");
	if (TeardownExe == NULL) {
		MessageBoxA(NULL, "Unable to open teardown.exe", "Error", MB_ICONERROR | MB_OK);
		return;
	}

	fseek(TeardownExe, 0, SEEK_END);
	long lFileSize = ftell(TeardownExe);
	rewind(TeardownExe);

	void* pExeBuffer = malloc(lFileSize);
	if (pExeBuffer == NULL) {
		MessageBoxA(NULL, "Unable to allocate memory for exe buffer", "Error", MB_ICONERROR | MB_OK);
		return;
	}

	fread(pExeBuffer, lFileSize, 1, TeardownExe);
	fclose(TeardownExe);

	/*
		remove steamstub stuff
	*/
	Steam::Defuse(ProcInfo.hProcess, ProcInfo.hThread, pExeBuffer);

	/*
		attach sledge or dumper
	*/
	if (!bWriteDump) {
		if (!DetourUpdateProcessWithDll(ProcInfo.hProcess, &cDLLPath2, 1)) {
			MessageBoxA(NULL, "DetourUpdateProcessWithDll failed", "Error", MB_ICONERROR | MB_OK);
			return;
		}
	}
	else {
		char cDumperPath[MAX_PATH];
		sprintf(cDumperPath, "%s\\%s", cCurrentPath, "dumper.dll");
		const char* cDumperPath2 = cDumperPath;

		if (!std::filesystem::exists(cDumperPath2)) {
			MessageBoxA(NULL, "Unable to find dumper.dll", "Error", MB_ICONERROR | MB_OK);
			return;
		}

		if (!DetourUpdateProcessWithDll(ProcInfo.hProcess, &cDumperPath2, 1)) {
			MessageBoxA(NULL, "DetourUpdateProcessWithDll failed", "Error", MB_ICONERROR | MB_OK);
			return;
		}
	}

	/*
		resume thread and get close handles once process closes	
	*/
	ResumeThread(ProcInfo.hThread);
	WaitForSingleObject(ProcInfo.hProcess, INFINITE);
	CloseHandle(ProcInfo.hProcess);
	CloseHandle(ProcInfo.hThread);
}