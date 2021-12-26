#include "game/steam.h"
#include "util/pe.h"

#include <minwindef.h>
#include <Psapi.h>

#include <openssl/evp.h>
#include <openssl/err.h>

class CSteamHeader
{
public:
	unsigned int m_Key; //0x0000
	unsigned int m_Signature; //0x0004
	char pad_0008[24]; //0x0008
	uint64_t m_RealEntryPointOffset; //0x0020
	char pad_0028[16]; //0x0028
	uint32_t m_AppId; //0x0038
	char pad_003C[12]; //0x003C
	uint64_t m_CodeSectionVirtualAddr; //0x0048
	uint64_t m_CodeSectionSize; //0x0050
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

void* Steam::GetUnpackedExe(void* pOriginalExe, long lFileSize) {
	DWORD64 dwExe = reinterpret_cast<DWORD64>(pOriginalExe);

	/*
		get pe headers
	*/
	PIMAGE_DOS_HEADER DOSHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(dwExe);
	if (DOSHeader->e_magic != IMAGE_DOS_SIGNATURE)
		return 0;

	PIMAGE_NT_HEADERS NTHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(dwExe + DOSHeader->e_lfanew);
	if (NTHeaders->Signature != IMAGE_NT_SIGNATURE)
		return 0;

	DWORD64 dwEntryPoint = PE::dwGetAddrFromVA(dwExe, NTHeaders->OptionalHeader.AddressOfEntryPoint);

	if (dwEntryPoint == 0)
		return nullptr;

	/*
		get and verify steam header
	*/
	CSteamHeader* SteamHeader;
	SteamHeader = reinterpret_cast<CSteamHeader*>(dwEntryPoint - sizeof(CSteamHeader));
	HeaderXOR(reinterpret_cast<DWORD64>(SteamHeader), sizeof(CSteamHeader));

	if (SteamHeader->m_Signature != 0xC0DEC0DF)
		return nullptr;


	/*
		get encrypted code section
	*/
	PIMAGE_SECTION_HEADER CodeSection = reinterpret_cast<PIMAGE_SECTION_HEADER>(PE::dwGetSectionFromVA(dwExe, SteamHeader->m_CodeSectionVirtualAddr));

	int iCodeSectionSize = CodeSection->SizeOfRawData + sizeof(SteamHeader->m_OriginalCodeBytes);
	DWORD64 dwEncryptedBuff = reinterpret_cast<DWORD64>(malloc(iCodeSectionSize));
	if (dwEncryptedBuff == NULL)
		return nullptr;

	memcpy(reinterpret_cast<void*>(dwEncryptedBuff), SteamHeader->m_OriginalCodeBytes, sizeof(SteamHeader->m_OriginalCodeBytes));
	memcpy(reinterpret_cast<void*>(dwEncryptedBuff + sizeof(SteamHeader->m_OriginalCodeBytes)), reinterpret_cast<void*>(PE::dwGetAddrFromVA(dwExe, SteamHeader->m_CodeSectionVirtualAddr)), CodeSection->SizeOfRawData);

	EVP_CIPHER_CTX* EVPContext = EVP_CIPHER_CTX_new();
	EVP_CIPHER_CTX_set_padding(EVPContext, 0);

	/*
		restore the initialization vector
	*/
	if (EVP_DecryptInit_ex(EVPContext, EVP_aes_256_ecb(), 0, SteamHeader->m_AESKey, NULL) != 1)
		return nullptr;

	unsigned char cDecryptedIV[16];
	int iDecryptedIVLen;

	if (EVP_DecryptUpdate(EVPContext, cDecryptedIV, &iDecryptedIVLen, SteamHeader->m_AESIV, 16) != 1) {
		free(reinterpret_cast<void*>(dwEncryptedBuff));
		return nullptr;
	}

	if (EVP_DecryptFinal_ex(EVPContext, cDecryptedIV + iDecryptedIVLen, &iDecryptedIVLen) != 1) {
		free(reinterpret_cast<void*>(dwEncryptedBuff));
		return nullptr;
	}

	/*
		restore code section
	*/
	if (EVP_DecryptInit_ex(EVPContext, EVP_aes_256_cbc(), 0, SteamHeader->m_AESKey, cDecryptedIV) != 1)
		return nullptr;

	DWORD64 dwDecryptedCodeData = reinterpret_cast<DWORD64>(malloc(iCodeSectionSize));
	if (dwDecryptedCodeData == 0)
		return nullptr;

	int iCodeSectionSizeDecrypted;

	if (EVP_DecryptUpdate(EVPContext, reinterpret_cast<unsigned char*>(dwDecryptedCodeData), &iCodeSectionSizeDecrypted, reinterpret_cast<unsigned char*>(dwEncryptedBuff), iCodeSectionSize) != 1) {
		free(reinterpret_cast<void*>(dwEncryptedBuff));
		free(reinterpret_cast<void*>(dwDecryptedCodeData));
		return nullptr;
	}

	if (EVP_DecryptFinal_ex(EVPContext, reinterpret_cast<unsigned char*>(dwDecryptedCodeData + iCodeSectionSizeDecrypted), &iCodeSectionSizeDecrypted) != 1) {
		free(reinterpret_cast<void*>(dwEncryptedBuff));
		free(reinterpret_cast<void*>(dwDecryptedCodeData));
		return nullptr;
	}

	EVP_CIPHER_CTX_free(EVPContext);

	free(reinterpret_cast<void*>(dwEncryptedBuff));

	/*
		create buffer with unpacked code section and fixed entry point
	*/
	DWORD64 dwExeBuffer = reinterpret_cast<DWORD64>(malloc(lFileSize));
	if (dwExeBuffer == NULL)
		return nullptr;

	NTHeaders->OptionalHeader.AddressOfEntryPoint = static_cast<DWORD>(SteamHeader->m_RealEntryPointOffset);
	memcpy(reinterpret_cast<void*>(dwExeBuffer), pOriginalExe, lFileSize);
	memcpy(reinterpret_cast<void*>(dwExeBuffer + CodeSection->PointerToRawData), reinterpret_cast<void*>(dwDecryptedCodeData), iCodeSectionSize);
	free(reinterpret_cast<void*>(dwDecryptedCodeData));

	return reinterpret_cast<void*>(dwExeBuffer);
}