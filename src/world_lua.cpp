#include "world.h"

#include <iostream>

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

  // Parameters: entity_id.
  // Return the x, y, z coordinates of the entity.
  lua_register(L, "getPosition", [](lua_State* L) -> int {
    id_type entity_id = luaL_checknumber(L, 1);
    const auto& pos = world->get<PositionComponent>(entity_id);
    lua_pushnumber(L, pos.x);
    lua_pushnumber(L, pos.y);
    lua_pushnumber(L, pos.z);
    return 3;
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

  // Parameters: entity_id, x, y.
  // Set ('x','y') to speed of entity 'entity_id'.
  lua_register(L, "setSpeed", [](lua_State* L) -> int {
    id_type entity_id = luaL_checknumber(L, 1);
    int x = luaL_checknumber(L, 2);
    int y = luaL_checknumber(L, 3);
    world->variable<SpeedComponent>(entity_id).setSpeed(x, y);
    return 0;
  });
}
