#include <minwindef.h>
#include "ui/window.h"


int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	Window::Create();
	return 1;
}