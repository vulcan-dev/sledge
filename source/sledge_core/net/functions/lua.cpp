#include "sledge/luahelpers.h"
#include "net/sledgelib.h"
#include "teardown/functions/lua.h"

#define sledgelib_func extern "C" __declspec(dllexport)

sledgelib_func void _RegisterLuaFunctionInternal(const char* cFunctionName, void* pFunction) { Sledge::LuaHelpers::AddToRegisteredLuaFunctions(cFunctionName, pFunction); }

sledgelib_func int _lua_tointeger(lua_State* L, int iIndex) { return Teardown::lua_tointeger(L, iIndex); }
sledgelib_func bool _lua_toboolean(lua_State* L, int iIndex) { return Teardown::lua_toboolean(L, iIndex); }

sledgelib_func void* _lua_tolstring(lua_State* L, int iIndex) {
	size_t lLength = 0;
	const char* cLuaString = Teardown::lua_tolstring(L, iIndex, &lLength);
	char* pStringBuilder = reinterpret_cast<char*>(SledgeLib::Interface->AllocateString(lLength + 1));
	
	if (cLuaString)
		memcpy(pStringBuilder, cLuaString, lLength);
	
	pStringBuilder[lLength] = '\0';

	return pStringBuilder;
}