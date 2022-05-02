#include "teardown/classes/player.h"
#include "teardown/classes/game.h"
#include "teardown/classes/vehicle.h"
#include "teardown/classes/body.h"
#include "teardown/classes/shape.h"
#include "teardown/classes/scene.h"

#define sledgelib_func extern "C" __declspec(dllexport)

sledgelib_func float Player_GetHealth() { return g_Player->m_Health; }
sledgelib_func void Player_SetHealth(float fValue) { g_Player->m_Health = fValue; }

sledgelib_func Vector3 Player_GetPosition() { return g_Player->m_Transform.m_Position; }
sledgelib_func void Player_SetPosition(Vector3 vValue) { g_Player->m_Transform.m_Position = vValue; }

sledgelib_func Vector3 Player_GetVelocity() { return g_Player->m_Velocity; }
sledgelib_func void Player_SetVelocity(Vector3 vValue) { g_Player->m_Velocity = vValue; }

sledgelib_func Transform Player_GetCameraTransform() { return g_Player->m_CameraTransform2; }

sledgelib_func Vector3 Player_GetCameraPosition() { return g_Player->m_CameraTransform2.m_Position; }
sledgelib_func void Player_SetCameraPosition(Vector3 vValue) { g_Player->m_CameraTransform2.m_Position = vValue; }

sledgelib_func Vector2 Player_GetCameraAngles() { return g_Player->m_CameraAngle; }
sledgelib_func void Player_SetCameraAngles(Vector2 vValue) { g_Player->m_CameraAngle = vValue; }

sledgelib_func Vector2 Player_GetMovementKeys() { return g_Player->m_MovementKeys; }

sledgelib_func bool Player_GetCrouching() { return g_Player->m_Crouching; }

sledgelib_func bool Player_GetInAir() { return g_Player->m_AirTime > 0 ? true : false; }

sledgelib_func void Player_ReleaseGrab() {
	if (g_Player->m_GrabBody)
		g_Player->m_GrabBody->Destroy(true);

	g_Player->m_GrabbedBody = nullptr;
	g_Player->m_GrabbedShape = nullptr;
}

sledgelib_func unsigned int Player_GetGrabbedShape() {
	if (!g_Player->m_GrabbedShape) return 0;
	return g_Player->m_GrabbedShape->m_Id;
}

sledgelib_func unsigned int Player_GetGrabbedBody() {
	if (!g_Player->m_GrabbedBody) return 0;
	return g_Player->m_GrabbedBody->m_Id;
}

sledgelib_func unsigned int Player_GetToolBody() {
	if (!g_Player->m_ToolBody) return 0;
	return g_Player->m_ToolBody->m_Id;
}

sledgelib_func unsigned int Player_GetVehicleBody() {
	if (!g_Scene->m_CurrentVehicle)
		return 0;
	return g_Scene->m_CurrentVehicle->m_Body->m_Id;
}

/*
	TO-DO:
		value "18" is hard-coded, it's what Teardown does as well (or at least during compile?)
		however, this will cause issues if a new tool were to be added
*/
sledgelib_func int Player_GetLastToolIdx() { return 18 + g_Player->m_RegisteredTools.size(); }
sledgelib_func void Player_RegisterTool(char* cId, char* cName, char* cFile, unsigned int iGroup) {
	if (iGroup < 1 || iGroup > 6)
		iGroup = 6;

	RegisteredTool* Tool = reinterpret_cast<RegisteredTool*>(Teardown::malloc(sizeof(RegisteredTool)));
	memset(Tool, 0, sizeof(RegisteredTool));
	
	small_string* ssId = new small_string(cId);
	small_string* ssName = new small_string(cName);
	small_string* ssFile = new small_string(cFile);

	memcpy(&Tool->m_Id, ssId, sizeof(small_string));
	memcpy(&Tool->m_Name, ssName, sizeof(small_string));
	memcpy(&Tool->m_Model, ssFile, sizeof(small_string));

	Tool->m_Group = iGroup;

	g_Player->m_RegisteredTools.push_back(*Tool);
}

sledgelib_func void Player_Respawn() { g_Game->m_RespawnPlayer = true; }

sledgelib_func bool Player_M1Down() { return g_Game->m_Player->m_M1Down; }
sledgelib_func bool Player_M2Down() { return g_Game->m_Player->m_M2Down; }

sledgelib_func Transform Player_GetSpawnTransform() { return g_Scene->m_SpawnTransform; }