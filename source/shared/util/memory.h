#pragma once
#include <minwindef.h>

namespace Memory {
	inline DWORD64 dwReadPtr(DWORD64 dwAddress, UINT iOffset) {	
        // reference:
		// DWORD dwRelOffset = *(DWORD64*)(dwAddress + iOffset)
        // DWORD64 dwReturn = dwAddress + dwRelOffset + sizeof(dwRelOffset) + iOffset;

        DWORD dwOffset = *(DWORD*)(dwAddress + iOffset);
		return *(DWORD64*)(dwAddress + dwOffset + sizeof(int) + iOffset);
	}
}