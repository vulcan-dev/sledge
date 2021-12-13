#include <minwindef.h>
#include "loader.h"

int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	Loader::Init();
	return 1;
}