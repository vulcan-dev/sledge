#pragma once

class CGame
{
public:
	unsigned int m_ResX; //0x0000
	unsigned int m_ResY; //0x0004
	unsigned int m_State; //0x0008
	unsigned int m_NextState; //0x000C
	float m_LoadingEffect; //0x0010
	bool m_Playing; //0x0014
	char pad_0015[3]; //0x0015
	float m_LoadingEffect2; //0x0018
	bool m_MapOpen; //0x001C
	char pad_001D[43]; //0x001D
	class CScene* m_Scene; //0x0048
	char pad_0050[1056]; //0x0050
}; //Size: 0x0470

inline CGame* g_Game;