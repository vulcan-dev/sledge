#include "sledge/luahelpers.h"
#include "net/sledgelib.h"
#include "teardown/functions/lua.h"

#define sledgelib_func extern "C" __declspec(dllexport)

sledgelib_func void _RegisterLuaFunctionInternal(const char* cFunctionName, void* pFunction) { Sledge::LuaHelpers::AddToRegisteredLuaFunctions(cFunctionName, pFunction); }

sledgelib_func int _lua_tointeger(lua_State* L, int iIndex) { return Teardown::lua_tointeger(L, iIndex); }
sledgelib_func bool _lua_toboolean(lua_State* L, int iIndex) { return Teardown::lua_toboolean(L, iIndex); }
sledgelib_func double _lua_tonumber(lua_State* L, int iIndex) { return Teardown::lua_tonumber(L, iIndex); }
sledgelib_func void* _lua_tolstring(lua_State* L, int iIndex) {
	size_t lLength = 0;
	const char* cLuaString = Teardown::lua_tolstring(L, iIndex, &lLength);
	char* pStringBuilder = reinterpret_cast<char*>(SledgeLib::Interface->AllocateString(lLength + 1));
	
	if (cLuaString)
		memcpy(pStringBuilder, cLuaString, lLength);
	
	pStringBuilder[lLength] = '\0';

	return pStringBuilder;
}

sledgelib_func void _lua_pushnil(lua_State* L) { Teardown::lua_pushnil(L); }
sledgelib_func void _lua_pushnumber(lua_State* L, double dValue) { Teardown::lua_pushnumber(L, dValue); }
sledgelib_func void _lua_pushinteger(lua_State* L, int iValue) { Teardown::lua_pushinteger(L, iValue); }
sledgelib_func void _lua_pushboolean(lua_State* L, bool bValue) { Teardown::lua_pushboolean(L, bValue); }
sledgelib_func void _lua_pushlstring(lua_State* L, const char* cValue) { Teardown::lua_pushlstring(L, cValue, strlen(cValue)); }