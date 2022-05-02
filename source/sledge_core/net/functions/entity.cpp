#include "teardown/utils.h"

#define sledgelib_func extern "C" __declspec(dllexport)

sledgelib_func unsigned int Entity_GetSibling(unsigned int iHandle) {
	Entity* pEntity = Teardown::Utils::GetEntityByIdx(iHandle);
	if (!pEntity || !pEntity->m_Sibling) return 0;
	return pEntity->m_Sibling->m_Id;
}

sledgelib_func unsigned int Entity_GetChild(unsigned int iHandle) {
	Entity* pEntity = Teardown::Utils::GetEntityByIdx(iHandle);
	if (!pEntity || !pEntity->m_Child) return 0;
	return pEntity->m_Child->m_Id;
}

sledgelib_func unsigned int Entity_GetParent(unsigned int iHandle) {
	Entity* pEntity = Teardown::Utils::GetEntityByIdx(iHandle);
	if (!pEntity || !pEntity->m_Parent) return 0;
	return pEntity->m_Parent->m_Id;
}

sledgelib_func void Entity_Destroy(unsigned int iHandle) {
	Entity* pEntity = Teardown::Utils::GetEntityByIdx(iHandle);
	if (!pEntity) return;
	pEntity->Destroy(true);
}

sledgelib_func void Entity_SetParent(unsigned int iHandle, unsigned int iParentHandle) {
	Entity* pChild = Teardown::Utils::GetEntityByIdx(iHandle);
	Entity* pParent = Teardown::Utils::GetEntityByIdx(iParentHandle);
	if (!pChild || !pParent) return;
	pChild->SetParent(pParent);
}