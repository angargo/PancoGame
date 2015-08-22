#ifndef SERDA_ENTITY_H
#define SERDA_ENTITY_H

#include <array>
#include <bitset>

typedef int id_type;

// Ids for all components.
enum ComponentId {
  POSITION = 0,
  NUM_IDS = 1,
};

// Entity class.
class Entity {
public:
  explicit Entity(const id_type id);

  const id_type id;
  std::bitset<ComponentId::NUM_IDS> components;
  std::array<int, ComponentId::NUM_IDS> component_indices;
};

// Component base class.
class Component {
public:
  explicit Component(const id_type entity_id);
  virtual ~Component() = 0;

  const id_type entity_id;
};

// Sample PositionComponent.
class PositionComponent : public Component {
public:
  explicit PositionComponent(const id_type entity_id, float x, float y);

  float x, y;
};
#endif  // SERDA_ENTITY_H
