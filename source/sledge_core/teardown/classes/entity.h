#include "teardown/enums.h"

class Entity
{
public:
	EEntityType m_Type; //0x0008
	unsigned char m_Flags; //0x0009
	char pad_000A[2]; //0x000A
	unsigned int m_Id; //0x000C
	class Entity* m_Parent;  //0x0010
	class Entity* m_Sibling; //0x0018
	class Entity* m_Child; //0x0020

	virtual void Destroy(bool bFree) = 0;
	virtual void SetParent(Entity* Parent) = 0;
	virtual void Function2() = 0;
	virtual void Function3() = 0;
}; //Size: 0x0028