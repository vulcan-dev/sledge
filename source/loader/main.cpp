#include <minwindef.h>

#include "ui/window.h"
#include "game/teardown.h"

int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR lpCmdLine, int) {
	if (strstr(lpCmdLine, "-nolauncher")) {
		Teardown::Launch();
		return 1;
	}

	Window::Create();
	return 1;
}