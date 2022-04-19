#pragma once

typedef int (*lua_CFunction) (class lua_State* L);

// from: lua.h
#define LUA_GLOBALSINDEX	(-10002)
#define lua_upvalueindex(i)	(LUA_GLOBALSINDEX-(i))

namespace Teardown {
	typedef void (*tlua_pushlightuserdata) (class lua_State* L, void* p);
	inline tlua_pushlightuserdata lua_pushlightuserdata;

	typedef const void* (*tlua_topointer) (class lua_State* L, int index);
	inline tlua_topointer lua_topointer;

	typedef int (*tlua_gettop) (class lua_State* L);
	inline tlua_gettop lua_gettop;

	typedef void (*tlua_pushcclosure) (class lua_State* L, int (*fn) (class lua_State* L), int n);
	inline tlua_pushcclosure lua_pushcclosure;

	typedef void (*tlua_setfield) (class lua_State* L, int index, const char* k);
	inline tlua_setfield lua_setfield;

	typedef void (*tlua_pushnil) (class lua_State* L);
	inline tlua_pushnil lua_pushnil;

	typedef int (*tlua_tointeger) (class lua_State* L, int index);
	inline tlua_tointeger lua_tointeger;

	typedef bool (*tlua_toboolean) (class lua_State* L, int index);
	inline tlua_toboolean lua_toboolean;

	typedef const char* (*tlua_tolstring) (class lua_State* L, int index, size_t* len);
	inline tlua_tolstring lua_tolstring;
}