/*
	to-do:
		confirm if std::call_once is actually faster than an if check
*/

#include "memory.h"
#include "globals.h"

#include <minwindef.h>
#include <Psapi.h>
#include <processthreadsapi.h>

#include <mutex>


DWORD64 dwBaseAddress;
DWORD dwModuleSize;

std::once_flag fHasModuleInfo;

bool bPatternCompare(const char* cData, const char* cPattern, const char* cMask) {
	for (; *cMask; ++cMask, ++cData, ++cPattern) {
		if (*cMask == 'x' && *cData != *cPattern)
			return false;
	}
	return true;
}

void GetModuleInfo() {
	MODULEINFO ModuleInfo = { 0 };
	GetModuleInformation(GetCurrentProcess(), reinterpret_cast<HMODULE>(g_Module), &ModuleInfo, sizeof(MODULEINFO));

	dwBaseAddress = reinterpret_cast<DWORD64>(g_Module);
	dwModuleSize = ModuleInfo.SizeOfImage;
}

DWORD64 Memory::dwReadPtr(DWORD64 dwAddress, UINT iOffset) {
	// reference:
	// DWORD dwRelOffset = *(DWORD64*)(dwAddress + iOffset)
	// DWORD64 dwFinalAddr = dwAddress + dwRelOffset + sizeof(dwRelOffset) + iOffset;

	DWORD dwOffset = *(DWORD*)(dwAddress + iOffset);
	return dwAddress + dwOffset + sizeof(int) + iOffset;
}

unsigned __int64 Memory::dwFindPattern(CSignature Sig) {
	std::call_once(fHasModuleInfo, GetModuleInfo); // using call_once cause it might be faster than just checking if dwImageSize isn't 0

	for (unsigned __int64 i = 0; i < dwModuleSize; i++) {
		if (bPatternCompare(reinterpret_cast<char*>(dwBaseAddress + i), Sig.m_Pattern, Sig.m_Mask)) {
			if (Sig.m_DirectRef)
				return Memory::dwReadPtr(dwBaseAddress + i, 1);
			return dwBaseAddress + i;
		}
	}

	return 0;
}