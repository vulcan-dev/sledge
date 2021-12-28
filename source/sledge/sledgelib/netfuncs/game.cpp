#include "teardown/classes/game.h"

#include <wtypes.h>
#include <oleauto.h>

#define sledgelib_func extern "C" __declspec(dllexport)

sledgelib_func unsigned int GetState() { return g_Game->m_State; }
sledgelib_func void SetState(unsigned int iState) { g_Game->m_NextState = static_cast<EGameState>(iState); }

sledgelib_func bool IsPlaying() { return g_Game->m_Playing; }

sledgelib_func bool GetMapOpen() { return g_Game->m_MapOpen; }
sledgelib_func void SetMapOpen(bool bValue) { g_Game->m_MapOpen = bValue; }