#include <iostream>
#include <cassert>

#include "world.h"

const id_type World::DYNAMIC_ID_RANGE_FROM = 1u << 25;
const id_type World::DYNAMIC_ID_RANGE_TO = (1u << 31) -1;

World::World()
    : entities(),
      range(entities),
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

void World::createEntity(id_type entity_id) {
  entities.emplace(entity_id, Entity(entity_id));
}

id_type World::createEntity() {
  id_type id = getRandomEntityId();
  createEntity(id);
  return id;
}

World::Range World::getEntities() {
  return range;
}

id_type World::getRandomEntityId() {
  std::default_random_engine generator;
  std::uniform_int_distribution<id_type> distribution(DYNAMIC_ID_RANGE_FROM,
                                                      DYNAMIC_ID_RANGE_TO);
  id_type id = DYNAMIC_ID_RANGE_FROM;
  while (entities.find(id) != entities.end()) {
    id = distribution(generator);
  }
  return id;
}
