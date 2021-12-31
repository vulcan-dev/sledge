#include "teardown/classes/entities.h"
#include "teardown/utils.h"

#include "teardown/functions/memory.h"
#include "teardown/functions/constructors.h"

#define sledgelib_func extern "C" __declspec(dllexport)

sledgelib_func unsigned int CreateBody() {
	void* pBodyBuffer = Teardown::alloc(sizeof(CBody));
	if (pBodyBuffer == NULL)
		return 0;

	Teardown::Constructors::Body(pBodyBuffer, nullptr);
	CBody* Body = reinterpret_cast<CBody*>(pBodyBuffer);
	return Body->m_Id;
}

sledgelib_func Transform GetBodyTransform(unsigned int iBodyHandle) {
	CBody* Body = Teardown::Utils::GetEntityByIdx<CBody*>(iBodyHandle, EEntityType::Body);
	if (!Body) return Transform();
	return Transform(Body->m_Position, Body->m_Rotation);
}

sledgelib_func void SetBodyTransform(unsigned int iBodyHandle, Transform tNewTransform) {
	CBody* Body = Teardown::Utils::GetEntityByIdx<CBody*>(iBodyHandle, EEntityType::Body);
	if (!Body) return;
	Body->m_Position = tNewTransform.m_Position;
	Body->m_Rotation = tNewTransform.m_Rotation;
}

sledgelib_func float GetBodyMass(unsigned int iBodyHandle) {
	CBody* Body = Teardown::Utils::GetEntityByIdx<CBody*>(iBodyHandle, EEntityType::Body);
	if (!Body) return 0;
	return Body->m_Mass;
}

sledgelib_func bool GetBodyDynamic(unsigned int iBodyHandle) {
	CBody* Body = Teardown::Utils::GetEntityByIdx<CBody*>(iBodyHandle, EEntityType::Body);
	if (!Body) return false;
	return Body->m_Dynamic;
}

sledgelib_func void SetBodyDynamic(unsigned int iBodyHandle, bool bDynamic) {
	CBody* Body = Teardown::Utils::GetEntityByIdx<CBody*>(iBodyHandle, EEntityType::Body);
	if (!Body) return;
	Body->m_Dynamic = bDynamic;
}

sledgelib_func void SetBodyVelocity(unsigned int iBodyHandle, Vector3 vVelocity) {
	CBody* Body = Teardown::Utils::GetEntityByIdx<CBody*>(iBodyHandle, EEntityType::Body);
	if (!Body) return;
	Body->m_Velocity = vVelocity;
}

sledgelib_func Vector3 GetBodyVelocity(unsigned int iBodyHandle) {
	CBody* Body = Teardown::Utils::GetEntityByIdx<CBody*>(iBodyHandle, EEntityType::Body);
	if (!Body) return Vector3();
	return Body->m_Velocity;
}

sledgelib_func Vector3 GetBodyAngularVelocity(unsigned int iBodyHandle) {
	CBody* Body = Teardown::Utils::GetEntityByIdx<CBody*>(iBodyHandle, EEntityType::Body);
	if (!Body) return Vector3();
	return Body->m_AngularVelocity;
}

sledgelib_func void SetBodyAngularVelocity(unsigned int iBodyHandle, Vector3 vVelocity) {
	CBody* Body = Teardown::Utils::GetEntityByIdx<CBody*>(iBodyHandle, EEntityType::Body);
	if (!Body) return;
	Body->m_AngularVelocity = vVelocity;
}

sledgelib_func Vector3 GetBodyPosition(unsigned int iBodyHandle) {
	CBody* Body = Teardown::Utils::GetEntityByIdx<CBody*>(iBodyHandle, EEntityType::Body);
	if (!Body) return Vector3();
	return Body->m_Position;
}

sledgelib_func void SetBodyPosition(unsigned int iBodyHandle, Vector3 vValue) {
	CBody* Body = Teardown::Utils::GetEntityByIdx<CBody*>(iBodyHandle, EEntityType::Body);
	if (!Body) return;
	Body->m_Position = vValue;
}

sledgelib_func Vector4 GetBodyRotation(unsigned int iBodyHandle) {
	CBody* Body = Teardown::Utils::GetEntityByIdx<CBody*>(iBodyHandle, EEntityType::Body);
	if (!Body) return Vector4();
	return Body->m_Rotation;
}

sledgelib_func void SetBodyRotation(unsigned int iBodyHandle, Vector4 vValue) {
	CBody* Body = Teardown::Utils::GetEntityByIdx<CBody*>(iBodyHandle, EEntityType::Body);
	if (!Body) return;
	Body->m_Rotation = vValue;
}