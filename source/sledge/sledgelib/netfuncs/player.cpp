#include "teardown/classes/game.h"
#include "teardown/classes/scene.h"
#include "teardown/classes/player.h"
#include "teardown/classes/entities.h"

#include "teardown/functions/registry.h"
#include "teardown/functions/memory.h"

#define sledgelib_func extern "C" __declspec(dllexport)

sledgelib_func void SetPlayerHealth(float fValue) { g_Game->m_Player->m_Health = fValue; }
sledgelib_func float GetPlayerHealth() { return g_Game->m_Player->m_Health; }

sledgelib_func void SetPlayerPosition(Vector3 vPosition) { g_Game->m_Player->m_Position = vPosition; }
sledgelib_func Vector3 GetPlayerPosition() { return g_Game->m_Player->m_Position; }

sledgelib_func void SetPlayerVelocity(Vector3 vVelocity) { g_Game->m_Player->m_Velocity = vVelocity; }
sledgelib_func Vector3 GetPlayerVelocity() { return g_Game->m_Player->m_Velocity; }

sledgelib_func void SetPlayerCameraTransform(Transform tCameraTransform) { g_Game->m_Player->m_CameraTransform2 = tCameraTransform; }
sledgelib_func Transform GetPlayerCameraTransform() { return g_Game->m_Player->m_CameraTransform2; }

sledgelib_func Vector2 GetPlayerMovementInput() { return g_Game->m_Player->m_MovementKeys; }
sledgelib_func Vector2 GetPlayerMouseInput() { return g_Game->m_Player->m_MouseInput; }

sledgelib_func void Respawn() { g_Game->m_RespawnPlayer = true; }

sledgelib_func unsigned int GetGrabbedBody() {
	if (!g_Game->m_Player->m_GrabbedBody) return 0;
	return g_Game->m_Player->m_GrabbedBody->m_Id;
}

sledgelib_func unsigned int GetGrabbedShape() {
	if (!g_Game->m_Player->m_GrabbedShape) return 0;
	return g_Game->m_Player->m_GrabbedShape->m_Id;
}

sledgelib_func void ReleaseGrab() {
	if (g_Game->m_Player->m_GrabBody)
		g_Game->m_Player->m_GrabBody->Destroy(true);

	g_Game->m_Player->m_GrabbedBody = nullptr;
	g_Game->m_Player->m_GrabbedShape = nullptr;
}

sledgelib_func int _GetLastToolIdx() {
	return 17 + g_Game->m_Player->m_RegisteredTools.size();
}

sledgelib_func void _RegisterTool(char* cId, char* cName, char* cFile, unsigned int iGroup) {
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

sledgelib_func unsigned int GetPlayerVehicleBody() {
	if (!g_Game->m_Scene->m_CurrentVehicle)
		return 0;
	return g_Game->m_Scene->m_CurrentVehicle->m_Body->m_Id;
}