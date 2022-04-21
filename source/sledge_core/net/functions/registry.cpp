#include "util/log.h"

#include "net/sledgelib.h"

#include "teardown/classes/game.h"
#include "teardown/types/small_string.h"
#include "teardown/functions/registry.h"

#define sledgelib_func extern "C" __declspec(dllexport)

sledgelib_func void SetBool(char* cKeyName, bool bValue) { Teardown::SetBool(g_Game->m_Registry, new small_string(cKeyName), bValue); }
sledgelib_func bool GetBool(char* cKeyName) { return Teardown::GetBool(g_Game->m_Registry, new small_string(cKeyName)); }

sledgelib_func void SetInt(char* cKeyName, int iValue) { Teardown::SetInt(g_Game->m_Registry, new small_string(cKeyName), iValue); }
sledgelib_func int GetInt(char* cKeyName) { return Teardown::GetInt(g_Game->m_Registry, new small_string(cKeyName)); }

sledgelib_func void SetFloat(char* cKeyName, float fValue) { Teardown::SetFloat(g_Game->m_Registry, new small_string(cKeyName), fValue); }
sledgelib_func float GetFloat(char* cKeyName) { return Teardown::GetFloat(g_Game->m_Registry, new small_string(cKeyName)); }

sledgelib_func void SetString(char* cKeyName, char* cValue) { Teardown::SetString(g_Game->m_Registry, new small_string(cKeyName), new small_string(cValue)); }
sledgelib_func void* _GetString(char* cKeyName) {
	/*
	TO-DO:	the value returned by GetString doesn't get disposed which results in a memory leak
			calling the destructor causes a heap corruption,
	*/
	small_string* ssReturn = new small_string("null");
	small_string* ssKeyName = new small_string(cKeyName);
	Teardown::GetString(g_Game->m_Registry, ssReturn, ssKeyName);

	size_t lRetLen = ssReturn->len();

	char* pStringBuilder = reinterpret_cast<char*>(SledgeLib::Interface->AllocateString(lRetLen + 1));
	memcpy(pStringBuilder, ssReturn->c_str(), lRetLen);
	pStringBuilder[lRetLen] = '\0';

	delete ssReturn;
	delete ssKeyName;

	return pStringBuilder;
}