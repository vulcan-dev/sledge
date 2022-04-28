#pragma once

#include "teardown/types/small_vector.h"

class ShaderInfo
{
public:
	char pad_0000[4]; //0x0000
	unsigned int m_Shader; //0x0004
	unsigned int m_Shader2; //0x0008
	unsigned int m_Program; //0x000C
	char pad_0010[40]; //0x0010
	small_vector<void*> m_Attributes; //0x0038
	char pad_0048[192]; //0x0048
	small_vector<void*> m_Uniforms; //0x0108
	char pad_0118[192]; //0x0118
	small_vector<void*> m_ShaderData; //0x01D8
	char pad_01E8[128]; //0x01E8
}; //Size: 0x0268

class FramebufferInfo
{
public:
	unsigned int m_FramebufferTextureId; //0x0000
	char pad_0004[8]; //0x0004
	unsigned int m_TextureId; //0x000C
	unsigned int m_NormalTextureId; //0x0010
	unsigned int N000004FC; //0x0014
	unsigned int N000004D3; //0x0018
	unsigned int m_DepthTextureId; //0x001C
	char pad_0020[12]; //0x0020
	unsigned int m_Width; //0x002C
	unsigned int m_Height; //0x0030
	unsigned int m_TextureCount; //0x0034
	unsigned int N000004E0; //0x0038
}; //Size: 0x003C
