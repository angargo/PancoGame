#include <iostream>
#include <cassert>

#include "world.h"

World::World()
    : entities(),
      position_components(),
      speed_components(),
      render_components() {
};

World::~World() {}

const Component& World::getComponent(const Entity& entity,
                                     Component::Id component_id) const {
  assert(entity.components[component_id]);
  const int index = entity.component_indices[component_id];
  switch (component_id) {
    case Component::POSITION: return position_components[index]; break;
    case Component::SPEED: return speed_components[index]; break;
    case Component::RENDER: return render_components[index]; break;
    default: std::cerr << "Error: invalid component id\n"; exit(0); break;
  }
}

const Component& World::getComponent(id_type entity_id,
                                     Component::Id component_id) const {
  return getComponent(entities.at(entity_id), component_id);
}

Component& World::mutableComponent(const Entity &entity,
                                   Component::Id component_id) {
  return const_cast<Component&>(getComponent(entity, component_id));
}

Component& World::mutableComponent(id_type entity_id,
                                   Component::Id component_id) {
  return mutableComponent(entities.at(entity_id), component_id);
}
