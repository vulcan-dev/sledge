#include <minwindef.h>
#include <libloaderapi.h>

#include "sledge/loader.h"

// empty export cause ms detours said so
__declspec(dllexport) int __stdcall _(void) { return 0; }

BOOL DllMain(HMODULE hModule, DWORD dwReason, LPVOID) {
	if (dwReason != DLL_PROCESS_ATTACH)
		return TRUE;
		
	Loader::Init(hModule);
	DisableThreadLibraryCalls(hModule);

	return TRUE;
}