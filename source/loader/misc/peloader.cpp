//#include "misc/peloader.h"
//#include "util/pe.h"
//
//#include <minwindef.h>
//#include <winnt.h>
//#include <libloaderapi.h>
//#include <memoryapi.h>
//#include <stdio.h>
//
///*
//	to-do:
//		on error, don't just return, free memory and get rid of everything
//*/
//
//
//typedef struct BASE_RELOCATION_ENTRY {
//	USHORT Offset : 12;
//	USHORT Type : 4;
//} BASE_RELOCATION_ENTRY, * PBASE_RELOCATION_ENTRY;
//
//
//void PELoader::Load(const char* cExePath, void* pPEData) {
//	DWORD64 dwPEData = reinterpret_cast<DWORD64>(pPEData);
//
//	/*
//		get headers
//	*/
//	PIMAGE_DOS_HEADER DOSHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(dwPEData);
//	if (DOSHeader->e_magic != IMAGE_DOS_SIGNATURE)
//		return;
//
//	PIMAGE_NT_HEADERS NTHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(dwPEData + DOSHeader->e_lfanew);
//	if (NTHeaders->Signature != IMAGE_NT_SIGNATURE)
//		return;
//
//	/*
//		alloc buffer for setting up and executing pe
//	*/
//	void* pImageBuff = VirtualAlloc(reinterpret_cast<LPVOID>(NTHeaders->OptionalHeader.ImageBase), NTHeaders->OptionalHeader.SizeOfImage, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
//	if (pImageBuff == NULL)
//		return;
//	DWORD64 dwImageBuff = reinterpret_cast<DWORD64>(pImageBuff);
//
//	/*
//	* http://undocumented.ntinternals.net/index.html?page=UserMode%2FUndocumented%20Functions%2FNT%20Objects%2FSection%2FNtUnmapViewOfSection.html
//	*/
//	typedef int (*tNtUnmapViewOfSection) (HANDLE ProcessHandle, PVOID BaseAddress);
//	HMODULE hNTDLL = LoadLibrary("ntdll.dll");
//	if (hNTDLL == NULL)
//		return;
//	reinterpret_cast<tNtUnmapViewOfSection>(GetProcAddress(hNTDLL, "NtUnmapViewOfSection"))(NULL, pImageBuff);;
//
//
//	/*
//		copy pe data to our buffer
//	*/
//	memcpy(pImageBuff, pPEData, NTHeaders->OptionalHeader.SizeOfHeaders);
//	DWORD64 dwSectionHeadersStart = dwPEData + DOSHeader->e_lfanew + sizeof(IMAGE_NT_HEADERS);
//	for (int i = 0; i < NTHeaders->FileHeader.NumberOfSections; i++) {
//		PIMAGE_SECTION_HEADER SectionHeader = reinterpret_cast<PIMAGE_SECTION_HEADER>(dwSectionHeadersStart + (sizeof(IMAGE_SECTION_HEADER) * i));
//		memcpy(reinterpret_cast<void*>(dwImageBuff + SectionHeader->VirtualAddress), reinterpret_cast<void*>(dwPEData + SectionHeader->PointerToRawData), SectionHeader->SizeOfRawData);
//	}
//
//	/*
//		rebuild import address table
//	*/
//	PIMAGE_DATA_DIRECTORY ImportData = &(NTHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT]);
//	DWORD64 dwImportDataAddr = dwImageBuff + ImportData->VirtualAddress;
//
//	PIMAGE_IMPORT_DESCRIPTOR ImportedDLL;
//
//	for (DWORD dwImportOffset = 0; dwImportOffset < ImportData->Size; dwImportOffset += sizeof(IMAGE_IMPORT_DESCRIPTOR)) {
//		ImportedDLL = reinterpret_cast<PIMAGE_IMPORT_DESCRIPTOR>(dwImportDataAddr + dwImportOffset);
//		char* cDLLName = reinterpret_cast<char*>(dwImageBuff + ImportedDLL->Name);
//
//		DWORD dwFirstThunk = ImportedDLL->FirstThunk;
//		DWORD dwOriginalFirstThunk = ImportedDLL->OriginalFirstThunk;
//
//		if (dwFirstThunk == 0 || dwOriginalFirstThunk == 0)
//			break;
//
//		HMODULE hDLL = LoadLibraryA(cDLLName);
//		if (hDLL == NULL) {
//			char cDLLBuff[MAX_PATH];
//			sprintf(cDLLBuff, "%s\\%s", cExePath, cDLLName);
//			hDLL = LoadLibraryA(cDLLBuff);
//			if (hDLL == NULL)
//				return;
//		}
//
//		DWORD dwFirstOffset = 0;
//		DWORD dwOriginOffset = 0;
//
//		while (true) {
//			PIMAGE_THUNK_DATA FirstThunk = reinterpret_cast<PIMAGE_THUNK_DATA>(dwImageBuff + dwFirstThunk + dwFirstOffset);
//			PIMAGE_THUNK_DATA OriginThunk = reinterpret_cast<PIMAGE_THUNK_DATA>(dwImageBuff + dwOriginalFirstThunk + dwOriginOffset);
//		
//			if (FirstThunk->u1.Function == NULL)
//				break;
//
//			if (OriginThunk->u1.Ordinal & IMAGE_ORDINAL_FLAG64) {
//				char* cFuncOrdinal = reinterpret_cast<char*>(OriginThunk->u1.Ordinal & 0xFFFF);
//				DWORD64 dwFunctionAddr = reinterpret_cast<DWORD64>(GetProcAddress(hDLL, cFuncOrdinal));
//				FirstThunk->u1.Function = dwFunctionAddr;
//			} else if (FirstThunk->u1.Function == OriginThunk->u1.Function) {
//				PIMAGE_IMPORT_BY_NAME ImportName = reinterpret_cast<PIMAGE_IMPORT_BY_NAME>(dwImageBuff + OriginThunk->u1.AddressOfData);
//				char* cFuncName = reinterpret_cast<char*>(ImportName->Name);
//				DWORD64 dwFunctionAddr = reinterpret_cast<DWORD64>(GetProcAddress(hDLL, cFuncName));
//				FirstThunk->u1.Function = dwFunctionAddr;
//			}
//
//			dwFirstOffset += sizeof(IMAGE_THUNK_DATA);
//			dwOriginOffset += sizeof(IMAGE_THUNK_DATA);
//		}
//	}
//
//	PIMAGE_SECTION_HEADER TestSection = reinterpret_cast<PIMAGE_SECTION_HEADER>(PE::dwGetSectionFromVA(dwImageBuff, 0x428798)); // unwind_code
//	DWORD64 dwTest = PE::dwGetAddrFromVA(dwImageBuff, 0x428798);
//	if (!TestSection && !dwTest)
//		return;
//
//	typedef void (*tStart) ();
//	tStart Start = reinterpret_cast<tStart>(dwImageBuff + NTHeaders->OptionalHeader.AddressOfEntryPoint);
//	Start();
//}