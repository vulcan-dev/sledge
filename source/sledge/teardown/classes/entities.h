#pragma once

#include "teardown/small_string.h"
#include "teardown/small_vector.h"
#include "util/math.h"

enum EEntityType : unsigned char {
	Body = 1,
	Shape = 2,
	Light = 3,
	Location = 4,
	Water = 5,
	Enemy = 6,
	Joint = 7,
	Vehicle = 8,
	Wheel = 9,
	Screen = 10,
	Trigger = 11,
	Script = 12
};

class CVox;

class CEntity
{
public:
	EEntityType m_Type; //0x0008
	unsigned char m_Flags; //0x0009
	char pad_000A[2]; //0x000A
	unsigned int m_Id; //0x000C
	class CEntity* m_Parent;  //0x0010
	class CEntity* m_Sibling; //0x0018
	class CEntity* m_Child; //0x0020

	virtual void Destroy(bool bFree) = 0;
	virtual void SetParent(CEntity* Parent) = 0;
	virtual void Function2() = 0;
	virtual void Function3() = 0;
}; //Size: 0x0028

class CBody : public CEntity
{
public:
	Transform m_Transform; //0x0028
	Transform m_Transform2; //0x0044
	Transform m_Transform3; //0x0060
	Vector3 m_Velocity; //0x007C
	Vector3 m_AngularVelocity; //0x0088
	char pad_0094[12]; //0x0094
	float m_Mass; //0x00A0
	Vector3 m_CenterOfMass; //0x00A4
	char pad_00B0[44]; //0x00B0
	bool m_Dynamic; //0x00DC
	char pad_00DD[7]; //0x00DD
	unsigned char m_ActiveTimeLeft; //0x00E4
	bool m_Colliding; //0x00E5
	char pad_00E6[2]; //0x00E6
}; //Size: 0x00E8

class CShape : public CEntity
{
public:
	Transform m_LocalTransform; //0x0028
	Bounds m_Bounds; //0x0044
	char pad_005C[4]; //0x005C
	void* N0000053F; //0x0060
	void* N000004EE; //0x0068
	char pad_0070[2]; //0x0070
	unsigned char m_CollisionLayer; //0x0072
	unsigned char m_CollisionMask; //0x0073
	char pad_0074[36]; //0x0074
	CVox* m_Vox; //0x0098
	char pad_00A0[16]; //0x00A0
	float m_EmissiveScale; //0x00B0
	bool m_Broken; //0x00B4
	char pad_00B5[3]; //0x00B5
	Vector3 m_Position; //0x00B8
	Vector4 m_Rotation; //0x00C4
	char pad_00D4[4]; //0x00D4
}; //Size: 0x00D8

class CLight : public CEntity
{
public:
	bool m_Active; //0x0028
	char pad_0029[3]; //0x0029
	unsigned char m_Type; //0x002C 1 - Sphere | 2 - Capsule | 3 - Cone | 4+ - Area
	char pad_002D[3]; //0x002D
	Vector3 m_Position; //0x0030
	Vector4 m_Rotation; //0x003C
	Vector3 m_Color; //0x004C
	char pad_0058[4]; //0x0058
	float m_Intensity; //0x005C
	float m_Reach; //0x0060
	float m_Size; //0x0064
	float m_Unshadowed; //0x0068
	float m_Angle; //0x006C
	float m_Penumbra; //0x0070
	float m_FogIter; //0x0074
	float m_FogScale; //0x0078
	Vector2 m_AreaSize; //0x007C
	float m_Glare; //0x0084
	float m_VisibleGlare; //0x0088
	char pad_008C[20]; //0x008C
	float m_IntensityMult; //0x00A0
	char pad_00A4[4796]; //0x00A4
}; //Size: 0x1360

class CVehicle : public CEntity
{
public:
	CBody* m_Body; //0x0028
	Vector3 m_Position; //0x0030
	Vector4 m_Rotation; //0x003C
	char pad_004C[28]; //0x004C
	small_vector<class CWheel*> m_Wheels; //0x0068
	char pad_0078[68]; //0x0078
	float m_TopSpeed; //0x00BC
	char pad_00C0[4]; //0x00C0
	float m_Spring; //0x00C4
	float m_Damping; //0x00C8
	float m_Acceleration; //0x00CC
	float m_Strength; //0x00D0
	float m_Friction; //0x00D4
	char pad_00D8[4]; //0x00D8
	bool m_Driven; //0x00DC
	char pad_00DD[3]; //0x00DD
	float m_AntiSpin; //0x00E0
	float m_SteerAssist; //0x00E4
	char pad_00E8[4]; //0x00E8
	float m_AntiRoll; //0x00EC
	float m_DiffLock; //0x00F0
	char pad_00F4[4]; //0x00F4
	small_string m_SoundName; //0x00F8
	float m_SoundVolume; //0x0108
	char pad_010C[56]; //0x010C
	float m_Handbrake; //0x0144
	char pad_0148[12]; //0x0148
	float N00000BC4; //0x0154
	char pad_0158[12]; //0x0158
	float m_AccelSound; //0x0164
	char pad_0168[4]; //0x0168
	float m_Health; //0x016C
	char pad_0170[32]; //0x0170
	CBody* m_Body2; //0x0190
	char pad_0198[112]; //0x0198
	CBody* m_Body3; //0x0208
	char pad_0210[128]; //0x0210
}; //Size: 0x0290