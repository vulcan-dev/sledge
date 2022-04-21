#include "sledgelib.h"

#include "globals.h"

#include "net/nethost.h"
#include "util/log.h"

#include <excpt.h>

#pragma warning(push, 0)
#include <fmt/format.h>
#pragma warning(pop)

typedef SLUnmanagedInterface (*tGetInterface) ();
tGetInterface GetInterface;

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
		get pointer to GetInterface function
	*/
	unsigned int iReturn = NetHost::hostfxr_load_assembly(wsLibPath.c_str(), L"Loader, sledgelib", L"GetInterface", L"Loader+dGetInterface, sledgelib", nullptr, (void**)&GetInterface);
	if (iReturn != 0 || GetInterface == nullptr) {
		LogError("hostfxr_load_assembly failed while loading sledgelib ({0:#x})", iReturn);
		return false;
	}
	LogVerbose("sledgelib Loader.GetInterface: {}", reinterpret_cast<void*>(GetInterface));

	/*
		get the C# interface
	*/
	SLUnmanagedInterface NetInterface = GetInterface();
	SledgeLib::Interface = new SLUnmanagedInterface();
	memcpy(SledgeLib::Interface, &NetInterface, sizeof(SLUnmanagedInterface));

	/*
		get the callback interface
	*/
	SledgeLib::CallbackInterface = new SLCallbackInterface();
	SLCallbackInterface NetCallbackInterface = SledgeLib::Interface->GetCallbackInterface();
	memcpy(SledgeLib::CallbackInterface, &NetCallbackInterface, sizeof(SLCallbackInterface));

	return true;
}

bool SledgeLib::Init() {
	/*
		invoke Init, catch potential exceptions
	*/
	__try {
		if (!SledgeLib::Interface->Init()) {
			LogError("SledgeLib Interface->Init() returned false");
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

void SledgeLib::Shutdown() {
	/*
		invoke Init, catch potential exceptions
	*/
	__try {
		if (!SledgeLib::Interface->Shutdown())
			LogError("SledgeLib Interface->Shutdown() returned false");
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		long lExceptionCode = _exception_code();
		LogError("SledgeLib threw exception {0:#x} on Shutdown()", lExceptionCode);
	}
}