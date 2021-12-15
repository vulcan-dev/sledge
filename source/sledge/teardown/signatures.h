#pragma once

#include "util/memory.h"

namespace Signatures {
	CSignature Free("\xE8\x00\x00\x00\x00\x4C\x39\x26", "x????xxx");
	CSignature Alloc("\xE8\x00\x00\x00\x00\xEB\x1F\x48\x85\xDB", "x????xxxxx");
}