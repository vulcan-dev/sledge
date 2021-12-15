#include "util/log.h"

#include "sledgelib.h"
#include "net.h"
#include "globals.h"

#include <filesystem>
#include <string>

typedef void (*tSledgeInit) (void* pGetInteralPtr);
tSledgeInit SledgeInit = nullptr;

void fWriteLog(char* cMsg) {
	LogInfo(cMsg);
}

struct CSledgeAPI_Internal {
	void* WriteLog = fWriteLog;
};

CSledgeAPI_Internal* g_Internal;



CSledgeAPI_Internal* GetInternal() {
	LogInfo("GetInternal");
	return g_Internal;
}

bool SledgeLib::Load() {
	std::string sModulePath(g_ModulePath);
	std::wstring LibPath(sModulePath.begin(), sModulePath.end());
	LibPath.append(L"\\sledgelib.dll");

	const wchar_t* LibInfo = L"SledgeLib.Loader, SledgeLib";

	int iReturn = Net::hostfxr_load_assembly(LibPath.c_str(), LibInfo, L"Init", L"SledgeLib.InitDelegate, SledgeLib", nullptr, (void**)&SledgeInit);
	if (iReturn != 0 || SledgeInit == nullptr) {
		LogError("hostfxr_load_assembly failed while loading sledgelib");
		return false;
	}

	g_Internal = new CSledgeAPI_Internal();

	LogVerbose("SledgeLib.Loader.Init: {}", reinterpret_cast<void*>(SledgeInit));
	SledgeInit(GetInternal);

	LogInfo("sledgelib loaded");

	return true;
}