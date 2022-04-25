#pragma once

#include "teardown/classes/entity.h"
#include "teardown/types/math.h"
#include "teardown/types/small_vector.h"

#pragma pack(push, 1)
class Vehicle : public Entity
{
public:
	class Body* m_Body; //0x0028
	class Transform m_Transform; //0x0030
	char pad_004C[28]; //0x004C
	small_vector<class Wheel*> m_Wheels; //0x0068
	char pad_0078[536]; //0x0078
}; //Size: 0x0290
#pragma pack(pop)