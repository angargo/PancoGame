#include "world.h"
#include "lua.hpp"

void World::registerLuaFunctions(lua_State* L) {
  World* world(this);

  L = luaL_newstate();
  luaL_openlibs(L);

  lua_register(L, "sayHelloWorld", [](lua_State* L) -> int {
    std::cout << "Hello World!" << std::endl;
    return 0;
  });
}
