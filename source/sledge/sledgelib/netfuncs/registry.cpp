#include "teardown/functions/registry.h"
#include "teardown/classes/game.h"

#include "teardown/small_string.h"

#define sledgelib_func extern "C" __declspec(dllexport)

sledgelib_func void SetBool(char* cKeyName, bool bValue) { Teardown::SetBool(g_Game->m_Registry, new small_string(cKeyName), bValue); }
sledgelib_func bool GetBool(char* cKeyName) { return Teardown::GetBool(g_Game->m_Registry, new small_string(cKeyName)); }

sledgelib_func void SetInt(char* cKeyName, int iValue) { Teardown::SetInt(g_Game->m_Registry, new small_string(cKeyName), iValue); }
sledgelib_func int GetInt(char* cKeyName) { return Teardown::GetInt(g_Game->m_Registry, new small_string(cKeyName)); }

sledgelib_func void SetFloat(char* cKeyName, float fValue) { Teardown::SetFloat(g_Game->m_Registry, new small_string(cKeyName), fValue); }
sledgelib_func float GetFloat(char* cKeyName) {
	small_string ssKeyName(cKeyName);
	float fValue = Teardown::GetFloat(g_Game->m_Registry, &ssKeyName);
	return fValue;
}