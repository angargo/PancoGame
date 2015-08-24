#include <iostream>
#include <cassert>

#include "world.h"

const id_type World::DYNAMIC_ID_RANGE_FROM = 1u << 25;
const id_type World::DYNAMIC_ID_RANGE_TO = (1u << 31) -1;

// World iterator for entities.
World::value_iterator::value_iterator() : World::entityMap::iterator() {}
World::value_iterator::value_iterator(World::entityMap::iterator e)
    : entityMap::iterator(e) {};
Entity* World::value_iterator::operator->() {
  return (Entity* const) & (entityMap::iterator::operator->()->second);
}
Entity World::value_iterator::operator*() {
  return entityMap::iterator::operator*().second;
}

// World Range struct to iterate over entities with for range loop.
World::Range::Range(std::unordered_map<id_type, Entity> &entities)
    : entities(entities) {}
World::value_iterator World::Range::begin() { return entities.begin(); }
World::value_iterator World::Range::end() { return entities.end(); }

// World class.
World::World()
    : entities(),
      range(entities),
      position_components(),
      speed_components(),
      render_components() {
};

World::~World() {}


void World::createEntity(id_type entity_id) {
  entities.emplace(entity_id, Entity(entity_id));
}
id_type World::createEntity() {
  id_type id = getRandomEntityId();
  createEntity(id);
  return id;
}

const Entity& World::getEntity(id_type entity_id) const {
  return entities.at(entity_id);
}
World::Range World::getEntities() {
  return range;
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

void World::addComponent(id_type entity_id, Component* c) {
  // Try all different components.
  if (auto pc = dynamic_cast<PositionComponent*>(c)) {
    addComponent(entity_id, *pc);
  } else if (auto sc = dynamic_cast<SpeedComponent*>(c)) {
    addComponent(entity_id, *sc);
  } else if (auto rc = dynamic_cast<RenderComponent*>(c)) {
    addComponent(entity_id, *rc);
  } else if (auto ic = dynamic_cast<InputComponent*>(c)) {
    addComponent(entity_id, *ic);
  } else {
    std::cerr << "Error adding component to world: Bad component" << std::endl;
  }
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

template<> const std::vector<PositionComponent>&
  World::vect<PositionComponent>() const { return position_components; }

template<> const std::vector<SpeedComponent>&
  World::vect<SpeedComponent>() const { return speed_components; }

template<> const std::vector<RenderComponent>&
  World::vect<RenderComponent>() const { return render_components; }

template<> const std::vector<InputComponent>&
  World::vect<InputComponent>() const { return input_components; }

template<typename C> const std::vector<C>& World::vect() const {
 std::cerr << "World::vect with unknown type" << std::endl;
 exit(0);
}
