#include "teardown/classes/entity.h"
#include "teardown/types/math.h"

#pragma pack(push, 1)
class Joint : public Entity
{
public:
	class Shape* m_ChildShape; //0x0028
	class Shape* m_ParentShape; //0x0030
	void* m_ParentJoint; //0x0038
	void* m_SiblingJoint; //0x0040
	unsigned int m_Type; //0x0048 1 - ball | 2 - hinge | 3 - prismatic
	float m_Size; //0x004C
	bool m_Collide; //0x0050
	bool m_Enabled; //0x0051
	char pad_0052[2]; //0x0052
	float m_RotStrength; //0x0054
	float m_RotSpring; //0x0058
	Vector3 m_RotForce; //0x005C
	Vector3 m_JointLocalPosParent; //0x0068
	Vector3 m_JointLocalPosChild; //0x0074
	char pad_0080[24]; //0x0080
	Vector4 m_Angle; //0x0098
	char pad_00A8[32]; //0x00A8
	bool m_UnkBool; //0x00C8
	char pad_00C9[3]; //0x00C9
	float m_UnkFloat; //0x00CC
	float m_UnkFloat2; //0x00D0
	char pad_00D4[4]; //0x00D4
}; //Size: 0x00D8
#pragma pack(pop)