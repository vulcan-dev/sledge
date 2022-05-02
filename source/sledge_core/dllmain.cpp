#include <minwindef.h>
#include <libloaderapi.h>

#include "sledge/loader.h"

/*
	empty export required by MS Detours
*/
__declspec(dllexport) int __stdcall _(void) { return 0; }

BOOL DllMain(HMODULE hMod, DWORD dwReason, LPVOID) {
	if (dwReason != DLL_PROCESS_ATTACH)
		return TRUE;

	Loader::Init(hMod);
	DisableThreadLibraryCalls(hMod);
	return TRUE;
}