#include "teardown/classes/game.h"
#include "teardown/classes/scene.h"
#include "teardown/classes/player.h"
#include "teardown/classes/entities.h"

#define sledgelib_func extern "C" __declspec(dllexport)


sledgelib_func float Player_GetHealth() { return g_Game->m_Player->m_Health; }
sledgelib_func void Player_SetHealth(float fValue) { g_Game->m_Player->m_Health = fValue; }

sledgelib_func Vector3 Player_GetPosition() { return g_Game->m_Player->m_Position; }
sledgelib_func void Player_SetPosition(Vector3 vValue) { g_Game->m_Player->m_Position = vValue; }

sledgelib_func Vector3 Player_GetVelocity() { return g_Game->m_Player->m_Velocity; }
sledgelib_func void Player_SetVelocity(Vector3 vValue) { g_Game->m_Player->m_Velocity = vValue; }

sledgelib_func Transform Player_GetCameraTransform() { return g_Game->m_Player->m_CameraTransform2; }
sledgelib_func void Player_SetCameraTransform(Transform tValue) { g_Game->m_Player->m_CameraTransform2 = tValue; }

sledgelib_func Vector2 Player_GetCameraAngles() { return g_Game->m_Player->m_CameraAngles; }
sledgelib_func void Player_SetCameraAngles(Vector2 vValue) { g_Game->m_Player->m_CameraAngles = vValue; }

sledgelib_func Transform Player_GetToolTransform() { return g_Game->m_Player->m_RegisteredToolTransform; }
sledgelib_func void Player_SetToolTransform(Transform tValue) { g_Game->m_Player->m_RegisteredToolTransform = tValue; }

sledgelib_func Vector2 Player_GetMovementInput() { return g_Game->m_Player->m_MovementKeys; }

sledgelib_func void Player_ReleaseGrab() {
	if (g_Game->m_Player->m_GrabBody)
		g_Game->m_Player->m_GrabBody->Destroy(true);

	g_Game->m_Player->m_GrabbedBody = nullptr;
	g_Game->m_Player->m_GrabbedShape = nullptr;
}

sledgelib_func unsigned int Player_GetGrabbedShape() {
	if (!g_Game->m_Player->m_GrabbedShape) return 0;
	return g_Game->m_Player->m_GrabbedShape->m_Id;
}

sledgelib_func unsigned int Player_GetGrabbedBody() {
	if (!g_Game->m_Player->m_GrabbedBody) return 0;
	return g_Game->m_Player->m_GrabbedBody->m_Id;
}

sledgelib_func unsigned int Player_GetToolBody() {
	if (!g_Game->m_Player->m_ToolBody) return 0;
	return g_Game->m_Player->m_ToolBody->m_Id;
}

sledgelib_func unsigned int Player_GetVehicleBody() {
	if (!g_Game->m_Scene->m_CurrentVehicle)
		return 0;
	return g_Scene->m_CurrentVehicle->m_Body->m_Id;
}

sledgelib_func int Player_GetLastToolIdx() { return 17 + g_Game->m_Player->m_RegisteredTools.size(); } // hard-coded number, but it's what teardown uses too from what i could see

sledgelib_func void Player_RegisterTool(char* cId, char* cName, char* cFile, unsigned int iGroup) {
	if (iGroup < 1 || iGroup > 6)
		iGroup = 6;
	
	CRegisteredTool Tool = *reinterpret_cast<CRegisteredTool*>(Teardown::alloc(sizeof(CRegisteredTool)));
	memset(&Tool, 0, sizeof(CRegisteredTool));
		
	Tool.m_Id = small_string(cId);;
	Tool.m_Name = small_string(cName);
	Tool.m_File = small_string(cFile);
	Tool.m_Group = iGroup;
	
	g_Game->m_Player->m_RegisteredTools.push_back(Tool);
}

sledgelib_func void Player_Respawn() { g_Game->m_RespawnPlayer = true; }

sledgelib_func bool Player_M1Down() { return g_Game->m_Player->m_M1Down; }
sledgelib_func bool Player_M2Down() { return g_Game->m_Player->m_M2Down; }