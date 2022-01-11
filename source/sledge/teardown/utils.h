#pragma once

#include "teardown/classes/entities.h"

namespace Teardown {
	namespace Utils {
		template <typename T>
		T GetEntityByIdx(unsigned int iIndex, EEntityType eType);
		CEntity* GetEntityByIdx(unsigned int iIndex);
	}
}