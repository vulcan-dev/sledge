#pragma once

#include "util/memory.h"

namespace Signatures {
	inline CSignature Free("\xE8\x00\x00\x00\x00\x4C\x39\x26", "x????xxx");
	inline CSignature Alloc("\xE8\x00\x00\x00\x00\xEB\x1F\x48\x85\xDB", "x????xxxxx");

	inline CSignature Game("\xE8\x00\x00\x00\x00\x48\x8B\xF8\xEB\x02\x33\xFF\x48\x8D\x4D\xA7", "x????xxxxxxxxxxx");
}