#pragma once

class CSignature {
public:
	CSignature(const char* cPattern, const char* cMask, bool bDirectRef = true) {
		m_Pattern = cPattern; m_Mask = cMask; m_DirectRef = bDirectRef;
	}

	const char* m_Pattern;
	const char* m_Mask;
	bool m_DirectRef;
};

namespace Memory {
	unsigned __int64 dwReadPtr(unsigned __int64 dwAddress, unsigned int iOffset = 0);
	unsigned __int64 dwFindPattern(CSignature Sig);
}