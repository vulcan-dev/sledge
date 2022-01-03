#pragma once
#include "util/math.h"

struct SProjectile
{
public:
	Vector3 m_Position; //0x0000
	Vector3 m_Velocity; //0x000C
	float m_LifeTime; //0x0018
	float m_MaxLifeTime; //0x001C
	unsigned int m_Type; //0x0020
	float m_DamageRadius; //0x0024
}; //Size: 0x0028