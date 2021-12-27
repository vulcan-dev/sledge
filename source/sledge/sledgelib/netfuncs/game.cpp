#include "teardown/classes/game.h"

#include <wtypes.h>
#include <oleauto.h>

#define _sledgelib_export extern "C" __declspec(dllexport)

_sledgelib_export unsigned int GetState() { return g_Game->m_State; }
_sledgelib_export void SetState(unsigned int iState) { g_Game->m_NextState = static_cast<EGameState>(iState); }

_sledgelib_export bool IsPlaying() { return g_Game->m_Playing; }

_sledgelib_export bool GetMapOpen() { return g_Game->m_MapOpen; }
_sledgelib_export void SetMapOpen(bool bValue) { g_Game->m_MapOpen = bValue; }