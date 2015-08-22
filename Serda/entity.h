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
  Entity();
  explicit Entity(const id_type id);
  bool hasId() const;
  bool hasComponent(Component::Id id) const;

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

  void addSpeed(float v_x, float v_y);
  void setSpeed(float v_x, float v_y);

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

struct InputEvent {
  enum KeyAction {
    KEY_PRESSED = 0,
    KEY_RELEASED = 1,
  };
  // If not pressed, it's implicitly released.
  bool key_pressed;
  sf::Keyboard::Key key;

  InputEvent(KeyAction key_action, sf::Keyboard::Key key)
      : key_pressed(key_action), key(key) {}

  // Comparator function to use InputEvent as a map key.
  bool operator<(const InputEvent& other) const {
    if (key_pressed ^ other.key_pressed) return key_pressed < other.key_pressed;
    return key < other.key;
  }
};


class InputComponent : public Component {
public:
  InputComponent();
  explicit InputComponent(const id_type entity_id);

  std::map<InputEvent, Action> bindings;
};

#endif  // SERDA_ENTITY_H
