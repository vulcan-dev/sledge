#include <minwindef.h>
#include <libloaderapi.h>

#include "sledge/loader.h"

/*
	empty export required by MS Detours
*/
__declspec(dllexport) int __stdcall _(void) { return 0; }

BOOL DllMain(HMODULE hMod, DWORD dwReason, LPVOID) {
	switch (dwReason) {
	case DLL_PROCESS_ATTACH:
		Loader::Init(hMod);
		break;
	case DLL_PROCESS_DETACH:
		Loader::Shutdown();
		break;
	}

	return TRUE;
}