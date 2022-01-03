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


struct SParticleParamV4
{
public:
	unsigned int m_InterpolationType; //0x0000 0 - linear | 1 - smooth | 2 - easein | 3 - constant | 4 - easeout
	float m_FadeIn; //0x0004
	float m_FadeOut; //0x0008
	Vector4 m_Start; //0x000C
	Vector4 m_End; //0x001C
}; //Size: 0x002C

struct SParticleParamF
{
public:
	unsigned int m_InterpolationType; //0x0000 0 - linear | 1 - smooth | 2 - easein | 3 - constant | 4 - easeout
	float m_FadeIn; //0x0004
	float m_FadeOut; //0x0008
	float m_Start; //0x000C
	float m_End; //0x0010
}; //Size: 0x0014


struct SParticleInfo
{
public:
	SParticleParamV4 m_Color;
	SParticleParamF m_Alpha; //0x002C
	SParticleParamF m_Radius; //0x0040
	SParticleParamF m_Gravity; //0x0054
	SParticleParamF m_Drag; //0x0068
	SParticleParamF m_Emissive; //0x007C
	SParticleParamF m_Rotation; //0x0090
	SParticleParamF m_Collide; //0x00A4
	SParticleParamF m_Stretch; //0x00B8
	SParticleParamF m_Sticky; //0x00CC
	char pad_00E0[20]; //0x00E0
	unsigned int m_Tile; //0x00F4
	char pad_00F8[8]; //0x00F8
	unsigned int m_Flags; //0x0100
	char pad_0104[4]; //0x0104
}; //Size: 0x0108