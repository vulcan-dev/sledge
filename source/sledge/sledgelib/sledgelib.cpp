#include "util/log.h"

#include "sledgelib.h"
#include "misc/nethost.h"
#include "globals.h"

#include <string>
#include <filesystem>

typedef bool (*tSledgeLibInit) (void*, char*);
tSledgeLibInit SledgeLibInit;

void NetWriteLog(char* cMsg) { Log(ELogType::Net, cMsg); }
void NetWriteError(char* cMsg) { Log(ELogType::NetError, cMsg); }

struct SSledgeLibInternal {
	void* _WriteLog = NetWriteLog;
	void* _WriteError = NetWriteError;
};

SSledgeLibInternal* GetInternalFunctions() {
	return new SSledgeLibInternal();;
}

bool SledgeLib::Load() {
	std::string sModulePath(g_ModulePath);

	std::wstring wsLibPath(sModulePath.begin(), sModulePath.end());
	wsLibPath.append(L"\\mods\\sledgelib.dll");

	if (!std::filesystem::exists(wsLibPath)) {
		LogError("missing file: mods\\sledgelib.dll");
		return false;
	}
	
	unsigned int iReturn = NetHost::hostfxr_load_assembly(wsLibPath.c_str(), L"SledgeLoader, sledgelib", L"Init", L"SledgeLoader+InitDelegate, sledgelib", nullptr, (void**)&SledgeLibInit);
	if (iReturn != 0 || SledgeLibInit == nullptr) {
		LogError("hostfxr_load_assembly failed while loading sledgelib ({0:#x})", iReturn);
		return false;
	}
	
	LogVerbose("sledgelib Loader.Init: {}", reinterpret_cast<void*>(SledgeLibInit));
	
	if (!SledgeLibInit(GetInternalFunctions, g_ModulePath)) {
		LogError("sledgelib failed to initialize");
		return false;
	}
	
	return true;
}