#include "luahelpers.h"

#include "teardown/functions/lua.h"
#include "teardown/classes/scriptcore.h"

#include "util/log.h"

#include <vector>
#include <mutex>

struct SRegistrableFunction {
	char* m_FunctionName = nullptr;
	void* m_Function = nullptr;

	SRegistrableFunction(const char* cFunctionName, void* pFunction) {
		size_t lNameLen = strlen(cFunctionName);

		m_FunctionName = reinterpret_cast<char*>(malloc(lNameLen + 1));
		
		if (m_FunctionName == nullptr)
			return;

		memcpy(m_FunctionName, cFunctionName, lNameLen);
		m_FunctionName[lNameLen] = '\0';
	
		m_Function = pFunction;
	}
};

std::mutex FuncLock;
std::vector <SRegistrableFunction*> vFunctions = {};

int InvokeLuaFunction(class lua_State* L) {
	tLuaFunction pFunction = reinterpret_cast<tLuaFunction>(Teardown::lua_topointer(L, lua_upvalueindex(1)));
	const void* pScriptCore = Teardown::lua_topointer(L, lua_upvalueindex(2));
	const char* cFunctionName = reinterpret_cast<const char*>(Teardown::lua_topointer(L, lua_upvalueindex(3)));
	return pFunction(reinterpret_cast<const ScriptCore*>(pScriptCore), L, cFunctionName);
}

void RegisterLuaFunction(class ScriptCore* pSC, char* cFunctionName, void* pFunction) {
	Teardown::lua_pushlightuserdata(*pSC->m_SCLS.m_LuaState, pFunction);
	Teardown::lua_pushlightuserdata(*pSC->m_SCLS.m_LuaState, pSC);
	Teardown::lua_pushlightuserdata(*pSC->m_SCLS.m_LuaState, reinterpret_cast<void*>(cFunctionName));
	Teardown::lua_pushcclosure(*pSC->m_SCLS.m_LuaState, InvokeLuaFunction, 3);

	Teardown::lua_setfield(*pSC->m_SCLS.m_LuaState, LUA_GLOBALSINDEX, cFunctionName);
}

void Sledge::LuaHelpers::RegisterLuaFunctions(class ScriptCore* pSC) {
	FuncLock.lock();
	for (SRegistrableFunction* Func : vFunctions)
		RegisterLuaFunction(pSC, Func->m_FunctionName, Func->m_Function);
	FuncLock.unlock();
}

void Sledge::LuaHelpers::AddToRegisteredLuaFunctions(const char* cFunctionName, void* pFunction) {
	FuncLock.lock();
	bool bHasFunc = std::any_of(vFunctions.begin(), vFunctions.end(), [cFunctionName](SRegistrableFunction* Func) -> bool { return (strcmp(Func->m_FunctionName, cFunctionName) == 0); });

	if (bHasFunc)
		LogVerbose("Attempted to re-register function {0}, skipping.", cFunctionName);
	else 
		vFunctions.push_back(new SRegistrableFunction(cFunctionName, pFunction));

	FuncLock.unlock();
}
void Sledge::LuaHelpers::RemoveRegisteredLuaFunction(const char* cFunctionName) {
	FuncLock.lock();
	vFunctions.erase(std::remove_if(vFunctions.begin(), vFunctions.end(), [cFunctionName](SRegistrableFunction* Func) -> bool { return (strcmp(Func->m_FunctionName, cFunctionName) == 0); }), vFunctions.end());
	FuncLock.unlock();
}