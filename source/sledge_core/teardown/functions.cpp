#include "teardown/functions.h"
#include "teardown/offsets.h"

#include "teardown/functions/memory.h"
#include "teardown/functions/tags.h"
#include "teardown/functions/registry.h"

#include "util/log.h"

#include "globals.h"

struct SFunctionInfo {
	unsigned __int64 m_Offset;
	void* m_Function;
	const char* m_Name;
};

static const SFunctionInfo aFunctions[] = {
	/*
		memory functions
	*/
	{ g_Offsets["free"], &Teardown::free, "free" },
	{ g_Offsets["malloc"], &Teardown::malloc, "malloc" },
	{ g_Offsets["calloc"], &Teardown::calloc, "calloc" },
	{ g_Offsets["realloc"], &Teardown::realloc, "realloc" },
	{ g_Offsets["memmove"], &Teardown::memmove, "memmove" },

	/*
		tag functions
	*/
	{ g_Offsets["SetTag"], &Teardown::SetTag, "SetTag" },
	{ g_Offsets["HasTag"], &Teardown::HasTag, "HasTag" },
	{ g_Offsets["RemoveTag"], &Teardown::RemoveTag, "RemoveTag" },
	{ g_Offsets["GetTag"], &Teardown::RemoveTag, "GetTag" },

	/*
		registry functions
	*/
	{ g_Offsets["SetBool"], &Teardown::SetBool, "SetBool" },
	{ g_Offsets["SetInt"], &Teardown::SetInt, "SetInt" },
	{ g_Offsets["SetFloat"], &Teardown::SetFloat, "SetFloat" },
	{ g_Offsets["SetString"], &Teardown::SetString, "SetString" },

	{ g_Offsets["GetBool"], &Teardown::GetBool, "GetBool" },
	{ g_Offsets["GetInt"], &Teardown::GetInt, "GetInt" },
	{ g_Offsets["GetFloat"], &Teardown::GetFloat, "GetFloat" },
	{ g_Offsets["GetString"], &Teardown::GetString, "GetString" }
};

void Teardown::GetFunctionAddresses() {
	for (const SFunctionInfo& Func : aFunctions) {
		unsigned __int64 iFunctionAddress = g_BaseAddress + Func.m_Offset;
		memcpy(Func.m_Function, &iFunctionAddress, sizeof(void*));
		LogVerbose("{} - {}", Func.m_Name, reinterpret_cast<void*>(iFunctionAddress));
	}
}