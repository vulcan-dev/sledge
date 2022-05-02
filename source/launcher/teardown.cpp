#include "teardown.h"
#include "steam.h"

#include <fmt/format.h>

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

	std::string sExePath = fmt::format("{}\\{}", cTeardownPath, "teardown.exe");
	const char* cExePath = sExePath.c_str();
	if (!std::filesystem::exists(sExePath)) {
		MessageBoxA(NULL, "Unable to find teardown.exe", "Error", MB_ICONERROR | MB_OK);
		return;
	}

	/*
		get path of dlls
	*/
	std::string sSledgePath = fmt::format("{}\\bin\\sledge_core.dll", cCurrentPath);
	std::string sDumperPath = fmt::format("{}\\bin\\dumper.dll", cCurrentPath);
	std::string sOpenVRPath = fmt::format("{}\\bin\\openvr_api.dll", cCurrentPath);

	const char* cSledgePath = sSledgePath.c_str();
	const char* cDumperPath = sDumperPath.c_str();
	const char* cOpenVRPath = sOpenVRPath.c_str();

	if (!std::filesystem::exists(cSledgePath)) {
		MessageBoxA(NULL, "Unable to find sledge.dll", "Error", MB_ICONERROR | MB_OK);
		return;
	}

	/*
		set up dll directory and env variablesbefore starting teardown
	*/
	std::string sUltralightPath = fmt::format("{}\\ultralight", cCurrentPath);
	SetDllDirectory(sUltralightPath.c_str());
	SetEnvironmentVariableA("SteamAppId", "1167630");

	/*
		parse command line arguments
	*/
	std::string sTeardownCmdLine(cExePath);
	char* cCMDLine = GetCommandLine();

	if (strstr(cCMDLine, "-nosplash"))
		sTeardownCmdLine.append(" -nosplash");

	bool bVR = false;
	if (strstr(cCMDLine, "-vr")) {
		bVR = true;
		sTeardownCmdLine.append(" -vr");
	}

	bool bWriteDump = false;
	if (strstr(cCMDLine, "-dump"))
		bWriteDump = true;

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
		remove steamstub stuff
	*/
	Steam::Defuse(ProcInfo.hProcess, ProcInfo.hThread, pExeBuffer);

	/*
		attach sledge
	*/
	if (!bWriteDump) {
		if (!DetourUpdateProcessWithDll(ProcInfo.hProcess, &cSledgePath, 1)) {
			MessageBoxA(NULL, "DetourUpdateProcessWithDll failed", "Error", MB_ICONERROR | MB_OK);
			CloseHandle(ProcInfo.hProcess);
			CloseHandle(ProcInfo.hThread);
			return;
		}

		if (!DetourUpdateProcessWithDll(ProcInfo.hProcess, &cOpenVRPath, 1)) {
			MessageBoxA(NULL, "DetourUpdateProcessWithDll failed", "Error", MB_ICONERROR | MB_OK);
			CloseHandle(ProcInfo.hProcess);
			CloseHandle(ProcInfo.hThread);
			return;
		}
	}
	else {
		if (!std::filesystem::exists(cDumperPath)) {
			MessageBoxA(NULL, "Unable to find dumper.dll", "Error", MB_ICONERROR | MB_OK);
			CloseHandle(ProcInfo.hProcess);
			CloseHandle(ProcInfo.hThread);
			return;
		}

		if (!DetourUpdateProcessWithDll(ProcInfo.hProcess, &cDumperPath, 1)) {
			MessageBoxA(NULL, "DetourUpdateProcessWithDll failed", "Error", MB_ICONERROR | MB_OK);
			CloseHandle(ProcInfo.hProcess);
			CloseHandle(ProcInfo.hThread);
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