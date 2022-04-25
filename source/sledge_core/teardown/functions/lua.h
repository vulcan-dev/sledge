#pragma once

typedef int (*lua_CFunction) (class lua_State* L);

// from: lua.h
#define LUA_GLOBALSINDEX	(-10002)
#define lua_upvalueindex(i)	(LUA_GLOBALSINDEX-(i))

class lua_State;

namespace Teardown {
	inline void (*lua_setfield) (lua_State* L, int index, const char* k);

	inline int (*lua_gettop) (lua_State* L);

	inline const void* (*lua_topointer) (lua_State* L, int index);
	inline int (*lua_tointeger) (lua_State* L, int index);
	inline bool (*lua_toboolean) (lua_State* L, int index);
	inline const char* (*lua_tolstring) (class lua_State* L, int index, size_t* len);
	inline double (*lua_tonumber) (class lua_State* L, int index);

	inline void (*lua_pushlightuserdata) (lua_State* L, void* p);
	inline void (*lua_pushcclosure) (lua_State* L, int (*fn) (class lua_State* L), int n);
	inline void (*lua_pushnil) (lua_State* L);
	inline void (*lua_pushnumber) (lua_State* L, double n);
	inline void (*lua_pushinteger) (lua_State* L, int n);
	inline void (*lua_pushlstring) (lua_State* L, const char* s, size_t len);
	inline void (*lua_pushboolean) (lua_State* L, int b);

	inline void (*lua_createtable) (lua_State* L, int narr, int nrec);
}