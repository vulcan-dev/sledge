#include "teardown/classes/game.h"

#define sledgelib_func extern "C" __declspec(dllexport)

sledgelib_func void _GetLevelPath(char* cReturn, unsigned int iReturnBufferSize) {
	size_t LevelPathLen = g_Game->m_LevelPath.len();

	if (LevelPathLen > iReturnBufferSize) {
		memcpy(cReturn, g_Game->m_LevelPath.c_str(), iReturnBufferSize);
		cReturn[iReturnBufferSize] = '\0';
	}

	memcpy(cReturn, g_Game->m_LevelPath.c_str(), LevelPathLen);
	cReturn[LevelPathLen] = '\0';
}

sledgelib_func void _GetLevelLayers(char* cReturn, unsigned int iReturnBufferSize) {
	size_t LevelLayersLen = g_Game->m_LevelLayers.len();

	if (LevelLayersLen > iReturnBufferSize) {
		memcpy(cReturn, g_Game->m_LevelLayers.c_str(), iReturnBufferSize);
		cReturn[iReturnBufferSize] = '\0';
	}

	memcpy(cReturn, g_Game->m_LevelLayers.c_str(), LevelLayersLen);
	cReturn[LevelLayersLen] = '\0';
}

sledgelib_func void _GetLevelId(char* cReturn, unsigned int iReturnBufferSize) {
	size_t LevelIdLen = g_Game->m_LevelId.len();

	if (LevelIdLen > iReturnBufferSize) {
		memcpy(cReturn, g_Game->m_LevelId.c_str(), iReturnBufferSize);
		cReturn[iReturnBufferSize] = '\0';
	}

	memcpy(cReturn, g_Game->m_LevelId.c_str(), LevelIdLen);
	cReturn[LevelIdLen] = '\0';
}

sledgelib_func void _LoadLevel(char* cLevelPath, char* cLevelLayers, char* cLevelId, char* cLevelModId) {
	small_string ssLevelPath(cLevelPath);
	small_string ssLevelLayers(cLevelLayers);
	small_string ssLevelId(cLevelId);
	small_string ssLevelModId(cLevelModId);

	memcpy(&g_Game->m_LevelPath, &ssLevelPath, sizeof(small_string));
	memcpy(&g_Game->m_LevelLayers, &ssLevelLayers, sizeof(small_string));
	memcpy(&g_Game->m_LevelId, &ssLevelId, sizeof(small_string));
	memcpy(&g_Game->m_LevelModId, &ssLevelModId, sizeof(small_string));

	g_Game->m_NextState = EGameState::Playing;
}