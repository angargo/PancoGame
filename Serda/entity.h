#ifndef SERDA_ENTITY_H
#define SERDA_ENTITY_H

#include <array>
#include <bitset>
#include <functional>
#include <map>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Keyboard.hpp>

typedef int id_type;

// Component base class.
class Component {
public:
  enum Id {
    POSITION = 0,
    SPEED = 1,
    RENDER = 2,
    INPUT = 3,
    NUM_IDS = 4,
  };

  Component();
  explicit Component(const id_type entity_id);
  virtual ~Component() = 0;

  id_type entity_id;
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
  PositionComponent(float x, float y);
  PositionComponent(const id_type entity_id, float x, float y);

  float x, y;
};

class SpeedComponent : public Component {
public:
  SpeedComponent(float vx, float vy);
  SpeedComponent(const id_type entity_id, float vx, float vy);

  float vx, vy;
};

class RenderComponent : public Component {
public:
  explicit RenderComponent(sf::Sprite sprite);
  RenderComponent(const id_type entity_id, sf::Sprite sprite);

  sf::Sprite sprite;
};

class World;
typedef std::function<void(World*,id_type)> Action;

class InputComponent : public Component {
public:
  explicit InputComponent(const id_type entity_id);

  std::map<sf::Keyboard::Key, Action> bindings;
};

#endif  // SERDA_ENTITY_H
