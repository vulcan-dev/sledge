#include "net.h"
#include "globals.h"

#include "util/log.h"

#include <libloaderapi.h>

bool Net::Init() {
	HMODULE HostFXR = LoadLibraryA("C:/Program Files/dotnet/host/fxr/6.0.0/hostfxr.dll");

	if (!HostFXR) {
		LogError("unable to load hostfxr.");
		return false;
	}

	hostfxr_init = reinterpret_cast<hostfxr_initialize_for_runtime_config_fn>(GetProcAddress(HostFXR, "hostfxr_initialize_for_runtime_config"));
	hostfxr_get_delegate = reinterpret_cast<hostfxr_get_runtime_delegate_fn>(GetProcAddress(HostFXR, "hostfxr_get_runtime_delegate"));
	hostfxr_close = reinterpret_cast<hostfxr_close_fn>(GetProcAddress(HostFXR, "hostfxr_close"));

	LogVerbose("hostfxr_init: {}", reinterpret_cast<void*>(hostfxr_init));
	LogVerbose("hostfxr_get_delegate: {}", reinterpret_cast<void*>(hostfxr_get_delegate));
	LogVerbose("hostfxr_close: {}", reinterpret_cast<void*>(hostfxr_close));
	
	if (!hostfxr_init || !hostfxr_get_delegate || !hostfxr_close) {
		LogError("unable to get pointer to required hostfxr function");
		return false;
	}

	std::string sModulePath(g_ModulePath);
	std::wstring wsConfigPath(sModulePath.begin(), sModulePath.end());
	wsConfigPath.append(L"\\sledgelib.runtimeconfig.json");

	hostfxr_handle HostfxrContext = nullptr;

	if (hostfxr_init(wsConfigPath.c_str(), nullptr, &HostfxrContext) || HostfxrContext == nullptr) {
		LogError("hostfxr_init failed");
		return false;
	}

	if (hostfxr_get_delegate(HostfxrContext, hdt_load_assembly_and_get_function_pointer, reinterpret_cast<void**>(&hostfxr_load_assembly)) || hostfxr_load_assembly == nullptr) {
		LogError("hostfxr_get_delegate failed");
		return false;
	}

	hostfxr_close(HostfxrContext);
	
	LogVerbose("hostfxr_load_assembly: {}", reinterpret_cast<void*>(hostfxr_load_assembly));

	return true;
}