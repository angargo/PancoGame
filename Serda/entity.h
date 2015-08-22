#ifndef SERDA_ENTITY_H
#define SERDA_ENTITY_H

#include <array>
#include <bitset>

typedef int id_type;

// Component base class.
class Component {
public:
  enum ComponentId {
    POSITION = 0,
    NUM_IDS = 1,
  };

  explicit Component(const id_type entity_id);
  virtual ~Component() = 0;

  const id_type entity_id;
};

// Entity class.
class Entity {
public:
  explicit Entity(const id_type id);
  bool hasComponent(Component::ComponentId id);

  const id_type id;
  std::bitset<Component::NUM_IDS> components;
  std::array<int, Component::NUM_IDS> component_indices;
};

// Sample PositionComponent.
class PositionComponent : public Component {
public:
  explicit PositionComponent(const id_type entity_id, float x, float y);

  float x, y;
};
#endif  // SERDA_ENTITY_H
