#include <minwindef.h>

#include "sledge/loader.h"

// empty export cause ms detours said so
__declspec(dllexport) int __stdcall _(void) { return 0; }

int __stdcall DllMain(HMODULE hModule, DWORD dwReason, LPVOID) {
	switch (dwReason) {
	case DLL_PROCESS_ATTACH:
		Loader::Init(hModule);
		break;
	}

	return true;
}