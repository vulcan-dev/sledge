#include "sledgelib/netfuncs.h"

#include "teardown/classes/game.h"
#include "teardown/classes/player.h"

float GetHealth() {
	return g_Game->m_Player->m_Health;
}

void SetHealth(float fHealth) {
	g_Game->m_Player->m_Health = fHealth;
}

Vector3 GetPosition() {
	return g_Game->m_Player->m_Position;
}

void SetPosition(Vector3 vPosition) {
	g_Game->m_Player->m_Position = vPosition;
}

struct SPlayerWrapper {
	// internal delegate float GetHealthDelegate();
	void* m_GetHealth = GetHealth;

	// internal delegate void SetHealthDelegate(float fHealth);
	void* m_SetHealth = SetHealth;

	//internal delegate Vector3 GetPositionDelegate();
	void* m_GetPosition = GetPosition;

	//internal delegate void SetPositionDelegate(Vector3 vPosition);
	void* m_SetPosition = SetPosition;
};

SPlayerWrapper* PlayerWraper = new SPlayerWrapper();

void* SledgeLib::NetFuncs::GetPlayer() {
	return PlayerWraper;
}