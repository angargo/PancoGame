#include "resource_manager.h"

#include <iostream>

LuaManager::LuaManager(const std::string& filename) {
  loadDictionary(filename);
}

void LuaManager::loadDictionary(const std::string& filename) {}

void LuaManager::runScript(lua_State* L, int script_id, int entity_id) {
  // We can choose to load the script anyway.
  if (loaded.count(script_id) == 0) {
    if (files.count(script_id) == 0) {
      throw std::runtime_error("LuaManager - Cannot find script " +
                               std::to_string(script_id));
    }
    if (luaL_dofile(L, files[script_id].c_str())) {
      std::cerr << lua_tostring(L, -1) << std::endl;
      lua_pop(L, 1);
    }
    else loaded.insert(script_id);
  }

  lua_getglobal(L, "game");
  lua_getfield(L, -1, std::to_string(script_id).c_str());
  lua_pushinteger(L, entity_id);
  if (lua_pcall(L, 1, 0, 0)) {
    std::cerr << lua_tostring(L, -1) << std::endl;
    lua_pop(L, 1);
  }
}
