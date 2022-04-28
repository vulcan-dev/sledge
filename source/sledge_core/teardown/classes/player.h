#pragma once

#include "teardown/types/small_string.h"
#include "teardown/types/small_vector.h"
#include "teardown/types/math.h"

#pragma pack(push, 1)
class Tool
{
public:
	small_string m_ModeLpath; //0x0000
	float m_ViewmodelRecoil; //0x0010
	float m_ViewPunch; //0x0014
	Vector3 m_Position; //0x0018
	Vector4 m_Rotation; //0x0024
	float m_Cooldown; //0x0034
	unsigned int m_Slot; //0x0038
	char m_Unused[4]; //0x003C
}; //Size: 0x0040

class RegisteredTool
{
public:
	small_string m_Id;
	small_string m_Name; //0x0010
	small_string m_Model; //0x0020
	unsigned int m_Group; //0x0030
	char pad_0034[4]; //0x0034
}; //Size: 0x0038

class Player
{
public:
	Vector3 m_Position; //0x0000
	char pad_000C[44]; //0x000C
	Vector3 m_Velocity; //0x0038
	char pad_0044[28]; //0x0044
	class Transform m_CameraTransform; //0x0060
	class Transform m_CameraTransform2; //0x007C
	class Transform m_CameraTransform3; //0x0098
	float m_CamPitchAnim; //0x00B4
	float m_CamRollAnim; //0x00B8
	float m_TravelDistance; //0x00BC
	float m_TravelDistanceClamped; //0x00C0
	Vector2 m_CameraAngle; //0x00C4
	Vector2 m_MovementKeys; //0x00CC
	Vector2 m_MouseDelta; //0x00D4
	char pad_00DC[1]; //0x00DC
	bool m_Crouching; //0x00DD
	char pad_00DE[1]; //0x00DE
	bool m_M2Down; //0x00DF
	char pad_00E0[1]; //0x00E0
	bool m_M1Down; //0x00E1
	char pad_00E2[2]; //0x00E2
	float m_Height; //0x00E4
	float N000003A0; //0x00E8
	float m_CollisionRadius; //0x00EC
	float N000003A1; //0x00F0
	float m_ClimbRadius; //0x00F4
	float m_BaseHeight; //0x00F8
	float N000012C8; //0x00FC
	float m_CurrentHeight; //0x0100
	float m_CrouchCompletion; //0x0104
	float m_RunSpeed; //0x0108
	char pad_010C[4]; //0x010C
	class Body* m_GrabbedBody; //0x0110
	char pad_0118[48]; //0x0118
	class Body* m_GrabBody; //0x0148
	char pad_0150[8]; //0x0150
	float m_Health; //0x0158
	char pad_015C[68]; //0x015C
	class Tool m_Sledge; //0x01A0
	class Tool m_Spraycan; //0x01E0
	class Tool m_Extinguisher; //0x0220
	class Tool m_Blowtorch; //0x0260
	class Tool m_Shotgun; //0x02A0
	class Tool m_Plank; //0x02E0
	class Tool m_Pipebomb; //0x0320
	class Tool m_Gun; //0x0360
	class Tool m_Bomb; //0x03A0
	class Tool m_Wire; //0x03E0
	class Tool m_Rocket; //0x0420
	class Tool m_Booster; //0x0460
	class Tool m_Leafblower; //0x04A0
	class Tool m_Turbo; //0x04E0
	class Tool m_Explosive; //0x0520
	class Tool m_Rifle; //0x0560
	class Tool m_Steroid; //0x05A0
	class small_string m_EquippedToolName; //0x05E0
	class Body* m_ToolBody; //0x05F0
	char pad_05F8[28]; //0x05F8
	class Transform m_ToolTransform; //0x0614
	char pad_0630[64]; //0x0630
	class Body* m_FloorBody; //0x0670
	char pad_0678[112]; //0x0678
	class Shape* m_GrabbedShape; //0x06E8
	class Shape* m_InteractableShape; //0x06F0
	char pad_06F8[12500]; //0x06F8
	float N00001549; //0x37CC
	small_vector<RegisteredTool> m_RegisteredTools; //0x37D0
	Transform m_CustomToolTransform; //0x37E0
	float m_ToolSway; //0x37FC Applies to all tools, values higher than 1.428 break it
	char pad_3800[824]; //0x3800
}; //Size: 0x3B38

#pragma pack(pop)

inline Player* g_Player;