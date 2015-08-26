#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <cassert>
#include <exception>
#include <fstream>
#include <memory>
#include <sstream>
#include <unordered_map>

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
  ResourceManager(const std::string& filename) { loadDictionary(filename); }
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

typedef ResourceManager<int, sf::Texture> TextureManager;
typedef ResourceManager<int, sf::Font> FontManager;

#endif  // RESOURCE_MANAGER_H