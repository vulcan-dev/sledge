#include "teardown/functions/constructors.h"
#include "teardown/functions/entity.h"
#include "teardown/classes/scene.h"
#include "teardown/utils.h"

#define sledgelib_func extern "C" __declspec(dllexport)

sledgelib_func unsigned int Body_Create() {
	void* pBuffer = Teardown::alloc(sizeof(CBody));
	if (pBuffer == NULL)
		return 0;

	Teardown::Constructors::Body(pBuffer, nullptr);
	CBody* Body = reinterpret_cast<CBody*>(pBuffer);
	return Body->m_Id;
}

sledgelib_func void Body_Destroy(unsigned int iHandle) {
	CBody* Body = Teardown::Utils::GetEntityByIdx<CBody*>(iHandle, EEntityType::Body);
	if (!Body) return;
	Body->Destroy(true);
}

sledgelib_func Transform Body_GetTransform(unsigned int iHandle) {
	CBody* Body = Teardown::Utils::GetEntityByIdx<CBody*>(iHandle, EEntityType::Body);
	if (!Body) return Transform();
	return Body->m_Transform;
}

sledgelib_func void Body_SetTransform(unsigned int iHandle, Transform tTransform) {
	CBody* Body = Teardown::Utils::GetEntityByIdx<CBody*>(iHandle, EEntityType::Body);
	if (!Body) return;
	Body->m_Transform = tTransform;
}

sledgelib_func Vector3 Body_GetPosition(unsigned int iHandle) {
	CBody* Body = Teardown::Utils::GetEntityByIdx<CBody*>(iHandle, EEntityType::Body);
	if (!Body) return Vector3();
	return Body->m_Transform.m_Position;
}

sledgelib_func void Body_SetPosition(unsigned int iHandle, Vector3 vValue) {
	CBody* Body = Teardown::Utils::GetEntityByIdx<CBody*>(iHandle, EEntityType::Body);
	if (!Body) return;
	Body->m_Transform.m_Position = vValue;
}

sledgelib_func Quaternion Body_GetRotation(unsigned int iHandle) {
	CBody* Body = Teardown::Utils::GetEntityByIdx<CBody*>(iHandle, EEntityType::Body);
	if (!Body) return Quaternion();
	return Body->m_Transform.m_Rotation;
}

sledgelib_func void Body_SetRotation(unsigned int iHandle, Quaternion qValue) {
	CBody* Body = Teardown::Utils::GetEntityByIdx<CBody*>(iHandle, EEntityType::Body);
	if (!Body) return;
	Body->m_Transform.m_Rotation = qValue;
}

sledgelib_func Vector3 Body_GetVelocity(unsigned int iHandle) {
	CBody* Body = Teardown::Utils::GetEntityByIdx<CBody*>(iHandle, EEntityType::Body);
	if (!Body) return Vector3();
	return Body->m_Velocity;
}

sledgelib_func void Body_SetVelocity(unsigned int iHandle, Vector3 vVelocity) {
	CBody* Body = Teardown::Utils::GetEntityByIdx<CBody*>(iHandle, EEntityType::Body);
	if (!Body) return;
	Body->m_Velocity = vVelocity;
}

sledgelib_func Vector3 Body_GetAngularVelocity(unsigned int iHandle) {
	CBody* Body = Teardown::Utils::GetEntityByIdx<CBody*>(iHandle, EEntityType::Body);
	if (!Body) return Vector3();
	return Body->m_AngularVelocity;
}

sledgelib_func void Body_SetAngularVelocity(unsigned int iHandle, Vector3 vVelocity) {
	CBody* Body = Teardown::Utils::GetEntityByIdx<CBody*>(iHandle, EEntityType::Body);
	if (!Body) return;
	Body->m_AngularVelocity = vVelocity;
}

sledgelib_func bool Body_GetDynamic(unsigned int iHandle) {
	CBody* Body = Teardown::Utils::GetEntityByIdx<CBody*>(iHandle, EEntityType::Body);
	if (!Body) return false;
	return Body->m_Dynamic;
}

sledgelib_func void Body_SetDynamic(unsigned int iHandle, bool bDynamic) {
	CBody* Body = Teardown::Utils::GetEntityByIdx<CBody*>(iHandle, EEntityType::Body);
	if (!Body) return;
	Body->m_Dynamic = bDynamic;
}

sledgelib_func bool Body_GetActive(unsigned int iHandle) {
	CBody* Body = Teardown::Utils::GetEntityByIdx<CBody*>(iHandle, EEntityType::Body);
	if (!Body) return false;
	return (Body->m_Dynamic && Body->m_ActiveTimeLeft != 0);
}

sledgelib_func void Body_SetActive(unsigned int iHandle, bool bActive) {
	CBody* Body = Teardown::Utils::GetEntityByIdx<CBody*>(iHandle, EEntityType::Body);
	if (!Body) return;

	if (!Body->m_Dynamic)
		return;

	if (bActive) {
		Body->m_ActiveTimeLeft = 0;
		Teardown::Entity::UpdateBodyDynamic(g_Scene->m_Physics, Body);
		Teardown::Entity::UnknownBodyFunction(g_Scene->m_Physics, Body, true);
	}
	else {
		Body->m_ActiveTimeLeft = 60;
		Teardown::Entity::UpdateBodyDynamic(g_Scene->m_Physics, Body);
	};
}

sledgelib_func float GetBodyMass(unsigned int iHandle) {
	CBody* Body = Teardown::Utils::GetEntityByIdx<CBody*>(iHandle, EEntityType::Body);
	if (!Body) return 0;
	return Body->m_Mass;
}