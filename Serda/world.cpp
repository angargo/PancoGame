#include <iostream>
#include <cassert>

#include "world.h"

const id_type World::DYNAMIC_ID_RANGE_FROM = 1u << 25;
const id_type World::DYNAMIC_ID_RANGE_TO = (1u << 31) - 1;

// World iterator for entities.
World::value_iterator::value_iterator() : World::entityMap::iterator() {}
World::value_iterator::value_iterator(World::entityMap::iterator e)
    : entityMap::iterator(e){};
Entity* World::value_iterator::operator->() {
  return (Entity * const) & (entityMap::iterator::operator->()->second);
}
Entity World::value_iterator::operator*() {
  return entityMap::iterator::operator*().second;
}

// World Range struct to iterate over entities with for range loop.
World::Range::Range(std::unordered_map<id_type, Entity>& entities)
    : entities(entities) {}
World::value_iterator World::Range::begin() { return entities.begin(); }
World::value_iterator World::Range::end() { return entities.end(); }

// World class.
World::World()
    : link_id(0),
      L(nullptr),
      entities(),
      range(entities),
      lower_bounds(0.0f, 0.0f),
      upper_bounds(1e9f, 1e9f),
      position_components(),
      speed_components(),
      render_components(),
      anim_components(),
      input_components() {
  registerLuaFunctions();
};

World::~World() { lua_close(L); }

int World::linkId() const {
  return link_id;
}

int& World::variableLinkId() {
  return link_id;
}

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
World::Range World::getEntities() { return range; }

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

const sf::Vector2f& World::lowerBounds() const {
  return lower_bounds;
}
sf::Vector2f& World::variableLowerBounds() {
  return lower_bounds;
}
const sf::Vector2f& World::upperBounds() const {
  return upper_bounds;
}
sf::Vector2f& World::variableUpperBounds() {
  return upper_bounds;
}

template <>
const std::vector<PositionComponent>& World::getVect<PositionComponent>()
    const {
  return position_components;
}

template <>
const std::vector<SpeedComponent>& World::getVect<SpeedComponent>() const {
  return speed_components;
}

template <>
const std::vector<RenderComponent>& World::getVect<RenderComponent>() const {
  return render_components;
}

template <>
const std::vector<AnimComponent>& World::getVect<AnimComponent>() const {
  return anim_components;
}

template <>
const std::vector<InputComponent>& World::getVect<InputComponent>() const {
  return input_components;
}

template <>
const std::vector<LogicComponent>& World::getVect<LogicComponent>() const {
  return logic_components;
}

template <typename C>
const std::vector<C>& World::getVect() const {
  std::cerr << "World::vect with unknown type" << std::endl;
  exit(0);
}
