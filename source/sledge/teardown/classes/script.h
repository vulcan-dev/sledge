#pragma once
#include "teardown/small_string.h"

class CScriptCore_LuaStateInfo
{
public:
	void* m_LuaState; //0x0000
	char pad_0008[32]; //0x0008
}; //Size: 0x0028

class CScriptCore_LuaState
{
public:
	char pad_0000[40]; //0x0000
	class CScriptCore* m_ScriptCore; //0x0028
	char pad_0030[8]; //0x0030
	CScriptCore_LuaStateInfo* m_LuaStateInfo; //0x0038
}; //Size: 0x0040

class CScriptCore
{
public:
	float m_RunTime; //0x0008
	char pad_000C[4]; //0x000C
	class small_string m_ScriptName; //0x0010
	class small_string m_ScriptPath; //0x0020
	class CScriptCore_LuaState m_LuaState; //0x0030
	char pad_0070[32]; //0x0070
	float m_RunTime2; //0x0090
	char pad_0094[300]; //0x0094
	void* N00000EDC; //0x01C0
	char pad_01C8[200]; //0x01C8

	virtual void Destroy() = 0;
	virtual void RegisterLuaFunctions() = 0;
	virtual void LoadScript() = 0;
	virtual void Function3() = 0;
	virtual void Init() = 0;
	virtual void Tick() = 0;
	virtual void Update() = 0;
	virtual void Function7() = 0;
	virtual void Function8() = 0;
}; //Size: 0x0290