#pragma once

#include "teardown/small_vector.h"
#include "teardown/classes/entities.h"
#include <teardown/classes/misc.h>

class CScene
{
public:
	void* m_Physics; //0x0008
	char pad_0010[88]; //0x0010
	void* m_SmokeParticles; //0x0068
	char pad_0070[8]; //0x0070
	void* m_WaterParticles; //0x0078 used for old particle system
	void* m_PlainParticles; //0x0080 used for old particle system
	void* m_FireSystem; //0x0088
	small_vector<SProjectile> m_Projectiles; //0x0090
	class CEnvironment* m_Environment; //0x00A0
	char pad_00A8[32]; //0x00A8
	class CLight* m_Flashlight; //0x00C8
	char pad_00D0[48]; //0x00D0
	class CVehicle* m_CurrentVehicle; //0x0100
	char pad_0108[64]; //0x0108
	small_vector<class CBody*> m_Bodies; //0x0148
	small_vector<class CShape*> m_Shapes; //0x0158
	small_vector<class CLight*> m_Lights; //0x0168
	small_vector<class CLocation*> m_Locations; //0x0178
	small_vector<class CWater*> m_Waters; //0x0188
	small_vector<class CEnemy*> m_Enemies; //0x0198
	small_vector<class CJoint*> m_Joints; //0x01A8
	small_vector<class CVehicle*> m_Vehicles; //0x01B8
	small_vector<class CWheel*> m_Wheels; //0x01C8
	small_vector<class CScreen*> m_Screens; //0x01D8
	small_vector<class CTrigger*> m_Triggers; //0x01E8
	small_vector<class CScript*> m_Scripts; //0x01F8
	char pad_0208[1672]; //0x0208
	small_vector<class CEntity*> m_Entities; //0x0890
	char pad_08A0[48]; //0x08A0

	virtual void Function0();
	virtual void Function1();
}; //Size: 0x08D0

inline CScene* g_Scene;