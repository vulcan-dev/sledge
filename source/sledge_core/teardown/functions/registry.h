#pragma once

#include "teardown/types/small_string.h"

namespace Teardown {
	inline void (*SetBool) (void* pRegistry, small_string* ssKeyName, bool bKeyValue);
	inline void (*SetInt) (void* pRegistry, small_string* ssKeyName, int iValue);
	inline void (*SetFloat) (void* pRegistry, small_string* ssKeyName, float fValue);
	inline void (*SetString) (void* pRegistry, small_string* ssKeyName, small_string* ssKeyValue);

	inline bool (*GetBool) (void* pRegistry, small_string* ssKeyName);
	inline int (*GetInt) (void* pRegistry, small_string* ssKeyName);
	inline float (*GetFloat) (void* pRegistry, small_string* ssKeyName);
	inline small_string* (*GetString) (void* pRegistry, small_string* ssReturn, small_string* ssKeyName);
}