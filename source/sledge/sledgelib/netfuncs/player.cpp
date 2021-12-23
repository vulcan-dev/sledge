#include "sledgelib/netfuncs.h"

#include "teardown/classes/game.h"
#include "teardown/classes/player.h"

#include "util/log.h"

void SetHealth(float fHealth) { g_Game->m_Player->m_Health = fHealth; }
float GetHealth() { return g_Game->m_Player->m_Health; }

void SetPosition(Vector3 vPosition) { g_Game->m_Player->m_Position = vPosition; }
Vector3 GetPosition() { return g_Game->m_Player->m_Position; }

void SetVelocity(Vector3 vVelocity) { g_Game->m_Player->m_Velocity = vVelocity; }
Vector3 GetVelocity() { return g_Game->m_Player->m_Velocity; }

void SetCameraTransform(Transform tCameraTransform) {g_Game->m_Player->m_CameraTransform2 = tCameraTransform; }
Transform GetCameraTransform() { return g_Game->m_Player->m_CameraTransform; }

Vector2 GetMovementKeys() { return g_Game->m_Player->m_MovementKeys; }
Vector2 GetMouseInput() { return g_Game->m_Player->m_MouseInput; }

struct SPlayerWrapper {
	void* _SetHealth = SetHealth;
	void* _GetHealth = GetHealth;
	void* _SetPosition = SetPosition;
	void* _GetPosition = GetPosition;
	void* _SetVelocity = SetVelocity;
	void* _GetVelocity = GetVelocity;
	void* _SetCameraTransform = SetCameraTransform;
	void* _GetCameraTransform = GetCameraTransform;
	void* _GetMovementKeys = GetMovementKeys;
	void* _GetMouseInput = GetMouseInput;
};

void* SledgeLib::NetFuncs::GetPlayer() {
	SPlayerWrapper* PlayerWrapper = new SPlayerWrapper();
	return PlayerWrapper;
}