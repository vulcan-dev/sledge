#pragma once

#include "teardown/types/small_string.h"

#pragma pack(push, 1)
class ScriptCore_LuaState
{
public:
	char pad_0000[56]; //0x0000
	class lua_State** m_LuaState; //0x0038
}; //Size: 0x0040

class ScriptCore
{
public:
	float m_RunTime; //0x0008
	char pad_000C[4]; //0x000C
	small_string m_ScriptPath; //0x0010
	small_string m_ScriptLocation; //0x0020
	ScriptCore_LuaState m_SCLS; //0x0030
	bool m_ScriptLoaded; //0x0070
	bool m_HasInitCallback; //0x0071
	bool m_HasTickCallback; //0x0072
	bool m_HasUpdateCallback; //0x0073
	bool m_HasDrawCallback; //0x0074
	bool m_HasCommandHandler; //0x0075
	char pad_0076[2]; //0x0076
	float m_RunTime2; //0x0078
	float m_UpdateRunTime; //0x007C
	char pad_0080[3200]; //0x0080

	virtual void Destroy(bool bFree) = 0;
	virtual void RegisterLuaFunctions() = 0;
	virtual void LoadScript(small_string* ssScriptName) = 0;
	virtual void Reset() = 0;
	virtual void Init() = 0;
	virtual void Tick(float fTimeStep, void* pUnknown) = 0;
	virtual void Update(float fTimeStep) = 0;
	virtual void Function7(void*) = 0;
	virtual void Function8(void*) = 0;
}; //Size: 0x0D00

#pragma pack(pop)