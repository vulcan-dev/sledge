#include "teardown/classes/game.h"
#include "teardown/classes/player.h"

#include "teardown/classes/entities.h"

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