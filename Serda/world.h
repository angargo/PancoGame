#ifndef SERDA_WORLD_H
#define SERDA_WORLD_H

#include <cassert>
#include <vector>
#include <unordered_map>

#include "entity.h"
#include "lua.hpp"

class World {
 private:
  // Class to iterate over values of entities.
  typedef std::unordered_map<id_type, Entity> entityMap;

  class value_iterator : public entityMap::iterator {
   public:
    value_iterator();
    value_iterator(entityMap::iterator e);
    Entity* operator->();
    Entity operator*();
  };

 public:
  // Range to allow for range loop in entities.
  struct Range {
    std::unordered_map<id_type, Entity>& entities;
    Range(std::unordered_map<id_type, Entity>& entities);
    value_iterator begin();
    value_iterator end();
  };

  // Range of the entity ids that the game will use.
  static const id_type DYNAMIC_ID_RANGE_FROM;
  static const id_type DYNAMIC_ID_RANGE_TO;

  // Constructors and destructors.
  World();
  ~World();

  // Reset the world.
  void reset();

  // Link handlers.
  int linkId() const;
  int& variableLinkId();

  // Register functions that are going to be called from Lua.
  void registerLuaFunctions();
  lua_State* getL() { return L; }

  // Entity operations.
  void createEntity(id_type entity_id);
  id_type createEntity();

  const Entity& getEntity(id_type entity_id) const;
  Range getEntities();

  const sf::Vector2f& lowerBounds() const;
  sf::Vector2f& variableLowerBounds();
  const sf::Vector2f& upperBounds() const;
  sf::Vector2f& variableUpperBounds();


  // Compenent queries.
  template <typename C>
  bool has(const Entity& entity) const {
    return entity.has<C>();
  }
  template <typename C>
  bool has(id_type entity_id) const {
    return has<C>(entities.at(entity_id));
  }

  // TODO: implement these methods.
  template <typename C>
  bool isActive(const Entity& entity) const;
  template <typename C>
  bool isActive(id_type entity_id) const;

  template <typename C>
  void activate(const Entity& entity) const;
  template <typename C>
  void activate(id_type entity_id) const;

  template <typename C>
  void deactivate(const Entity& entity) const;
  template <typename C>
  void deactivate(id_type entity_id) const;

  // Component gettters.
  template <typename C>
  const C& get(const Entity& entity) const {
    assert(has<C>(entity));
    return getVect<C>()[entity.component_indices[Id<C>()]];
  }
  template <typename C>
  const C& get(id_type entity_id) const {
    return get<C>(entities.at(entity_id));
  }

  template <typename C>
  C& variable(const Entity& entity) {
    return const_cast<C&>(get<C>(entity));
  }
  template <typename C>
  C& variable(id_type entity_id) {
    return variable<C>(entities.at(entity_id));
  }

  // Add a component to an entity.
  template <typename C>
  void add(id_type entity_id, C component) {
    component.entity_id = entity_id;
    Entity& entity = entities.at(entity_id);
    assert(not has<C>(entity));
    entity.component_indices[Id<C>()] = getVect<C>().size();
    entity.components.set(Id<C>(), true);
    mutVect<C>().push_back(component);
  }

 private:
  // Returns an random entityId not being used.
  id_type getRandomEntityId();

  // Container access.
  template <typename C>
  const std::vector<C>& getVect() const;
  template <typename C>
  std::vector<C>& mutVect() {
    return const_cast<std::vector<C>&>(getVect<C>());
  }

  id_type link_id;

  lua_State* L;

  // TODO: cache problems, maybe.
  std::unordered_map<id_type, Entity> entities;

  Range range;

  sf::Vector2f lower_bounds;
  sf::Vector2f upper_bounds;;

  // Components go here.
  std::vector<PositionComponent> position_components;
  std::vector<SpeedComponent> speed_components;
  std::vector<RenderComponent> render_components;
  std::vector<AnimComponent> anim_components;
  std::vector<InputComponent> input_components;
  std::vector<LogicComponent> logic_components;
};

#endif  // SERDA_WORLD_H
