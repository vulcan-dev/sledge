#include "util/log.h"

#include "sledgelib.h"
#include "sledgelib/netfuncs.h"

#include "sledge/misc/nethost.h"
#include "sledge/misc/binds.h"

#include "globals.h"

#include <string>
#include <filesystem>

struct SSledgeLibInternal {
	void* _WriteLog = SledgeLib::NetFuncs::WriteLog;
	void* _RegisterInputReader = SledgeLib::NetFuncs::RegisterInputReader;
	void* _RegisterCallback = SledgeLib::NetFuncs::RegisterCallback;
	void* _GetPlayer = SledgeLib::NetFuncs::GetPlayer;
};

typedef bool (*tSledgeLibInit) (SSledgeLibInternal*, char*);

bool SledgeLib::Load() {
	std::string sModulePath(g_ModulePath);
	
	std::wstring wsLibPath(sModulePath.begin(), sModulePath.end());
	wsLibPath.append(L"\\mods\\sledgelib.dll");

	if (!std::filesystem::exists(wsLibPath)) {
		LogError("missing file: mods\\sledgelib.dll");
		return false;
	}
	
	tSledgeLibInit SledgeLibInit;

	unsigned int iReturn = NetHost::hostfxr_load_assembly(wsLibPath.c_str(), L"SledgeLoader, sledgelib", L"Init", L"SledgeLoader+dInit, sledgelib", nullptr, (void**)&SledgeLibInit);
	if (iReturn != 0 || SledgeLibInit == nullptr) {
		LogError("hostfxr_load_assembly failed while loading sledgelib ({0:#x})", iReturn);
		return false;
	}
	
	LogVerbose("sledgelib Loader.Init: {}", reinterpret_cast<void*>(SledgeLibInit));
	
	SSledgeLibInternal* pInternal = new SSledgeLibInternal();
	
	if (!SledgeLibInit(pInternal, g_ModulePath)) {
		LogError("sledgelib failed to initialize");
		return false;
	}
	
	return true;
}