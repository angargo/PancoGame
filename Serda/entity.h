#ifndef SERDA_ENTITY_H
#define SERDA_ENTITY_H

#include <array>
#include <bitset>

#include <SFML/Graphics/Sprite.hpp>

typedef int id_type;

// Component base class.
class Component {
public:
  enum Id {
    POSITION = 0,
    SPEED = 1,
    RENDER = 2,
    NUM_IDS = 3,
  };

  explicit Component(const id_type entity_id);
  virtual ~Component() = 0;

  const id_type entity_id;
};

// Entity class.
class Entity {
public:
  explicit Entity(const id_type id);
  bool hasComponent(Component::Id id);

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

class SpeedComponent : public Component {
public:
  explicit SpeedComponent(const id_type entity_id, float vx, float vy);

  float vx, vy;
};

class RenderComponent : public Component {
public:
  explicit RenderComponent(const id_type entity_id, sf::Sprite sprite);

  sf::Sprite sprite;
};

#endif  // SERDA_ENTITY_H
