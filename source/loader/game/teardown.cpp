#include "game/teardown.h"
#include "game/steam.h"
#include "ui/window.h"

#include <string>
#include <filesystem>
#include <fstream>
#include <regex>

#include <windef.h>
#include <winreg.h>
#include <WinUser.h>
#include <WinBase.h>

#include <handleapi.h>
#include <synchapi.h>
#include <processthreadsapi.h>
#include <processenv.h>
#include <memoryapi.h>

#include <detours.h>

// from: winerror.h
#define ERROR_SUCCESS 0L

// from: WinBase.h
#define CREATE_DEFAULT_ERROR_MODE 0x04000000
#define CREATE_SUSPENDED 0x00000004
#define INFINITE 0xFFFFFFFF

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

	char cDLLPath[MAX_PATH];
	sprintf(cDLLPath, "%s\\%s", cCurrentPath, "sledge.dll");
	const char* cDLLPath2 = cDLLPath;

	if (!std::filesystem::exists(cDLLPath)) {
		MessageBoxA(NULL, "Unable to find sledge.dll", "Error", MB_ICONERROR | MB_OK);
		return;
	}

	char cExePath[MAX_PATH];
	sprintf(cExePath, "%s\\%s", cTeardownPath, "teardown.exe");
	if (!std::filesystem::exists(cExePath)) {
		MessageBoxA(NULL, "Unable to find teardown.exe", "Error", MB_ICONERROR | MB_OK);
		return;
	}

	/*
		read packed exe into a buffer, unpack the buffer, write buffer to exe
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

	void* pUnpackedBuffer = Steam::GetUnpackedExe(pExeBuffer, lFileSize);
	free(pExeBuffer);

	if (pUnpackedBuffer == nullptr) {
		MessageBoxA(NULL, "Failed to unpack Teardown", "Error", MB_ICONERROR | MB_OK);
		return;
	}

	char cUnpackedDir[MAX_PATH];
	sprintf(cUnpackedDir, "%s\\teardown.unpacked.exe", cTeardownPath);
	FILE* UnpackedOutput = fopen(cUnpackedDir, "wb");
	if (UnpackedOutput == NULL) {
		MessageBoxA(NULL, "Unable to open teardown.unpacked.exe (is Sledge already running?)", "Error", MB_ICONERROR | MB_OK);
		return;
	}

	if (!fwrite(pUnpackedBuffer, 1, lFileSize, UnpackedOutput)) {
		MessageBoxA(NULL, "Unable to write to teardown.unpacked.exe", "Error", MB_ICONERROR | MB_OK);
		return;
	}
	fclose(UnpackedOutput);

	/*
		launch unpacked exe
	*/
	char cUltralightDir[MAX_PATH];
	sprintf(cUltralightDir, "%s\\ultralight", cCurrentPath);
	SetDllDirectory(cUltralightDir);

	SetEnvironmentVariableA("SteamAppId", "1167630"); // Set SteamAppId var to initialize SteamAPI

	PROCESS_INFORMATION ProcInfo;
	STARTUPINFOA StartupInfo;

	ZeroMemory(&ProcInfo, sizeof(ProcInfo));
	ZeroMemory(&StartupInfo, sizeof(StartupInfo));


	if (!CreateProcessA(NULL, const_cast<LPSTR>(cUnpackedDir), NULL, NULL, TRUE, CREATE_DEFAULT_ERROR_MODE | CREATE_SUSPENDED, NULL, cTeardownPath, &StartupInfo, &ProcInfo)) {
		MessageBoxA(NULL, "CreateProcessA failed when starting teardown.unpacked.exe", "Error", MB_ICONERROR | MB_OK);
		return;
	}

	if (!DetourUpdateProcessWithDll(ProcInfo.hProcess, &cDLLPath2, 1)) {
		MessageBoxA(NULL, "DetourUpdateProcessWithDll failed", "Error", MB_ICONERROR | MB_OK);
		return;
	}

	ResumeThread(ProcInfo.hThread);

	Window::Close();

	WaitForSingleObject(ProcInfo.hProcess, INFINITE);

	CloseHandle(ProcInfo.hProcess);
	CloseHandle(ProcInfo.hThread);
}

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