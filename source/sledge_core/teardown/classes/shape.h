#pragma once

#include "teardown/types/math.h"
#include <teardown/classes/entity.h>

class Shape : public Entity
{
public:
	class Transform m_LocalTransform; //0x0028
	Vector3 m_BoundsMin; //0x0044
	Vector3 m_BoundsMax; //0x0050
	char pad_005C[4]; //0x005C
	void* m_DormantPhysBuffer; //0x0060
	void* m_ActivePhysBuffer; //0x0068
	unsigned int m_Flags; //0x0070
	float N00001259; //0x0074
	float N0000118C; //0x0078
	char pad_007C[8]; //0x007C
	float N00001261; //0x0084
	float N00001283; //0x0088
	char pad_008C[12]; //0x008C
	void* m_VoxBuffer; //0x0098
	void* m_Joint; //0x00A0
	float m_Emissiveness; //0x00A8
	bool m_Broken; //0x00AC
	char pad_00AD[3]; //0x00AD
	class Transform m_Transform; //0x00B0
	char pad_00CC[4]; //0x00CC
}; //Size: 0x00D0