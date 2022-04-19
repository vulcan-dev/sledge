#pragma once
typedef int (*tLuaFunction)(const class ScriptCore* pSC, class lua_State* L, const char* cFunctionName);

namespace Sledge {
	namespace LuaHelpers {
		void RegisterLuaFunctions(class ScriptCore* pSC);
		void AddToRegisteredLuaFunctions(const char* cFunctionName, void* pFunction);
	}
}