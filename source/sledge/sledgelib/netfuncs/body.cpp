#include "teardown/classes/game.h"
#include "teardown/classes/scene.h"
#include "teardown/classes/entities.h"

#define sledgelib_func extern "C" __declspec(dllexport)

sledgelib_func float GetBodyMass(unsigned int iBodyHandle) {
	CBody* Body = reinterpret_cast<CBody*>(g_Game->m_Scene->m_Entities.get_at(iBodyHandle));
	if (Body == nullptr || Body->m_Type != EEntityType::Body)
		return 0;

	return Body->m_Mass;
}

sledgelib_func bool GetBodyDynamic(unsigned int iBodyHandle) {
	CBody* Body = reinterpret_cast<CBody*>(g_Game->m_Scene->m_Entities.get_at(iBodyHandle));
	if (Body == nullptr || Body->m_Type != EEntityType::Body)
		return 0;

	return Body->m_Dynamic;
}

sledgelib_func void SetBodyDynamic(unsigned int iBodyHandle, bool bDynamic) {
	CBody* Body = reinterpret_cast<CBody*>(g_Game->m_Scene->m_Entities.get_at(iBodyHandle));
	if (Body == nullptr || Body->m_Type != EEntityType::Body)
		return;

	Body->m_Dynamic = bDynamic;
}

sledgelib_func void SetBodyVelocity(unsigned int iBodyHandle, Vector3 vVelocity) {
	CBody* Body = reinterpret_cast<CBody*>(g_Game->m_Scene->m_Entities.get_at(iBodyHandle));
	if (Body == nullptr || Body->m_Type != EEntityType::Body)
		return;

	Body->m_Velocity = vVelocity;
}

sledgelib_func Vector3 GetBodyVelocity(unsigned int iBodyHandle) {
	CBody* Body = reinterpret_cast<CBody*>(g_Game->m_Scene->m_Entities.get_at(iBodyHandle));
	if (Body == nullptr || Body->m_Type != EEntityType::Body)
		return Vector3(0, 0, 0);

	return Body->m_Velocity;
}

sledgelib_func Vector3 GetBodyAngularVelocity(unsigned int iBodyHandle) {
	CBody* Body = reinterpret_cast<CBody*>(g_Game->m_Scene->m_Entities.get_at(iBodyHandle));
	if (Body == nullptr || Body->m_Type != EEntityType::Body)
		return Vector3(0, 0, 0);

	return Body->m_AngularVelocity;
}

sledgelib_func void SetBodyAngularVelocity(unsigned int iBodyHandle, Vector3 vVelocity) {
	CBody* Body = reinterpret_cast<CBody*>(g_Game->m_Scene->m_Entities.get_at(iBodyHandle));
	if (Body == nullptr || Body->m_Type != EEntityType::Body)
		return;

	Body->m_AngularVelocity = vVelocity;
}

sledgelib_func Vector3 GetBodyPosition(unsigned int iBodyHandle) {
	CBody* Body = reinterpret_cast<CBody*>(g_Game->m_Scene->m_Entities.get_at(iBodyHandle));
	if (Body == nullptr || Body->m_Type != EEntityType::Body)
		return Vector3(0, 0, 0);

	return Body->m_Position;
}

sledgelib_func void SetBodyPosition(unsigned int iBodyHandle, Vector3 vValue) {
	CBody* Body = reinterpret_cast<CBody*>(g_Game->m_Scene->m_Entities.get_at(iBodyHandle));
	if (Body == nullptr || Body->m_Type != EEntityType::Body)
		return;

	Body->m_Position = vValue;
}

sledgelib_func Vector4 GetBodyRotation(unsigned int iBodyHandle) {
	CBody* Body = reinterpret_cast<CBody*>(g_Game->m_Scene->m_Entities.get_at(iBodyHandle));
	if (Body == nullptr || Body->m_Type != EEntityType::Body)
		return Vector4(0, 0, 0, 0);

	return Body->m_Rotation;
}

sledgelib_func void SetBodyRotation(unsigned int iBodyHandle, Vector4 vValue) {
	CBody* Body = reinterpret_cast<CBody*>(g_Game->m_Scene->m_Entities.get_at(iBodyHandle));
	if (Body == nullptr || Body->m_Type != EEntityType::Body)
		return;

	Body->m_Rotation = vValue;
}