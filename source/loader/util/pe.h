#pragma once

namespace PE {
	unsigned __int64 dwGetSectionFromVA(unsigned __int64 dwPEAddress, unsigned __int64 dwVAAddress);
	unsigned __int64 dwGetAddrFromVA(unsigned __int64 dwPEAddress, unsigned __int64 dwVAAddress);
}