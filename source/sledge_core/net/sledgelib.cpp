#include "sledgelib.h"

#include "globals.h"

#include "net/nethost.h"
#include "util/log.h"

#include <excpt.h>

#include <fmt/format.h>

typedef bool (*tSledgeLibInit) ();
tSledgeLibInit SledgeLibInit;

bool RunInit() {
	/*
		invoke Init, catch potential exceptions
	*/
	__try {
		if (!SledgeLibInit()) {
			LogError("sledgelib Loader.Init returned false");
			return false;
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		long lExceptionCode = _exception_code();
		LogError("SledgeLib threw exception {0:#x} on Init()", lExceptionCode);
		return false;
	}
	return true;
}

bool SledgeLib::Load() {
	/*
		get expected path of sledgelib.dll and check if it exists
	*/
	std::string sLibPath = fmt::format("{}\\sledgelib.dll", g_ModulePath);
	if (!std::filesystem::exists(sLibPath)) {
		LogError("Missing file: bin\\sledgelib.dll");
		return false;
	}
	std::wstring wsLibPath(sLibPath.begin(), sLibPath.end());

	/*
		get pointer to init function
	*/
	unsigned int iReturn = NetHost::hostfxr_load_assembly(wsLibPath.c_str(), L"Loader, sledgelib", L"Init", L"Loader+dInit, sledgelib", nullptr, (void**)&SledgeLibInit);
	if (iReturn != 0 || SledgeLibInit == nullptr) {
		LogError("hostfxr_load_assembly failed while loading sledgelib ({0:#x})", iReturn);
		return false;
	}
	LogVerbose("sledgelib Loader.Init: {}", reinterpret_cast<void*>(SledgeLibInit));

	/*
		return result of executing the init function
	*/
	return RunInit();
}