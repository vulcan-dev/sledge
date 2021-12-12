#include <minwindef.h>

// empty export cause ms detours said so
__declspec(dllexport) int __stdcall _(void) { return 0; }

int __stdcall DllMain(HMODULE, DWORD dwReason, LPVOID) {
	switch (dwReason) {
	case DLL_PROCESS_ATTACH:
		break;
	}

	return true;
}