#include "teardown/utils.h"

#include "teardown/classes/scene.h"
#include "teardown/classes/entity.h"
#include "teardown/classes/body.h"
#include "teardown/classes/shape.h"
#include "teardown/classes/joint.h"

template<typename T>
T Teardown::Utils::GetEntityByIdx(unsigned int iIndex, EEntityType eType) {
	Entity* Entity = g_Scene->m_Entities.get_at(iIndex);

	if (Entity == nullptr || Entity->m_Type != eType)
		return nullptr;

	return reinterpret_cast<T>(Entity);
}

Entity* Teardown::Utils::GetEntityByIdx(unsigned int iIndex) {
	Entity* pEntity = g_Scene->m_Entities.get_at(iIndex);
	return pEntity;
}

template Entity* Teardown::Utils::GetEntityByIdx(unsigned int, EEntityType);
template Body* Teardown::Utils::GetEntityByIdx(unsigned int, EEntityType);
template Shape* Teardown::Utils::GetEntityByIdx(unsigned int, EEntityType);
template Joint* Teardown::Utils::GetEntityByIdx(unsigned int, EEntityType);