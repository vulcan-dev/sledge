#include <minwindef.h>

#include "teardown.h"

int WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	Teardown::Launch();
	return 1;
}