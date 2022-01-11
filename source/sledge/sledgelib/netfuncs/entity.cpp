#include "teardown/utils.h"

#define sledgelib_func extern "C" __declspec(dllexport)

sledgelib_func unsigned int Entity_GetSibling(unsigned int iHandle) {
	CEntity* Entity = Teardown::Utils::GetEntityByIdx(iHandle);
	if (!Entity|| !Entity->m_Sibling) return 0 ;
	return Entity->m_Sibling->m_Id;
}

sledgelib_func unsigned int Entity_GetChild(unsigned int iHandle) {
	CEntity* Entity = Teardown::Utils::GetEntityByIdx(iHandle);
	if (!Entity || !Entity->m_Child) return 0;
	return Entity->m_Child->m_Id;
}

sledgelib_func unsigned int Entity_GetParent(unsigned int iHandle) {
	CEntity* Entity = Teardown::Utils::GetEntityByIdx(iHandle);
	if (!Entity || !Entity->m_Parent) return 0;
	return Entity->m_Parent->m_Id;
}