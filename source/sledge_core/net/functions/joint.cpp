#include "teardown/functions/memory.h"
#include "teardown/functions/constructors.h"
#include "teardown/functions/entity.h"
#include "teardown/classes/shape.h"
#include "teardown/classes/Joint.h"
#include "teardown/utils.h"

#define sledgelib_func extern "C" __declspec(dllexport)

sledgelib_func unsigned int Joint_Create() {
	void* pBuffer = Teardown::malloc(sizeof(Joint));
	if (pBuffer == NULL)
		return 0;
	Teardown::JointConstructor(pBuffer, nullptr);
	return reinterpret_cast<Joint*>(pBuffer)->m_Id;

}
sledgelib_func void Joint_Detach(unsigned int iJoint, unsigned int iChildShape) {
	Joint* pJoint = Teardown::Utils::GetEntityByIdx<Joint*>(iJoint, Type_Joint);
	if (!pJoint) return;

	Shape* pChild = Teardown::Utils::GetEntityByIdx<Shape*>(iChildShape, Type_Shape);
	if (!pChild) return;

	Teardown::DetachJoint(pJoint, pChild);
}

sledgelib_func void Joint_Attach(unsigned int iJoint, unsigned int iParentShape, unsigned int iChildShape, Vector3 vChildJointPos, Vector3 vParentJointPos) {
	Joint* pJoint = Teardown::Utils::GetEntityByIdx<Joint*>(iJoint, Type_Joint);
	if (!pJoint) return;

	Shape* pParent = Teardown::Utils::GetEntityByIdx<Shape*>(iParentShape, Type_Shape);
	if (!pParent) return;

	Shape* pChild = Teardown::Utils::GetEntityByIdx<Shape*>(iChildShape, Type_Shape);
	if (!pChild) return;

	Teardown::AttachJoint(pJoint, pChild, pParent, &vChildJointPos, &vParentJointPos);

	pJoint->m_UnkBool = true;
	pJoint->m_UnkFloat = 3000.0f;
	pJoint->m_UnkFloat2 = 0.8f;
}

sledgelib_func bool Joint_GetCollide(unsigned int iJoint) {
	Joint* pJoint = Teardown::Utils::GetEntityByIdx<Joint*>(iJoint, Type_Joint);
	if (!pJoint) return false;
	return pJoint->m_Collide;
}
sledgelib_func void Joint_SetCollide(unsigned int iJoint, bool bValue) {
	Joint* pJoint = Teardown::Utils::GetEntityByIdx<Joint*>(iJoint, Type_Joint);
	if (!pJoint) return;
	pJoint->m_Collide = bValue;
}

sledgelib_func unsigned int Joint_GetType(unsigned int iJoint) {
	Joint* pJoint = Teardown::Utils::GetEntityByIdx<Joint*>(iJoint, Type_Joint);
	if (!pJoint) return 0;
	return pJoint->m_Type;
}
sledgelib_func void Joint_SetType(unsigned int iJoint, unsigned int iValue) {
	Joint* pJoint = Teardown::Utils::GetEntityByIdx<Joint*>(iJoint, Type_Joint);
	if (!pJoint) return;
	pJoint->m_Type = iValue;
}

sledgelib_func float Joint_GetSize(unsigned int iJoint) {
	Joint* pJoint = Teardown::Utils::GetEntityByIdx<Joint*>(iJoint, Type_Joint);
	if (!pJoint) return 0.f;
	return pJoint->m_Size;
}
sledgelib_func void Joint_SetSize(unsigned int iJoint, float fValue) {
	Joint* pJoint = Teardown::Utils::GetEntityByIdx<Joint*>(iJoint, Type_Joint);
	if (!pJoint) return;
	pJoint->m_Size = fValue;
}

sledgelib_func float Joint_GetRotStrength(unsigned int iJoint) {
	Joint* pJoint = Teardown::Utils::GetEntityByIdx<Joint*>(iJoint, Type_Joint);
	if (!pJoint) return 0.f;
	return pJoint->m_RotStrength;
}
sledgelib_func void Joint_SetRotStrength(unsigned int iJoint, float fValue) {
	Joint* pJoint = Teardown::Utils::GetEntityByIdx<Joint*>(iJoint, Type_Joint);
	if (!pJoint) return;
	pJoint->m_RotStrength = fValue;
}

sledgelib_func float Joint_GetRotSpring(unsigned int iJoint) {
	Joint* pJoint = Teardown::Utils::GetEntityByIdx<Joint*>(iJoint, Type_Joint);
	if (!pJoint) return 0.f;
	return pJoint->m_RotSpring;
}
sledgelib_func void Joint_SetRotSpring(unsigned int iJoint, float fValue) {
	Joint* pJoint = Teardown::Utils::GetEntityByIdx<Joint*>(iJoint, Type_Joint);
	if (!pJoint) return;
	pJoint->m_RotSpring = fValue;
}