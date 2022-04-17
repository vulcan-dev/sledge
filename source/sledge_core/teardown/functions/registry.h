#pragma once

#include "teardown/types/small_string.h"

typedef void (*tSetBool) (void* pRegistry, small_string* ssKeyName, bool bKeyValue);
typedef void (*tSetInt) (void* pRegistry, small_string* ssKeyName, int iValue);
typedef void (*tSetFloat) (void* pRegistry, small_string* ssKeyName, float fValue);
typedef void (*tSetString) (void* pRegistry, small_string* ssKeyName, small_string* ssKeyValue);

typedef bool (*tGetBool) (void* pRegistry, small_string* ssKeyName);
typedef int (*tGetInt) (void* pRegistry, small_string* ssKeyName);
typedef float (*tGetFloat) (void* pRegistry, small_string* ssKeyName);
typedef void (*tGetString) (void* pRegistry, small_string* ssReturn, small_string* ssKeyName);

namespace Teardown {
	inline tSetBool SetBool;
	inline tSetInt SetInt;
	inline tSetFloat SetFloat;
	inline tSetString SetString;

	inline tGetBool GetBool;
	inline tGetInt GetInt;
	inline tGetFloat GetFloat;
	inline tGetString GetString;
}