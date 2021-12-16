#include "nethost.h"
#include "globals.h"

#include "util/log.h"

#include <libloaderapi.h>

#include <string>
#include <filesystem>

void NetErrorWriter(const wchar_t* wcErrorMsg) {
	size_t lMsgLen = wcslen(wcErrorMsg);
	char* cErrorMsg = new char[lMsgLen];

	wcstombs(cErrorMsg, wcErrorMsg, lMsgLen);
	cErrorMsg[lMsgLen] = '\0';

	LogNetError("{0}", cErrorMsg);
}

bool NetHost::Init() {
	HMODULE HostFXR = LoadLibraryA("C:/Program Files/dotnet/host/fxr/6.0.0/hostfxr.dll");

	if (!HostFXR) {
		LogError("unable to load hostfxr.");
		return false;
	}

	hostfxr_init = reinterpret_cast<hostfxr_initialize_for_runtime_config_fn>(GetProcAddress(HostFXR, "hostfxr_initialize_for_runtime_config"));
	hostfxr_get_delegate = reinterpret_cast<hostfxr_get_runtime_delegate_fn>(GetProcAddress(HostFXR, "hostfxr_get_runtime_delegate"));
	hostfxr_close = reinterpret_cast<hostfxr_close_fn>(GetProcAddress(HostFXR, "hostfxr_close"));
	hostfxr_set_error_writer = reinterpret_cast<hostfxr_set_error_writer_fn>(GetProcAddress(HostFXR, "hostfxr_set_error_writer"));

	LogVerbose("hostfxr_init: {}", reinterpret_cast<void*>(hostfxr_init));
	LogVerbose("hostfxr_get_delegate: {}", reinterpret_cast<void*>(hostfxr_get_delegate));
	LogVerbose("hostfxr_close: {}", reinterpret_cast<void*>(hostfxr_close));
	LogVerbose("hostfxr_set_error_writer: {}", reinterpret_cast<void*>(hostfxr_set_error_writer));
	
	if (!hostfxr_init || !hostfxr_get_delegate || !hostfxr_close || !hostfxr_set_error_writer) {
		LogError("unable to get pointer to required hostfxr function");
		return false;
	}

	std::string sModulePath(g_ModulePath);
	std::wstring wsConfigPath(sModulePath.begin(), sModulePath.end());
	wsConfigPath.append(L"\\sledgelib.runtimeconfig.json");

	if (!std::filesystem::exists(wsConfigPath)) {
		LogError("missing file: sledgelib.runtimeconfig.json");
		return false;
	}

	hostfxr_handle HostfxrContext = nullptr;

	if (hostfxr_init(wsConfigPath.c_str(), nullptr, &HostfxrContext) || HostfxrContext == nullptr) {
		LogError("hostfxr_init failed");
		return false;
	}

	if (hostfxr_get_delegate(HostfxrContext, hdt_load_assembly_and_get_function_pointer, reinterpret_cast<void**>(&hostfxr_load_assembly)) || hostfxr_load_assembly == nullptr) {
		LogError("hostfxr_get_delegate failed");
		return false;
	}

	hostfxr_set_error_writer(NetErrorWriter);

	hostfxr_close(HostfxrContext);
	
	LogVerbose("hostfxr_load_assembly: {}", reinterpret_cast<void*>(hostfxr_load_assembly));

	return true;
}