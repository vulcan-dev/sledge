#pragma once

#include "teardown/types/math.h"
#include <teardown/classes/entity.h>

class Body : public Entity
{
public:
	Transform m_Transform; //0x0028
	Transform m_Transform2; //0x0044
	Transform m_Transform3; //0x0060
	Vector3 m_Velocity; //0x007C
	Vector4 m_AngularVelocity; //0x0088
	char pad_0098[12]; //0x0098
	float m_Mass; //0x00A4
	Vector3 m_CenterOfMass; //0x00A8
	char pad_00B4[40]; //0x00B4
	bool m_Dynamic; //0x00DC
	char pad_00DD[7]; //0x00DD
	unsigned int m_ActiveTicksLeft; //0x00E4
	bool m_Colliding; //0x00E5
	char pad_00E6[14]; //0x00E6
}; //Size: 0x00F4