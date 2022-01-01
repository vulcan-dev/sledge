#include "teardown/functions/registry.h"
#include "teardown/classes/game.h"

#include "teardown/small_string.h"

#define sledgelib_func extern "C" __declspec(dllexport)

sledgelib_func void SetBool(char* cKeyName, bool bValue) { Teardown::SetBool(g_Game->m_Registry, new small_string(cKeyName), bValue); }
sledgelib_func bool GetBool(char* cKeyName) { return Teardown::GetBool(g_Game->m_Registry, new small_string(cKeyName)); }

sledgelib_func void SetInt(char* cKeyName, int iValue) { Teardown::SetInt(g_Game->m_Registry, new small_string(cKeyName), iValue); }
sledgelib_func int GetInt(char* cKeyName) { return Teardown::GetInt(g_Game->m_Registry, new small_string(cKeyName)); }

sledgelib_func void SetFloat(char* cKeyName, float fValue) { Teardown::SetFloat(g_Game->m_Registry, new small_string(cKeyName), fValue); }
sledgelib_func float GetFloat(char* cKeyName) { return Teardown::GetFloat(g_Game->m_Registry, new small_string(cKeyName)); }

sledgelib_func void SetString(char* cKeyName, char* cValue) { Teardown::SetString(g_Game->m_Registry, new small_string(cKeyName), new small_string(cValue)); }
sledgelib_func void _GetString(char* cKeyName, char* cReturn, unsigned int iReturnBufferSize) {
	small_string ssReturn("null");
	Teardown::GetString(g_Game->m_Registry, &ssReturn, new small_string(cKeyName));
	if (ssReturn.len() > iReturnBufferSize) {
		cReturn[0] = '\0';
		return;
	}

	memcpy(cReturn, ssReturn.c_str(), ssReturn.len());
	cReturn[ssReturn.len()] = '\0';
}