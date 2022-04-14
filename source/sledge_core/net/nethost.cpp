#include "nethost.h"
#include "globals.h"

#include "util/log.h"

#include <fmt/format.h>

#include <filesystem>
#include <libloaderapi.h>

bool NetHost::Init() {
	/*
		check if hostfxr is installed
	*/
	if (!std::filesystem::exists("C:/Program Files/dotnet/host/fxr/")) {
		LogError("hostfxr not found");
		return false;
	}

	/*
		get latest installed hostfxr version
	*/
	int iLatestVersion = 0;
	std::string sLatestVersion = "";
	int iCurrentVersion = 0;
	for (std::filesystem::directory_entry const& Directory : std::filesystem::directory_iterator("C:/Program Files/dotnet/host/fxr/")) {
		std::string Path = Directory.path().filename().string();
		Path.erase(std::remove(Path.begin(), Path.end(), '.'), Path.end());

		iCurrentVersion = std::stoi(Path);
		if (iCurrentVersion > iLatestVersion) {
			iLatestVersion = iCurrentVersion;
			sLatestVersion = Directory.path().filename().string();
		}
	}

	/*
		check if hostfxr is outdated
	*/
	if (iCurrentVersion < 600) {
		LogError("Incompatible hostfxr instalation ({}), .NET Desktop Runtime 6.0.0+ required.", sLatestVersion);
		return false;
	}

	/*
		find and load  hostfxr
	*/
	std::string sLibPath = fmt::format("C:/Program Files/dotnet/host/fxr/{}/hostfxr.dll", sLatestVersion);
	LogVerbose("Loading hostfxr version: {}", sLatestVersion);

	HMODULE hHostFXR = LoadLibraryA(sLibPath.c_str());
	if (!hHostFXR) {
		LogError("LoadLibraryA returned null when attempting to load hostfxr.dll");
		return false;
	}

	/*
		get hostfxr functions
	*/
	hostfxr_init = reinterpret_cast<hostfxr_initialize_for_runtime_config_fn>(GetProcAddress(hHostFXR, "hostfxr_initialize_for_runtime_config"));
	hostfxr_get_delegate = reinterpret_cast<hostfxr_get_runtime_delegate_fn>(GetProcAddress(hHostFXR, "hostfxr_get_runtime_delegate"));
	hostfxr_close = reinterpret_cast<hostfxr_close_fn>(GetProcAddress(hHostFXR, "hostfxr_close"));
	/*hostfxr_set_error_writer = reinterpret_cast<hostfxr_set_error_writer_fn>(GetProcAddress(hHostFXR, "hostfxr_set_error_writer"));*/

	LogVerbose("hostfxr_init: {}", reinterpret_cast<void*>(hostfxr_init));
	LogVerbose("hostfxr_get_delegate: {}", reinterpret_cast<void*>(hostfxr_get_delegate));
	LogVerbose("hostfxr_close: {}", reinterpret_cast<void*>(hostfxr_close));
	/*LogVerbose("hostfxr_set_error_writer: {}", reinterpret_cast<void*>(hostfxr_set_error_writer));*/

	if (!hostfxr_init || !hostfxr_get_delegate || !hostfxr_close /*|| !hostfxr_set_error_writer*/) {
		LogError("Unable to obtain required hostfxr functions");
		return false;
	}

	/*
		check if runtimeconfig exists
	*/
	std::string sModulePath(g_ModulePath);
	std::wstring wsConfigPath(sModulePath.begin(), sModulePath.end());
	wsConfigPath.append(L"\\sledge.runtimeconfig.json");

	if (!std::filesystem::exists(wsConfigPath)) {
		LogError("Missing file: sledge.runtimeconfig.json");
		return false;
	}

	/*
		initialize HostFXR
	*/
	hostfxr_handle HostFXRContext = nullptr;

	unsigned int iReturn = hostfxr_init(wsConfigPath.c_str(), nullptr, &HostFXRContext);
	if (iReturn != 0 || HostFXRContext == nullptr) {
		LogError("hostfxr_init failed: ({0:#x})", iReturn);
		return false;
	}

	/*
		get hostfxr_load_assembly and close context
	*/
	if (hostfxr_get_delegate(HostFXRContext, hdt_load_assembly_and_get_function_pointer, reinterpret_cast<void**>(&hostfxr_load_assembly)) || hostfxr_load_assembly == nullptr) {
		LogError("hostfxr_get_delegate failed");
		return false;
	}
	LogVerbose("hostfxr_load_assembly: {}", reinterpret_cast<void*>(hostfxr_load_assembly));
	hostfxr_close(HostFXRContext);

	return true;
}