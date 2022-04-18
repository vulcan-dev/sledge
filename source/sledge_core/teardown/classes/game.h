#include "teardown/enums.h"
#include "teardown/types/small_string.h"

class Game
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
	void* N00000053; //0x0020
	void* N00000054; //0x0028
	class Input* m_Input; //0x0030
	char pad_0038[16]; //0x0038
	class Scene* m_Scene; //0x0048
	void* m_SoundSystem; //0x0050
	void* m_CameraController; //0x0058
	void* m_Editor; //0x0060
	class HUD* m_SplashHud; //0x0068
	class HUD* m_LoadingHud; //0x0070
	class HUD* m_GameHud; //0x0078
	class HUD* m_MenuHud; //0x0080
	void* N0000012D; //0x0088
	void* N0000012E; //0x0090
	void* m_Device; //0x0098
	class Player* m_Player; //0x00A0
	class LevelInfo* m_LevelInfo; //0x00A8
	void* N00000132; //0x00B0
	void* N00000133; //0x00B8
	void* m_Registry; //0x00C0
	class ModManager* m_ModManager; //0x00C8
	void* N00000136; //0x00D0
	void* N00000137; //0x00D8
	void* N00000138; //0x00E0
	void* N00000139; //0x00E8
	char pad_00F0[380]; //0x00F0
	bool m_QuickLoad; //0x026C
	bool m_QuickSave; //0x026D
	char pad_026E[210]; //0x026E
	small_string m_LevelId; //0x0340
	small_string m_LevelPath; //0x0350
	small_string m_LevelLayers; //0x0360
	small_string m_LevelModId; //0x0370
	char pad_0380[248]; //0x0380
	bool N000001AB; //0x0478
	char pad_0479[7]; //0x0479
}; //Size: 0x0480

inline Game* g_Game;