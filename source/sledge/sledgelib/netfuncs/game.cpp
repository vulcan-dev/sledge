#include "teardown/classes/game.h"
#include "teardown/functions/misc.h"

#include <wtypes.h>
#include <oleauto.h>

#define sledgelib_func extern "C" __declspec(dllexport)

sledgelib_func unsigned int _GetState() { return static_cast<unsigned int>(g_Game->m_State); }
sledgelib_func void _SetState(unsigned int iState) { g_Game->m_NextState = static_cast<EGameState>(iState); }

sledgelib_func bool IsPlaying() { return g_Game->m_Playing; }

sledgelib_func bool GetMapOpen() { return g_Game->m_MapOpen; }
sledgelib_func void SetMapOpen(bool bValue) { g_Game->m_MapOpen = bValue; }

sledgelib_func void QuickSave() { g_Game->m_QuickSave = true; }
sledgelib_func void QuickLoad() { g_Game->m_QuickLoad = true; }

sledgelib_func float GetUpdateDelta() { return g_Game->m_UpdateDelta; }

sledgelib_func void _DebugPrint(char* sMessage) {
	small_string ssMessage(sMessage);

	Teardown::DebugPrint(g_Game->m_Debug, &ssMessage);
}