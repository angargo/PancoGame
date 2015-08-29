#include "world.h"
#include "lua.hpp"

namespace {
World* world;
}

void World::registerLuaFunctions() {
  world = this;

  L = luaL_newstate();
  luaL_openlibs(L);

  lua_register(L, "sayHelloWorld", [](lua_State* L) -> int {
    std::cout << "Hello World!" << std::endl;
    return 0;
  });

  // Parameters: entity_id, x, y.
  // Add ('x','y') to speed of entity 'entity_id'.
  lua_register(L, "addSpeed", [](lua_State* L) -> int {
    id_type entity_id = luaL_checknumber(L, 1);
    int x = luaL_checknumber(L, 2);
    int y = luaL_checknumber(L, 3);
    world->variable<SpeedComponent>(entity_id).addSpeed(x, y);
    return 0;
  });
}
