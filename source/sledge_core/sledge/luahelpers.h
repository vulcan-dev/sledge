#pragma once

namespace Sledge {
	namespace LuaHelpers {
		void RegisterLuaFunctions(class ScriptCore* pSC);
		void AddToRegisteredLuaFunctions(const char* cFunctionName, void* pFunction);
	}
}