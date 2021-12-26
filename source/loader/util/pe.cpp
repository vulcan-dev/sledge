#include "pe.h"

#include <minwindef.h>

DWORD64 PE::dwGetSectionFromVA(DWORD64 dwImageAddr, DWORD64 dwVA) {
	PIMAGE_DOS_HEADER DOSHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(dwImageAddr);
	if (DOSHeader->e_magic != IMAGE_DOS_SIGNATURE)
		return 0;

	PIMAGE_NT_HEADERS NTHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(dwImageAddr + DOSHeader->e_lfanew);
	if (NTHeaders->Signature != IMAGE_NT_SIGNATURE)
		return 0;

	DWORD64 dwSectionHeadersStart = dwImageAddr + DOSHeader->e_lfanew + sizeof(IMAGE_NT_HEADERS);

	for (int i = 0; i < NTHeaders->FileHeader.NumberOfSections; i++) {
		PIMAGE_SECTION_HEADER SectionHeader = reinterpret_cast<PIMAGE_SECTION_HEADER>(dwSectionHeadersStart + (sizeof(IMAGE_SECTION_HEADER) * i));

		DWORD64 dwSectionVA = SectionHeader->VirtualAddress;

		if ((dwVA >= dwSectionVA) && (dwVA <= dwSectionVA + SectionHeader->SizeOfRawData))
			return reinterpret_cast<DWORD64>(SectionHeader);
	}

	return 0;
}

DWORD64 PE::dwGetAddrFromVA(DWORD64 dwImageAddr, DWORD64 dwVA) {
	PIMAGE_SECTION_HEADER Section = reinterpret_cast<PIMAGE_SECTION_HEADER>(PE::dwGetSectionFromVA(dwImageAddr, dwVA));
	if (Section == nullptr)
		return 0;

	return dwImageAddr + (Section->PointerToRawData + (dwVA - Section->VirtualAddress));
}