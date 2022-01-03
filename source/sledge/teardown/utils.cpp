#include "teardown/utils.h"

#include "teardown/classes/scene.h"
#include "teardown/classes/entities.h"

template<typename T>
T Teardown::Utils::GetEntityByIdx(unsigned int iIndex, EEntityType eType) {
	CEntity* Entity = g_Scene->m_Entities.get_at(iIndex);

	if (Entity == nullptr || Entity->m_Type != eType)
		return nullptr;

	return reinterpret_cast<T>(Entity);
}

template CEntity* Teardown::Utils::GetEntityByIdx(unsigned int, EEntityType);
template CBody* Teardown::Utils::GetEntityByIdx(unsigned int, EEntityType);
template CShape* Teardown::Utils::GetEntityByIdx(unsigned int, EEntityType);