#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <exception>
#include <iostream>
#include <memory>
#include <unordered_map>

template <class K, class T> class ResourceManager {
public:
  ResourceManager() {}
  ~ResourceManager() {}

  void load(const K& key, const std::string& filename) {
    std::unique_ptr<T> resource(new T());
    if (!resource->loadFromFile(filename)) {
      throw std::runtime_error("ResourceManager::load - cannot load file " +
                               filename);
    }
    auto it = resources.insert(std::make_pair(key, std::move(resource)));
    assert(it->second);
  }

  T& get(const K& key) {
    auto it = resources.find(key);
    if (it == resources.end()) {
      throw std::runtime_error("ResourceManager::get - no resource found");
    }
    return *it->second;
  }

  const T& get(const K& key) const {
    auto it = resources.find(key);
    if (it == resources.end()) {
      throw std::runtime_error("ResourceManager::get - no resource found");
    }
    return *it->second;
  }

private:
  std::unordered_map<K, std::unique_ptr<T>> resources;
};

#endif  // RESOURCE_MANAGER_H
