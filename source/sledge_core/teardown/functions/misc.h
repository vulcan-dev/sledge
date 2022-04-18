#pragma once

#include "teardown/types/small_string.h"

namespace Teardown {
	typedef void (*tDebugPrint) (void* m_Debug, small_string* ssMessage);
	inline tDebugPrint DebugPrint;
}