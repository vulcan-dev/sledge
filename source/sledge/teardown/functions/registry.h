#pragma once

typedef void (*tSetBool) (void* pRegistry, class small_string* ssKeyName, bool bKeyValue);
typedef void (*tSetInt) (void* pRegistry, class small_string* ssKeyName, int iValue);
typedef void (*tSetFloat) (void* pRegistry, class small_string* ssKeyName, float fValue);
typedef void (*tSetString) (void* pRegistry, class small_string* ssKeyName, class small_string* ssKeyValue);

typedef bool (*tGetBool) (void* pRegistry, class small_string* ssKeyName);
typedef int (*tGetInt) (void* pRegistry, class small_string* ssKeyName);
typedef float (*tGetFloat) (void* pRegistry, class small_string* ssKeyName);
typedef class small_string* (*tGetString) (void* pRegistry, class small_string* ssKeyName);

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