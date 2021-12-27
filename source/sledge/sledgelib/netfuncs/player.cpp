#include "teardown/classes/game.h"
#include "teardown/classes/player.h"

#define _sledgelib_export extern "C" __declspec(dllexport)

_sledgelib_export void SetHealth(float fValue) { g_Game->m_Player->m_Health = fValue; }
_sledgelib_export float GetHealth() { return g_Game->m_Player->m_Health; }

_sledgelib_export void SetPosition(Vector3 vPosition) { g_Game->m_Player->m_Position = vPosition; }
_sledgelib_export Vector3 GetPosition() { return g_Game->m_Player->m_Position; }

_sledgelib_export void SetVelocity(Vector3 vVelocity) { g_Game->m_Player->m_Velocity = vVelocity; }
_sledgelib_export Vector3 GetVelocity() { return g_Game->m_Player->m_Velocity; }

_sledgelib_export void SetCameraTransform(Transform tCameraTransform) { g_Game->m_Player->m_CameraTransform2 = tCameraTransform; }
_sledgelib_export Transform GetCameraTransform() { return g_Game->m_Player->m_CameraTransform2; }

_sledgelib_export Vector2 GetMovementInput() { return g_Game->m_Player->m_MovementKeys; }
_sledgelib_export Vector2 GetMouseInput() { return g_Game->m_Player->m_MouseInput; }

_sledgelib_export void Respawn() { g_Game->m_RespawnPlayer = true; }