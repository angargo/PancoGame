#ifndef SERDA_WORLD_H
#define SERDA_WORLD_H

#include <vector>
#include <unordered_map>

#include "entity.h"

class World {
public:
  static const id_type DYNAMIC_ID_RANGE_FROM;
  static const id_type DYNAMIC_ID_RANGE_TO;
  World();
  ~World();

  const Component& getComponent(const Entity& entity,
                                Component::Id component_id) const;
  const Component& getComponent(id_type entity_id,
                                Component::Id component_id) const;
  Component& mutableComponent(const Entity& entity, Component::Id component_id);
  Component& mutableComponent(id_type entity_id, Component::Id component_id);

  void createEntity(id_type entity_id);
  id_type createEntity();

private:
  // Class to iterate over values of entities.
  typedef std::unordered_map<id_type, Entity> entityMap;

  class value_iterator : public entityMap::iterator {
  public:
    value_iterator() : entityMap::iterator() {}
    value_iterator(entityMap::iterator e) : entityMap::iterator(e) {};
    Entity* operator->() {
      return (Entity* const) & (entityMap::iterator::operator->()->second);
    }
    Entity operator*() { return entityMap::iterator::operator*().second; }
  };

public:
  // Range to allow for range loop in entities.
  struct Range {
    std::unordered_map<id_type, Entity>& entities;
    Range(std::unordered_map<id_type, Entity>& entities) : entities(entities) {}
    value_iterator begin() { return entities.begin(); }
    value_iterator end() { return entities.end(); }
  };

  Range getEntities();

private:

  // Returns an random entityId not being used.
  id_type getRandomEntityId();

  // TODO: cache problems, maybe.
  std::unordered_map<id_type, Entity> entities;

  Range range;

  // Components go here.
  std::vector<PositionComponent> position_components;
  std::vector<SpeedComponent> speed_components;
  std::vector<RenderComponent> render_components;
};

#endif  // SERDA_WORLD_H
