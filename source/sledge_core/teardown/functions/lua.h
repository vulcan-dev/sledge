#pragma once

typedef int (*lua_CFunction) (class lua_State* L);

// from: lua.h
#define LUA_GLOBALSINDEX	(-10002)
#define lua_upvalueindex(i)	(LUA_GLOBALSINDEX-(i))

class lua_State;

namespace Teardown {
	inline void (*lua_pushlightuserdata) (lua_State* L, void* p);
	inline const void* (*lua_topointer) (lua_State* L, int index);
	inline int (*lua_gettop) (lua_State* L);
	inline void (*lua_pushcclosure) (lua_State* L, int (*fn) (class lua_State* L), int n);
	inline void (*lua_setfield) (lua_State* L, int index, const char* k);
	inline void (*lua_pushnil) (lua_State* L);
	inline int (*lua_tointeger) (lua_State* L, int index);
	inline bool (*lua_toboolean) (lua_State* L, int index);
	inline const char* (*lua_tolstring) (class lua_State* L, int index, size_t* len);
}