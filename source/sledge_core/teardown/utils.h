#pragma once

#include "teardown/classes/entity.h"

namespace Teardown {
	namespace Utils {
		template <typename T>
		T GetEntityByIdx(unsigned int iIndex, EEntityType eType);
		Entity* GetEntityByIdx(unsigned int iIndex);
	}
}