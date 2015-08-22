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


bool World::hasComponent(const Entity& entity, Component::Id cid) const {
  return entity.component_indices[cid] != -1;
}
bool World::hasComponent(id_type entity_id, Component::Id cid) const {
  return entities.at(entity_id).component_indices[cid] != -1;
}

// Getters.
const Component& World::getComponent(const Entity& entity,
                                     Component::Id component_id) const {
  assert(hasComponent(entity, component_id));
  const int index = entity.component_indices[component_id];
  switch (component_id) {
    case Component::POSITION: return position_components[index]; break;
    case Component::SPEED: return speed_components[index]; break;
    case Component::RENDER: return render_components[index]; break;
    case Component::INPUT: return input_components[index]; break;
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


// PositionComponent getters.
const PositionComponent& World::getPositionComponent(const Entity& ent) const {
  static const Component::Id cid = Component::POSITION;
  assert(hasComponent(ent, cid));
  return position_components[ent.component_indices[cid]];
}
const PositionComponent& World::getPositionComponent(id_type entity_id) const {
  return getPositionComponent(entities.at(entity_id));
}
PositionComponent& World::mutablePositionComponent(const Entity& entity) {
  return const_cast<PositionComponent&>(getPositionComponent(entity));
}
PositionComponent& World::mutablePositionComponent(id_type entity_id) {
  return mutablePositionComponent(entities.at(entity_id));
}

// SpeedComponent getters.
const SpeedComponent& World::getSpeedComponent(const Entity& entity) const {
  static const Component::Id cid = Component::SPEED;
  assert(hasComponent(entity, cid));
  return speed_components[entity.component_indices[cid]];
}
const SpeedComponent& World::getSpeedComponent(id_type entity_id) const {
  return getSpeedComponent(entities.at(entity_id));
}
SpeedComponent& World::mutableSpeedComponent(const Entity& entity) {
  return const_cast<SpeedComponent&>(getSpeedComponent(entity));
}
SpeedComponent& World::mutableSpeedComponent(id_type entity_id) {
  return mutableSpeedComponent(entities.at(entity_id));
}

// RenderComponent getters.
const RenderComponent& World::getRenderComponent(const Entity& entity) const {
  static const Component::Id cid = Component::RENDER;
  assert(hasComponent(entity, cid));
  return render_components[entity.component_indices[cid]];
}
const RenderComponent& World::getRenderComponent(id_type entity_id) const {
  return getRenderComponent(entities.at(entity_id));
}
RenderComponent& World::mutableRenderComponent(const Entity& entity) {
  return const_cast<RenderComponent&>(getRenderComponent(entity));
}
RenderComponent& World::mutableRenderComponent(id_type entity_id) {
  return mutableRenderComponent(entities.at(entity_id));
}

// InputComponent getters.
const InputComponent& World::getInputComponent(const Entity& entity) const {
  static const Component::Id cid = Component::INPUT;
  assert(hasComponent(entity, cid));
  return input_components[entity.component_indices[cid]];
}
const InputComponent& World::getInputComponent(id_type entity_id) const {
  return getInputComponent(entities.at(entity_id));
}
InputComponent& World::mutableInputComponent(const Entity& entity) {
  return const_cast<InputComponent&>(getInputComponent(entity));
}
InputComponent& World::mutableInputComponent(id_type entity_id) {
  return mutableInputComponent(entities.at(entity_id));
}


void World::addComponent(id_type entity_id, PositionComponent pc) {
  pc.entity_id = entity_id;
  assert(entities.find(entity_id) != entities.end());
  Entity& entity = entities.at(entity_id);
  entity.component_indices[Component::POSITION] = position_components.size();
  entity.components.set(Component::POSITION, true);
  position_components.push_back(pc);
}

void World::addComponent(id_type entity_id, SpeedComponent sc) {
  sc.entity_id = entity_id;
  assert(entities.find(entity_id) != entities.end());
  Entity& entity = entities.at(entity_id);
  entity.component_indices[Component::SPEED] = speed_components.size();
  entity.components.set(Component::SPEED, true);
  speed_components.push_back(sc);
}

void World::addComponent(id_type entity_id, RenderComponent rc) {
  rc.entity_id = entity_id;
  assert(entities.find(entity_id) != entities.end());
  Entity& entity = entities.at(entity_id);
  entity.component_indices[Component::RENDER] = render_components.size();
  entity.components.set(Component::RENDER, true);
  render_components.push_back(rc);
}

void World::addComponent(id_type entity_id, InputComponent ic) {
  ic.entity_id = entity_id;
  assert(entities.find(entity_id) != entities.end());
  Entity& entity = entities.at(entity_id);
  entity.component_indices[Component::INPUT] = input_components.size();
  entity.components.set(Component::INPUT, true);
  input_components.push_back(ic);
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
