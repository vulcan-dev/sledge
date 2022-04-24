#pragma once

#include "teardown/types/small_string.h"
#include "teardown/classes/entity.h"

namespace Teardown {
	inline void (*SetTag) (Entity* pEntity, small_string* ssName, small_string* ssValue);
	inline bool (*tHasTag) (Entity* pEntity, small_string* ssName);
	inline void (*tRemoveTag) (Entity* pEntity, small_string* ssName);
	inline small_string* (*tGetTag) (Entity* pEntity, small_string* ssReturn, small_string* ssName);
}