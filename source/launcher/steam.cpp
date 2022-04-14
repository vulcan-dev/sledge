#include "steam.h"

#include "util/wininternals.h"
#include "util/pe.h"

#include <cstdlib>

#include <minwindef.h>
#include <processthreadsapi.h>
#include <libloaderapi.h>
#include <memoryapi.h>

#include <openssl/evp.h>
#include <openssl/err.h>

typedef NTSTATUS(*tNtQueryInformationProcess) (HANDLE hProc, PROCESSINFOCLASS ProcInfoClass, PVOID ProcInfo, ULONG ProcInfoLen, PULONG RetLen);
tNtQueryInformationProcess NtQueryInformationProcess;

typedef NTSTATUS(*tNtUnmapViewOfSection) (HANDLE hProc, PVOID pBaseAddr);
tNtUnmapViewOfSection NtUnmapViewOfSection;

class CSteamHeader
{
public:
	unsigned int m_Key; //0x0000
	unsigned int m_Signature; //0x0004
	char pad_0008[24]; //0x0008
	unsigned long long m_RealEntryPointOffset; //0x0020
	char pad_0028[16]; //0x0028
	unsigned int m_AppId; //0x0038
	char pad_003C[12]; //0x003C
	unsigned long long m_CodeSectionVirtualAddr; //0x0048
	unsigned long long m_CodeSectionSize; //0x0050
	unsigned char m_AESKey[32]; //0x0058
	unsigned char m_AESIV[16]; //0x0078
	char m_OriginalCodeBytes[16]; //0x0088
	char pad_0098[88]; //0x0098
}; //Size: 0x00F0

void HeaderXOR(DWORD64 pData, size_t lSize) {
	unsigned int iKey;
	iKey = *reinterpret_cast<int*>(pData);

	for (int i = 4; i < lSize; i += 4) {
		unsigned int iValue = *reinterpret_cast<int*>(pData + i);
		unsigned int iData = iValue ^ iKey;

		memcpy(reinterpret_cast<void*>(pData + i), &iData, sizeof(unsigned int));

		iKey = iValue;
	}
}

void Steam::Defuse(void* hProc, void* hThread, void* pExeBuffer) {
	DWORD64 dwExeBuffer = reinterpret_cast<DWORD64>(pExeBuffer);

	/*
		get ntdll functions
	*/
	HMODULE hNTDLL = LoadLibraryA("ntdll.dll");
	if (!hNTDLL)
		return;

	NtQueryInformationProcess = reinterpret_cast<tNtQueryInformationProcess>(GetProcAddress(hNTDLL, "NtQueryInformationProcess"));
	NtUnmapViewOfSection = reinterpret_cast<tNtUnmapViewOfSection>(GetProcAddress(hNTDLL, "NtUnmapViewOfSection"));

	/*
		Get process's basic information
	*/
	PROCESS_BASIC_INFORMATION ProcInfo;
	ZeroMemory(&ProcInfo, sizeof(PROCESS_BASIC_INFORMATION));

	ULONG RetLength;
	NtQueryInformationProcess(hProc, PROCESSINFOCLASS::ProcessBasicInformation, &ProcInfo, sizeof(PROCESS_BASIC_INFORMATION), &RetLength);

	/*
		allocate and read PEB, store ImageBaseAddress
	*/
	PEB* pPEB = reinterpret_cast<PEB*>(malloc(sizeof(PEB)));
	if (!pPEB)
		return;

	bool bSuccess = ReadProcessMemory(hProc, ProcInfo.PebBaseAddress, pPEB, sizeof(PEB), 0);
	if (!bSuccess)
		return;

	/*
		get PE Headers
	*/
	PIMAGE_DOS_HEADER DOSHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(dwExeBuffer);
	if (DOSHeader->e_magic != IMAGE_DOS_SIGNATURE)
		return;

	PIMAGE_NT_HEADERS NTHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(dwExeBuffer + DOSHeader->e_lfanew);
	if (NTHeaders->Signature != IMAGE_NT_SIGNATURE)
		return;

	DWORD64 dwEntryPoint = PE::dwGetAddrFromVA(dwExeBuffer, NTHeaders->OptionalHeader.AddressOfEntryPoint);
	if (dwEntryPoint == 0)
		return;

	/*
		read steam header
	*/
	CSteamHeader* SteamHeader = reinterpret_cast<CSteamHeader*>(dwEntryPoint - sizeof(CSteamHeader));
	HeaderXOR(reinterpret_cast<DWORD64>(SteamHeader), sizeof(CSteamHeader));

	if (SteamHeader->m_Signature != 0xC0DEC0DF)
		return;

	/*
		get encrypted code section and allocate buffer for decryption
	*/
	PIMAGE_SECTION_HEADER CodeSection = reinterpret_cast<PIMAGE_SECTION_HEADER>(PE::dwGetSectionFromVA(dwExeBuffer, SteamHeader->m_CodeSectionVirtualAddr));
	int iCodeSectionSize = CodeSection->SizeOfRawData + sizeof(SteamHeader->m_OriginalCodeBytes);
	DWORD64 dwEncryptedCodeBuff = reinterpret_cast<DWORD64>(malloc(iCodeSectionSize));

	if (!dwEncryptedCodeBuff || iCodeSectionSize < sizeof(SteamHeader->m_OriginalCodeBytes))
		return;

	/*
		write first 16 bytes steamstub "stole" back onto decrypted buffer
		then write everything else onto it
	*/
	memcpy(reinterpret_cast<void*>(dwEncryptedCodeBuff), SteamHeader->m_OriginalCodeBytes, sizeof(SteamHeader->m_OriginalCodeBytes));
	memcpy(reinterpret_cast<void*>(dwEncryptedCodeBuff + sizeof(SteamHeader->m_OriginalCodeBytes)), reinterpret_cast<void*>(PE::dwGetAddrFromVA(dwExeBuffer, SteamHeader->m_CodeSectionVirtualAddr)), CodeSection->SizeOfRawData);

	/*
		set up EVP for decryption
	*/
	EVP_CIPHER_CTX* EVPContext = EVP_CIPHER_CTX_new();
	EVP_CIPHER_CTX_set_padding(EVPContext, 0);

	/*
		decrypt iv
	*/
	unsigned char cDecryptedIV[16];
	int iDecryptedIVLen;

	if (EVP_DecryptInit_ex(EVPContext, EVP_aes_256_ecb(), 0, SteamHeader->m_AESKey, NULL) != 1)
		return;

	if (EVP_DecryptUpdate(EVPContext, cDecryptedIV, &iDecryptedIVLen, SteamHeader->m_AESIV, 16) != 1)
		return;
	if (EVP_DecryptFinal_ex(EVPContext, cDecryptedIV + iDecryptedIVLen, &iDecryptedIVLen) != 1)
		return;

	/*
		decrypt code section
	*/
	if (EVP_DecryptInit_ex(EVPContext, EVP_aes_256_cbc(), 0, SteamHeader->m_AESKey, cDecryptedIV) != 1)
		return;

	DWORD64 dwDecryptedCodeBuff = reinterpret_cast<DWORD64>(malloc(iCodeSectionSize));
	if (dwDecryptedCodeBuff == 0)
		return;

	int iCodeSectionDecryptedSize;

	if (EVP_DecryptUpdate(EVPContext, reinterpret_cast<unsigned char*>(dwDecryptedCodeBuff), &iCodeSectionDecryptedSize, reinterpret_cast<unsigned char*>(dwEncryptedCodeBuff), iCodeSectionSize) != 1)
		return;

	if (EVP_DecryptFinal_ex(EVPContext, reinterpret_cast<unsigned char*>(dwDecryptedCodeBuff + iCodeSectionDecryptedSize), &iCodeSectionDecryptedSize) != 1)
		return;

	EVP_CIPHER_CTX_free(EVPContext);

	/*
		update code section on running process
	*/
	DWORD64 dwCodeAddr = reinterpret_cast<DWORD64>(pPEB->ImageBaseAddress) + CodeSection->VirtualAddress;
	bSuccess = WriteProcessMemory(hProc, reinterpret_cast<LPVOID>(dwCodeAddr), reinterpret_cast<LPCVOID>(dwDecryptedCodeBuff), CodeSection->SizeOfRawData, 0);
	if (!bSuccess)
		return;

	/*
		Update thread ctx
	*/
	CONTEXT Context;
	ZeroMemory(&Context, sizeof(CONTEXT));
	Context.ContextFlags = CONTEXT_INTEGER;
	GetThreadContext(hThread, &Context);

	/*
		write jmp out of SteamStub's entry point
	*/
	DWORD64 dwRealEntryPoint = reinterpret_cast<DWORD64>(pPEB->ImageBaseAddress) + SteamHeader->m_RealEntryPointOffset;

	if (dwRealEntryPoint < dwCodeAddr || dwRealEntryPoint > dwCodeAddr + CodeSection->SizeOfRawData)
		return;

	unsigned char cPatch[] = {
		0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // mov rax, addr
		0xFF, 0xE0 // jmp rax
	};

	memcpy(cPatch + 2, &dwRealEntryPoint, sizeof(dwRealEntryPoint));

	bSuccess = WriteProcessMemory(hProc, reinterpret_cast<LPVOID>(Context.Rcx), cPatch, sizeof(cPatch), 0);
	if (!bSuccess)
		return;

	/*
		free buffers
	*/
	free(reinterpret_cast<void*>(dwEncryptedCodeBuff));
	free(reinterpret_cast<void*>(dwDecryptedCodeBuff));
	free(reinterpret_cast<void*>(pPEB));
}