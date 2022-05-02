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
	Transform m_Transform;
	class Transform m_DeltaTransform; //0x001C
	Vector3 m_Velocity; //0x0038
	Vector3 N00000390; //0x0044
	Vector3 m_ProcessedVelocity; //0x0050
	int m_PhysicInteractions; //0x005C
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
	bool m_Jump; //0x00DC
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
	bool m_Throwing; //0x0150
	char pad_0151[3]; //0x0151
	float m_GrabbingTravelDistance; //0x0154
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
	char pad_0630[24]; //0x0630
	float m_IsOnGround; //0x0648 what's the purpose of storing this as a float?
	float m_AirTime; //0x064C
	float m_IsOnGround2; //0x0650
	float m_AirTime2; //0x0654
	Vector3 m_GroundNormal; //0x0658
	Vector3 m_GroundCollisionPos; //0x0664
	class Body* m_CurrentCollidingBody; //0x0670
	char pad_0678[44]; //0x0678
	float m_TimeSinceLastJump; //0x06A4
	float m_FastViewPunch; //0x06A8
	float m_SlowViewPunch; //0x06AC
	char pad_06B0[16]; //0x06B0
	class Shape* m_TargetShape; //0x06C0
	float m_TargetDist; //0x06C8
	Vector3 m_TargetPos; //0x06CC
	char pad_06D8[16]; //0x06D8
	class Shape* m_GrabbedShape; //0x06E8
	class Shape* m_InteractableShape; //0x06F0
	char pad_06F8[112]; //0x06F8
	float m_JumpHeight; //0x0768
	float N000012E1; //0x076C
	char pad_0770[13]; //0x0770
	bool m_StandingOnSnow; //0x077D
	char pad_077E[23]; //0x077E
	bool N000012C1; //0x0795
	char pad_0796[12342]; //0x0796
	float N00001549; //0x37CC
	small_vector<RegisteredTool> m_RegisteredTools; //0x37D0
	Transform m_CustomToolTransform; //0x37E0
	float m_ToolSway; //0x37FC Applies to all tools, values higher than 1.428 break it
	char pad_3800[820]; //0x3800
	float m_Slipperiness; //0x3B34
	char pad_3B38[4]; //0x3B38

}; //Size: 0x3B38

#pragma pack(pop)

inline Player* g_Player;