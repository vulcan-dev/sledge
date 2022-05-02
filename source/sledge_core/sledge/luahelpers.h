#pragma once

class ScriptCore;
struct lua_State;

typedef int (*tLuaFunction)(const ScriptCore* pSC, lua_State* L, const char* cFunctionName);

namespace Sledge {
	namespace LuaHelpers {
		void RegisterLuaFunctions(ScriptCore* pSC);
		void AddToRegisteredLuaFunctions(const char* cFunctionName, void* pFunction);
	}
}