#include "game.h"
#include "globals.h"

#include <string>
#include <filesystem>
#include <fstream>
#include <regex>

#include <winreg.h>

// from: winerror.h
#define ERROR_SUCCESS 0L

#define MAX_PATH_PLUS_ONE 261

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
	if (std::filesystem::exists(sTeardownPath)) {
		memcpy(g_TeardownPath, (sTeardownPath + "\\teardown.exe").c_str(), MAX_PATH_PLUS_ONE);
		return false;
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
			memcpy(g_TeardownPath, (sTeardownPath + "\\teardown.exe").c_str(), MAX_PATH_PLUS_ONE);
			return true;
		}
	}

	return false;
}