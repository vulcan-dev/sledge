#include "teardown/functions/memory.h"
#include "teardown/functions/constructors.h"
#include "teardown/functions/entity.h"
#include "teardown/classes/body.h"
#include "teardown/classes/scene.h"
#include "teardown/utils.h"

#define sledgelib_func extern "C" __declspec(dllexport)


sledgelib_func unsigned int Body_Create() {
	void* pBuffer = Teardown::malloc(sizeof(Body));
	if (pBuffer == NULL)
		return 0;

	Teardown::BodyConstructor(pBuffer, nullptr);
	Body* pBody = reinterpret_cast<Body*>(pBuffer);
	return pBody->m_Id;
}
sledgelib_func void Body_Destroy(unsigned int iHandle) {
	Body* pBody = Teardown::Utils::GetEntityByIdx<Body*>(iHandle, Type_Body);
	if (!pBody) return;
	pBody->Destroy(true);
}

sledgelib_func Transform Body_GetTransform(unsigned int iHandle) {
	Body* pBody = Teardown::Utils::GetEntityByIdx<Body*>(iHandle, Type_Body);
	if (!pBody) return Transform();
	return pBody->m_Transform;
}
sledgelib_func void Body_SetTransform(unsigned int iHandle, Transform tNew) {
	Body* pBody = Teardown::Utils::GetEntityByIdx<Body*>(iHandle, Type_Body);
	if (!pBody) return;
	pBody->m_Transform = tNew;
}

sledgelib_func Vector3 Body_GetPosition(unsigned int iHandle) {
	Body* pBody = Teardown::Utils::GetEntityByIdx<Body*>(iHandle, Type_Body);
	if (!pBody) return Vector3();
	return pBody->m_Transform.m_Position;
}
sledgelib_func void Body_SetPosition(unsigned int iHandle, Vector3 vNew) {
	Body* pBody = Teardown::Utils::GetEntityByIdx<Body*>(iHandle, Type_Body);
	if (!pBody) return;
	pBody->m_Transform.m_Position = vNew;
}

sledgelib_func Vector3 Body_GetVelocity(unsigned int iHandle) {
	Body* pBody = Teardown::Utils::GetEntityByIdx<Body*>(iHandle, Type_Body);
	if (!pBody) return Vector3();
	return pBody->m_Velocity;
}
sledgelib_func void Body_SetVelocity(unsigned int iHandle, Vector3 vNew) {
	Body* pBody = Teardown::Utils::GetEntityByIdx<Body*>(iHandle, Type_Body);
	if (!pBody) return;
	pBody->m_Velocity = vNew;
}

sledgelib_func Vector4 Body_GetAngularVelocity(unsigned int iHandle) {
	Body* pBody = Teardown::Utils::GetEntityByIdx<Body*>(iHandle, Type_Body);
	if (!pBody) return Vector4();
	return pBody->m_AngularVelocity;
}
sledgelib_func void Body_SetAngularVelocity(unsigned int iHandle, Vector4 vNew) {
	Body* pBody = Teardown::Utils::GetEntityByIdx<Body*>(iHandle, Type_Body);
	if (!pBody) return;
	pBody->m_AngularVelocity = vNew;
}

sledgelib_func bool Body_GetDynamic(unsigned int iHandle) {
	Body* pBody = Teardown::Utils::GetEntityByIdx<Body*>(iHandle, Type_Body);
	if (!pBody) return false;
	return pBody->m_Dynamic;
}
sledgelib_func void Body_SetDynamic(unsigned int iHandle, bool bNew) {
	Body* pBody = Teardown::Utils::GetEntityByIdx<Body*>(iHandle, Type_Body);
	if (!pBody) return;
	pBody->m_Dynamic = bNew;
}

sledgelib_func bool Body_GetActive(unsigned int iHandle) {
	Body* pBody = Teardown::Utils::GetEntityByIdx<Body*>(iHandle, Type_Body);
	if (!pBody) return false;
	return (pBody->m_Dynamic && pBody->m_ActiveTicksLeft != 0);
}
sledgelib_func void Body_SetActive(unsigned int iHandle, bool bActive) {
	Body* pBody = Teardown::Utils::GetEntityByIdx<Body*>(iHandle, Type_Body);
	if (!pBody) return;

	if (!pBody->m_Dynamic)
		return;

	if (bActive)
		pBody->m_ActiveTicksLeft = 60;
	else
		pBody->m_ActiveTicksLeft = 0;

	Teardown::UpdateBodyActive(g_Scene->m_Physics, pBody);
}

sledgelib_func float Body_GetMass(unsigned int iHandle) {
	Body* pBody = Teardown::Utils::GetEntityByIdx<Body*>(iHandle, Type_Body);
	if (!pBody) return 0.f;
	return pBody->m_Mass;
}