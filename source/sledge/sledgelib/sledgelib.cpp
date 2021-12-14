#include "util/log.h"

#include "sledgelib.h"
#include "net.h"
#include "globals.h"

#include <filesystem>
#include <string>

typedef void	(*tSledgeInit) ();
tSledgeInit SledgeInit = nullptr;

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

	LogVerbose("SledgeLib.Loader.Init: {}", reinterpret_cast<void*>(SledgeInit));
	SledgeInit();

	LogInfo("sledgelib loaded");

	return true;
}