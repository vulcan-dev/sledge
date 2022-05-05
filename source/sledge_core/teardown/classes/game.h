#pragma once

#include "teardown/enums.h"
#include "teardown/types/small_string.h"
#include "teardown/types/rendertypes.h"

#include <glm/mat4x4.hpp>

#pragma pack(push, 1)
class GameSettings
{
public:
	unsigned int m_RenderScale; //0x0000
	unsigned int m_Quality; //0x0004
	unsigned int m_Gamma; //0x0008
	unsigned int m_FOV; //0x000C
	unsigned int m_MotionBlur; //0x0010
	unsigned int m_Barrel; //0x0014
	unsigned int m_DOF; //0x0018
	unsigned int m_VSync; //0x001C
	unsigned int m_SoundVolume; //0x0020
	unsigned int m_AmbienceVolume; //0x0024
	unsigned int m_MusicVolume; //0x0028
	unsigned int m_MenuMusic; //0x002C
	unsigned int m_MouseSmoothing; //0x0030
	unsigned int m_MouseSensitivity; //0x0034
	unsigned int m_MouseInvert; //0x0038
	unsigned int m_ToolSway; //0x003C
	unsigned int m_HeadBob; //0x0040
}; //Size: 0x0044

class Game
{
public:
	uint32_t m_ResX; //0x0000
	uint32_t m_ResY; //0x0004
	EGameState m_State; //0x0008
	EGameState m_NextState; //0x000C
	float m_LoadingScreen; //0x0010
	int8_t m_InGame; //0x0014
	char pad_0015[3]; //0x0015
	float m_LoadingScreen2; //0x0018
	bool m_MapOpen; //0x001C
	char pad_001D[3]; //0x001D
	void* N00000074; //0x0020
	RenderInfo* m_RenderInfo; //0x0028
	void* N00000054; //0x0030
	void* N00000055; //0x0038
	class Renderer* m_Renderer; //0x0040
	class Scene* m_Scene; //0x0048
	void* m_Sound; //0x0050
	void* m_Camera; //0x0058
	void* m_Editor; //0x0060
	void* m_SplashHud; //0x0068
	void* m_LoadingHud; //0x0070
	void* m_GameHud; //0x0078
	void* m_MenuHud; //0x0080
	void* N0000007E; //0x0088
	void* m_Debug; //0x0090
	void* m_Device; //0x0098
	class Player* m_Player; //0x00A0
	void* m_LevelInfo; //0x00A8
	void* N00000083; //0x00B0
	void* N00000084; //0x00B8
	void* m_Registry; //0x00C0
	void* m_Mods; //0x00C8
	void* N00000087; //0x00D0
	void* N00000088; //0x00D8
	void* N00000089; //0x00E0
	void* N0000008A; //0x00E8
	char pad_00F0[48]; //0x00F0
	bool m_Pause; //0x0120
	char pad_0121[3]; //0x0121
	uint32_t m_UpdateCount; //0x0124
	uint32_t N00000112; //0x0128
	char pad_012C[8]; //0x012C
	float m_TimeStep; //0x0134
	char pad_0138[52]; //0x0138
	glm::mat4 m_PerspectiveMatrix; //0x016C
	glm::mat4 m_UnkMatrix; //0x01AC
	glm::mat4 m_HUDMVPMatrix; //0x01EC
	glm::mat4 m_HUDScaleMatrix; //0x022C
	bool m_QuickLoad; //0x026C
	bool m_QuickSave; //0x026D
	bool m_ApplyGraphics; //0x026E
	bool m_RespawnPlayer; //0x026F
	char pad_0270[32]; //0x0270
	class GameSettings m_Settings; //0x0290
	char pad_02D4[108]; //0x02D4
	class small_string m_LevelId; //0x0340
	class small_string m_LevelPath; //0x0350
	class small_string m_LevelLayers; //0x0360
	class small_string m_LevelModId; //0x0370
	char pad_0380[104]; //0x0380
	FramebufferInfo m_FinalFBO; //0x03E8
	bool m_Usingindless; //0x0424
	char pad_0425[11]; //0x0425
	bool m_UsingPhotomode; //0x0430
	char pad_0431[3]; //0x0431
	FramebufferInfo m_PhotoModeFBO; //0x0434
	bool m_TakeScreenshot; //0x0470
	char pad_0471[8]; //0x0471
}; //Size: 0x0479
#pragma pack(pop)

inline Game* g_Game;