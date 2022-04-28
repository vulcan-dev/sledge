#pragma once
#include "teardown/types/small_string.h"
#include "teardown/types/small_vector.h"

#pragma pack(push, 1)
class Scene
{
public:
	void* m_Physics; //0x0008
	void* m_Unknown; //0x0010
	char pad_0018[8]; //0x0018
	void* N0000009F; //0x0020
	void* N000000A0; //0x0028
	void* N000000A1; //0x0030
	void* N000000A2; //0x0038
	char pad_0040[40]; //0x0040
	void* m_SmokeParticles; //0x0068
	void* m_WaterParticles; //0x0070
	void* m_PlainParticles; //0x0078
	void* m_FireSystem; //0x0080
	small_vector<class Projectile*> m_Projectiles; //0x0088
	class Environment* m_Environment; //0x0098
	char pad_00A0[32]; //0x00A0
	class Light* m_Flashlight; //0x00C0
	class Script* m_ExplosionScript; //0x00C8
	char pad_00D0[40]; //0x00D0
	class Vehicle* m_CurrentVehicle; //0x00F8
	class Screen* m_CurrentScreen; //0x0100
	char pad_0108[56]; //0x0108
	small_vector<class Body*> m_Bodies; //0x0140
	small_vector<class Shape*> m_Shapes; //0x0150
	small_vector<class Light*> m_Lights; //0x0160
	small_vector<class Location*> m_Locations; //0x0170
	small_vector<class Water*> m_Waters; //0x0180
	small_vector<class Joint*> m_Joints; //0x0190
	small_vector<class Vehicle*> m_Vehicle; //0x01A0
	small_vector<class Wheel*> m_Wheels; //0x01B0
	small_vector<class Screen*> m_Screens; //0x01C0
	small_vector<class Trigger*> m_Triggers; //0x01D0
	small_vector<class Script*> m_Scripts; //0x01E0
	char pad_01F0[8]; //0x01F0
	class small_vector<void*>* m_ScriptPointers; //0x01F8
	class small_vector<void*>* m_BodiesPtr; //0x0200
	class small_vector<void*>* m_ShapesPtr; //0x0208
	class small_vector<void*>* m_LightsPtr; //0x0210
	class small_vector<void*>* m_LocationsPtr; //0x0218
	class small_vector<void*>* m_WatersPtr; //0x0220
	class small_vector<void*>* m_JointsPtr; //0x0228
	class small_vector<void*>* m_VehiclesPtr; //0x0230
	class small_vector<void*>* m_WheelsPtr; //0x0238
	class small_vector<void*>* m_ScreensPtr; //0x0240
	class small_vector<void*>* m_TriggersPtr; //0x0248
	class small_vector<void*>* m_ScriptsPtr; //0x0250
	char pad_0258[1560]; //0x0258
	small_vector<class Entity*> m_Entities; //0x0870
	char pad_0880[4]; //0x0880
	unsigned int m_VoxCount; //0x0884
	char pad_0888[56]; //0x0888

	virtual void Function0();
	virtual void Function1();
}; //Size: 0x08C0
#pragma pack(pop)

inline Scene* g_Scene = nullptr;