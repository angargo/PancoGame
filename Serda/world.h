#ifndef SERDA_WORLD_H
#define SERDA_WORLD_H

#include <vector>
#include <unordered_map>

#include "entity.h"

class World {
public:
  World();
  ~World();

  const Component& getComponent(const Entity& entity,
                                Component::Id component_id) const;
  const Component& getComponent(id_type entity_id,
                                Component::Id component_id) const;
  Component& mutableComponent(const Entity& entity, Component::Id component_id);
  Component& mutableComponent(id_type entity_id, Component::Id component_id);

private:
  // TODO: cache problems, maybe.
  std::unordered_map<id_type, Entity> entities;

  // Components go here.
  std::vector<PositionComponent> position_components;
  std::vector<SpeedComponent> speed_components;
  std::vector<RenderComponent> render_components;
};

#endif  // SERDA_WORLD_H
