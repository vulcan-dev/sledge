#pragma once

#include "teardown/small_string.h"

enum class EGameState: unsigned int {
	Splash = 1,
	Menu = 2,
	UI = 3,
	Playing = 4,
	Editing = 5,
	Quitting = 6
};

class CGame
{
public:
	unsigned int m_ResX; //0x0000
	unsigned int m_ResY; //0x0004
	EGameState m_State; //0x0008
	EGameState m_NextState; //0x000C
	float m_LoadEffect; //0x0010
	bool m_Playing; //0x0014
	char pad_0015[3]; //0x0015
	float m_LoadEffect2; //0x0018
	bool m_MapOpen; //0x001C
	char pad_001D[3]; //0x001D
	void* N00000052; //0x0020
	void* N00000053; //0x0028
	class CGameInput* m_Input; //0x0030
	void* N00000055; //0x0038
	void* N00000056; //0x0040
	class CScene* m_Scene; //0x0048
	void* N00000078; //0x0050
	class CCameraController* m_CameraController; //0x0058
	class CEditor* m_Editor; //0x0060
	char pad_0068[8]; //0x0068
	class CHud* m_Hud; //0x0070
	class CHud* m_Hud2; //0x0078
	class CHud* m_Hud3; //0x0080
	class CHud* m_Hud4; //0x0088
	void* N00000080; //0x0090
	class CDefaultDevice* m_Device; //0x0098
	class CPlayer* m_Player; //0x00A0
	void* N00000083; //0x00A8
	void* N00000084; //0x00B0
	void* N00000085; //0x00B8
	void* m_Registry; //0x00C0
	void* N00000087; //0x00C8
	void* N00000088; //0x00D0
	void* N00000089; //0x00D8
	class CSteamInterface* m_SteamInterface; //0x00E0
	void* N0000008B; //0x00E8
	char pad_00F0[52]; //0x00F0
	float m_SoundSpeed; //0x0124
	float m_GameSpeed; //0x0128
	float m_TimeStep; //0x012C
	char pad_0130[16]; //0x0130
	float m_UpdateDelta; //0x0140
	char pad_0144[288]; //0x0144
	bool m_QuickLoad; //0x0264
	bool m_QuickSave; //0x0265
	char pad_0266[1]; //0x0266
	bool m_RespawnPlayer; //0x0267
	char pad_0268[96]; //0x0268
	class small_string m_LevelId; //0x02C8
	class small_string m_LevelPath; //0x02D8
	class small_string m_LevelLayers; //0x02E8
	char pad_02F8[376]; //0x02F8
}; //Size: 0x0470

inline CGame* g_Game;