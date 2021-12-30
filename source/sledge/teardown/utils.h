#pragma once

enum EEntityType : unsigned char;

namespace Teardown {
	namespace Utils {
		template <typename T>
		T GetEntityByIdx(unsigned int iIndex, EEntityType);
	}
}