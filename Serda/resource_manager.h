#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <cassert>
#include <exception>
#include <fstream>
#include <iostream>
#include <memory>
#include <set>
#include <sstream>
#include <unordered_map>

#include "lua.hpp"
#include <SFML/Graphics.hpp>

namespace Fonts {
enum {
  Menu = 0,
  NumFonts,
};
}

template <class K, class T>
class ResourceManager {
 public:
  ResourceManager() {}
  explicit ResourceManager(const std::string& filename) {
    loadDictionary(filename);
  }
  ~ResourceManager() {}

  T* get(const K& key) {
    auto it = resources.find(key);
    if (it == resources.end()) {
      throw std::runtime_error("ResourceManager::get - no resource found");
    }
    return it->second.get();
  }

  const T* get(const K& key) const {
    auto it = resources.find(key);
    if (it == resources.end()) {
      throw std::runtime_error("ResourceManager::get - no resource found");
    }
    return it->second.get();
  }

  void load(const K& key, const std::string& filename) {
    std::unique_ptr<T> resource(new T());
    if (!resource->loadFromFile(filename)) {
      throw std::runtime_error("ResourceManager::load - cannot load file " +
                               filename);
    }
    auto it = resources.insert(std::make_pair(key, std::move(resource)));
    assert(it.second);
  }

 private:
  void loadDictionary(const std::string& filename) {
    std::ifstream in(filename);
    if (!in.good()) {
      throw std::runtime_error("ResourceManager - cannot load dictionary " +
                               filename);
    }
    std::string line;
    while (getline(in, line)) {
      std::stringstream ss(line);
      K key;
      std::string file;
      ss >> key >> file;
      load(key, file);
    }
  }

  std::unordered_map<K, std::unique_ptr<T>> resources;
};

typedef ResourceManager<int, sf::Image> ImageManager;
typedef ResourceManager<int, sf::Font> FontManager;

class LuaManager {
  public:
    explicit LuaManager(const std::string& filename);
    // TODO: if entity_id changes, change this.
    void loadScript(lua_State* L, int script_id);

    void callScript(lua_State* L, int nargs) {
      if (lua_pcall(L, nargs, 0, 0)) {
        std::cerr << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);
      }
    }

    template<typename... Args>
    void callScript(lua_State* L, int nargs, int p, Args... args) {
      lua_pushinteger(L, p);
      callScript(L, nargs + 1, args...);
    }
    template<typename... Args>
    void callScript(lua_State* L, int nargs, bool p, Args... args) {
      lua_pushboolean(L, p);
      callScript(L, nargs + 1, args...);
    }

    template<typename... Args>
    void runScript(lua_State* L, int script_id, Args... args) {
      loadScript(L, script_id);

      lua_getglobal(L, "game");
      lua_getfield(L, -1, std::to_string(script_id).c_str());

      callScript(L, 0, args...);
    }

    void loadDictionary(const std::string& filename);

  private:
    std::unordered_map<int, std::string> files;
    std::set<int> loaded;
};

#endif  // RESOURCE_MANAGER_H
