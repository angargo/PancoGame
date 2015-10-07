#include "resource_manager.h"

LuaManager::LuaManager(const std::string& filename) {
  loadDictionary(filename);
}

void LuaManager::loadDictionary(const std::string& filename) {
  std::ifstream in(filename);
  if (!in.good()) {
    throw std::runtime_error("ResourceManager - cannot load dictionary " +
        filename);
  }
  std::string line;
  while (getline(in, line)) {
    std::stringstream ss(line);
    int script_id;
    std::string file;
    ss >> script_id >> file;
    files[script_id] = file;
  }
}

void LuaManager::loadScript(lua_State* L, int script_id) {
  // We can choose to load the script anyway.
  const auto it = files.find(script_id);
  if (it == files.end()) {
    throw std::runtime_error("LuaManager - Cannot find script " +
        std::to_string(script_id));
  }

  const std::string& filename = it->second;
  if (loaded.count(filename) == 0 or true) {
    if (luaL_dofile(L, filename.c_str())) {
      std::cerr << lua_tostring(L, -1) << std::endl;
      lua_pop(L, 1);
    }
    else loaded.insert(filename);
  }
}
