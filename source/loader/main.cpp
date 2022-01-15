#include <minwindef.h>

#include "ui/window.h"
#include "game/teardown.h"
#include <filesystem>
#include <iostream>
#include <fstream>
#include "tinyjson.h"

#include <libloaderapi.h>
#include <consoleapi.h>
#include <processenv.h>
#include <filesystem>

#include "util/log.h"

struct SJsonCol {
	int r = 255;
	int g = 255;
	int b = 255;

	friend std::ostream& operator<<(std::ostream& os, const SJsonCol& col) {
		os << "\"" << col.r << "," << col.g << "," << col.b << "\"";
		return os;
	}
};

int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR lpCmdLine, int) {
#ifdef _DEBUG
	AllocConsole();

	freopen("CONOUT$", "w", stdout);

	HANDLE hConsole = GetStdHandle((DWORD)-11);

	// enable escape sequences
	DWORD dwMode = 0;
	GetConsoleMode(hConsole, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hConsole, dwMode);
#endif

	std::string sLogPath = std::filesystem::current_path().string() + "\\logs\\";
	LogInit("Launcher", sLogPath.c_str());

	LogInfo("Launcher Loaded");

	if (strstr(lpCmdLine, "-nolauncher")) {
		Teardown::Launch();
		return 1;
	}

	if (!std::filesystem::exists("config.json")) {
		std::ofstream config("config.json");
		tiny::TinyJson json;

		SJsonCol icol { 255, 118, 117 };
		SJsonCol hcol { 255, 138, 137 };
		SJsonCol acol { 255, 88, 87 };
		SJsonCol bgcol { 45, 52, 54 };

		json["button_icol"].Set(icol);
		json["button_hcol"].Set(hcol);
		json["button_acol"].Set(acol);
		json["bg_col"].Set(bgcol);

		std::string str = json.WriteJson();
		config << str;

		config.close();
	}

	Window::Create();
	return 1;
}