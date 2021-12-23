#include "util/math.h"

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
	Vector2 m_MouseInput; //0x00C4 useful for calculating camera rotation
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
	char pad_0118[64]; //0x0118
	float m_Health; //0x0158
	char pad_015C[1420]; //0x015C
	class CShape* m_GrabbedShape; //0x06E8
	class CShape* m_InteractableShape; //0x06F0
	char pad_06F8[132]; //0x06F8
	bool m_BlurEffect; //0x077C
	char pad_077D[12419]; //0x077D
	Vector3 m_GroundVelocity; //0x3800
	char pad_380C[812]; //0x380C
}; //Size: 0x3B38