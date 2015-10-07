#ifndef SERDA_LUA_HPP
#define SERDA_LUA_HPP

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

inline void luaRegister(lua_State* L, const char* n, lua_CFunction f) {
  lua_pushcfunction(L, f);
  lua_setglobal(L, n);
}

#endif
