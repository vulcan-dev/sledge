#pragma once

#include "util/math.h"
#include "teardown/small_string.h"
#include "teardown/small_vector.h"

class CRegisteredTool
{
public:
	small_string m_Id; //0x0000
	small_string m_Name; //0x0010
	small_string m_File; //0x0020
	int32_t m_Group; //0x0030
	char pad_0034[4]; //0x0034
}; //Size: 0x0038

class CPlayer
{
public:
	Vector3 m_Position; //0x0000
	char pad_000C[44]; //0x000C
	Vector3 m_Velocity; //0x0038
	char pad_0044[12]; //0x0044
	Vector3 m_VelocityProcessed; //0x0050
	char pad_005C[4]; //0x005C
	Transform m_CameraTransform; //0x0060
	Transform m_CameraTransform2; //0x007C
	Transform m_CameraTransform3; //0x0098
	float m_CamPitchAnim; //0x00B4
	float m_CamRollAnim; //0x00B8
	float m_TravelDistance; //0x00BC
	float m_TravelDistanceClamped; //0x00C0
	Vector2 m_CameraAngles; //0x00C4 Camera angles in radians
	Vector2 m_MovementKeys; //0x00CC
	Vector2 m_MouseDelta; //0x00D4
	char pad_00DC[1]; //0x00DC
	bool m_Crouching; //0x00DD
	char pad_00DE[1]; //0x00DE
	bool m_M2Down; //0x00DF
	char pad_00E0[1]; //0x00E0
	bool m_M1Down; //0x00E1
	char pad_00E2[46]; //0x00E2
	class CBody* m_GrabbedBody; //0x0110
	char pad_0118[48]; //0x0118
	class CBody* m_GrabBody; //0x0148
	char pad_0150[8]; //0x0150
	float m_Health; //0x0158
	char pad_015C[1156]; //0x015C
	small_string m_ToolName; //0x05E0
	class CBody* m_ToolBody; //0x05F0
	char pad_05F8[28]; //0x05F8
	Transform m_ToolTransform; //0x0614
	char pad_0630[8]; //0x0630
	float m_ToolRecoil; //0x0638 breaks if > 1
	float m_EquipAnim; //0x063C union between float and bool
	float m_ToolCooldown; //0x0640
	char pad_0644[96]; //0x0644
	float m_TimeSinceLastJump; //0x06A4
	char pad_06A8[64]; //0x06A8
	class CShape* m_GrabbedShape; //0x06E8
	class CShape* m_InteractableShape; //0x06F0
	char pad_06F8[132]; //0x06F8
	bool m_BlurEffect; //0x077C
	char pad_077D[12371]; //0x077D
	small_vector<CRegisteredTool> m_RegisteredTools; //0x37D0
	Transform m_RegisteredToolTransform; // 0x37D0
	float m_RegisteredToolSway; // 0x37E0
	Vector3 m_GroundVelocity; //0x3800
	char pad_380C[812]; //0x380C
}; //Size: 0x3B38