#pragma once

#include "teardown/types/small_string.h"
#include "teardown/classes/entity.h"

namespace Teardown {
	typedef void (*tSetTag) (Entity* pEntity, small_string* ssName, small_string* ssValue);
	inline tSetTag SetTag;

	typedef bool (*tHasTag) (Entity* pEntity, small_string* ssName);
	inline tHasTag HasTag;

	typedef void (*tRemoveTag) (Entity* pEntity, small_string* ssName);
	inline tRemoveTag RemoveTag;

	typedef small_string* (*tGetTag) (Entity* pEntity, small_string* ssReturn, small_string* ssName);
	inline tGetTag GetTag;
}