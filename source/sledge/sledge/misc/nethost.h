#pragma once

// from: hostfxr.h
typedef void* hostfxr_handle;
enum hostfxr_delegate_type
{
	hdt_com_activation,
	hdt_load_in_memory_assembly,
	hdt_winrt_activation,
	hdt_com_register,
	hdt_com_unregister,
	hdt_load_assembly_and_get_function_pointer,
	hdt_get_function_pointer,
};

namespace NetHost {
	namespace {
		// from: hostfxr.h
		typedef int (*hostfxr_initialize_for_runtime_config_fn) (const wchar_t* runtime_config_path, const struct hostfxr_initialize_parameters* parameters, hostfxr_handle* host_context_handle);
		typedef int (*hostfxr_get_runtime_delegate_fn) (const hostfxr_handle host_context_handle, enum hostfxr_delegate_type type, void** delegate);
		typedef int (*hostfxr_close_fn) (const hostfxr_handle host_context_handle);
		typedef void* (*hostfxr_set_error_writer_fn) (void* error_writer);

		// from: coreclr_delegates
		typedef int (*load_assembly_and_get_function_pointer_fn) (const wchar_t* assembly_path, const wchar_t* type_name, const wchar_t* method_name, const wchar_t* delegate_type_name, void* reserved, void** delegate);
	}

	inline hostfxr_initialize_for_runtime_config_fn hostfxr_init;
	inline hostfxr_get_runtime_delegate_fn hostfxr_get_delegate;
	inline hostfxr_close_fn hostfxr_close;
	inline load_assembly_and_get_function_pointer_fn hostfxr_load_assembly;
	inline hostfxr_set_error_writer_fn hostfxr_set_error_writer;

	bool Init();
}