#include "sledgelib.h"

#include "util/log.h"

#include "sledge/misc/nethost.h"

#include "globals.h"

bool SledgeLib::Load() {
	std::string sModulePath(g_ModulePath);

	std::wstring wsLibPath(sModulePath.begin(), sModulePath.end());
	wsLibPath.append(L"\\mods\\sledgelib.dll");

	if (!std::filesystem::exists(wsLibPath)) {
		LogError("missing file: mods\\sledgelib.dll");
		return false;
	}
	
	typedef bool (*tSledgeLibInit) ();
	tSledgeLibInit SledgeLibInit;
	unsigned int iReturn = NetHost::hostfxr_load_assembly(wsLibPath.c_str(), L"SledgeLoader, sledgelib", L"Init", L"SledgeLoader+dInit, sledgelib", nullptr, (void**)&SledgeLibInit);
	if (iReturn != 0 || SledgeLibInit == nullptr) {
		LogError("hostfxr_load_assembly failed while loading sledgelib ({0:#x})", iReturn);
		return false;
	}
	
	LogVerbose("sledgelib Loader.Init: {}", reinterpret_cast<void*>(SledgeLibInit));
	
	if (!SledgeLibInit()) {
		LogError("sledgelib Loader.Init failed");
		return false;
	}

	return true;
}