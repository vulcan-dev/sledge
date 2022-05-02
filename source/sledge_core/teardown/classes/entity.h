#pragma once

#include "teardown/enums.h"

#pragma pack(push, 1)
class Entity
{
public:
	EEntityType m_Type; //0x0008
	uint16_t m_Flags; //0x0009
	char pad_000B[1]; //0x000B
	unsigned int m_Id; //0x000C
	Entity* m_Parent; //0x0010
	Entity* m_Sibling; //0x0018
	Entity* m_Child; //0x0020

	virtual void Destroy(bool bFree) = 0;
	virtual void SetParent(Entity* Parent) = 0;
	virtual void Function2() = 0;
	virtual void Function3() = 0;
}; //Size: 0x0028
#pragma pack(pop)