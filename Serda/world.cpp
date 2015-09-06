#include <cassert>

#include "world.h"

const id_type World::DYNAMIC_ID_RANGE_FROM = 1u << 25;
const id_type World::DYNAMIC_ID_RANGE_TO = (1u << 31) - 1;

// World iterator for entities.
World::value_iterator::value_iterator() : World::EntityMap::iterator() {}
World::value_iterator::value_iterator(const World::EntityMap::iterator& e)
    : EntityMap::iterator(e){};
Entity* World::value_iterator::operator->() {
  return (Entity * const) & (EntityMap::iterator::operator->()->second);
}
Entity& World::value_iterator::operator*() {
  return EntityMap::iterator::operator*().second;
}

// World iterator for pairs of entities.
World::pair_iterator::pair_iterator(World::EntityMap& entities,
                                    World::EntityMap::iterator first,
                                    World::EntityMap::iterator second)
    : entities(entities), p(first, second) {}
std::pair<Entity&, Entity&> World::pair_iterator::operator*() {
  return std::pair<Entity&, Entity&>(p.first->second, p.second->second);
}
World::pair_iterator& World::pair_iterator::operator++() {
  ++p.second;
  if (p.second == entities.end()) {
    p.second = ++p.first;
    ++p.second;
    if (p.second == entities.end()) ++p.first;
  }
  return *this;
}
bool World::pair_iterator::operator!=(const World::pair_iterator& pi) const {
  return p != pi.p;
}

// World Range struct to iterate over entities with for range loop.
World::Range::Range(EntityMap& entities) : entities(entities) {}
World::value_iterator World::Range::begin() { return entities.begin(); }
World::value_iterator World::Range::end() { return entities.end(); }

// World PairRange struct to iterate over pairs of entities.
World::PairRange::PairRange(EntityMap& entities) : entities(entities) {}
World::pair_iterator World::PairRange::begin() {
  if (entities.size() < 2) {
    return end();
  }
  return pair_iterator(entities, entities.begin(), ++entities.begin());
}
World::pair_iterator World::PairRange::end() {
  return pair_iterator(entities, entities.end(), entities.end());
}

// World class.
World::World()
    : link_id(0),
      L(nullptr),
      entities(),
      range(entities),
      pair_range(entities),
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

void World::reset() {
  // TODO: maybe reset Lua.

  // Reset entities and components.
  entities.clear();
  position_components.clear();
  speed_components.clear();
  render_components.clear();
  anim_components.clear();
  input_components.clear();
  logic_components.clear();

  // Reset link.
  link_id = 0;

  // Reset map bounds.
  lower_bounds = sf::Vector2f(0.0f, 0.0f);
  upper_bounds = sf::Vector2f(1e9f, 1e9f);
}

int World::linkId() const { return link_id; }

int& World::variableLinkId() { return link_id; }

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
World::PairRange World::getEntityPairs() { return pair_range; }

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

const sf::Vector2f& World::lowerBounds() const { return lower_bounds; }
sf::Vector2f& World::variableLowerBounds() { return lower_bounds; }
const sf::Vector2f& World::upperBounds() const { return upper_bounds; }
sf::Vector2f& World::variableUpperBounds() { return upper_bounds; }

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

template <>
const std::vector<CollisionComponent>& World::getVect<CollisionComponent>()
    const {
  return collision_components;
}

template <typename C>
const std::vector<C>& World::getVect() const {
  std::runtime_error("World::vect with unknown type");
}
