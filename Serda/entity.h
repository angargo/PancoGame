#ifndef SERDA_ENTITY_H
#define SERDA_ENTITY_H

#include <array>
#include <bitset>
#include <exception>
#include <functional>
#include <map>
#include <memory>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Keyboard.hpp>

typedef int id_type;

// Component base class.
class Component {
 public:
  enum Id { POSITION, SPEED, RENDER, INPUT, NUM_IDS };

  Component();
  explicit Component(const id_type entity_id);

  virtual ~Component() = 0;

  id_type entity_id;
};
template <typename C>
struct Id {
  operator int() {
    throw std::runtime_error("Id::operator int() with unknown type");
  }
};

// Entity class.
class Entity {
 public:
  Entity();
  explicit Entity(const id_type id);
  bool hasId() const;

  // Check if this entity has component C.
  template <typename C>
  bool has() const {
    return components[Id<C>()];
  }

  const id_type id;
  std::bitset<Component::NUM_IDS> components;
  std::array<int, Component::NUM_IDS> component_indices;
};

// Sample PositionComponent.
class PositionComponent : public Component {
 public:
  PositionComponent();
  PositionComponent(float x, float y);
  PositionComponent(const id_type entity_id, float x, float y);

  float x, y;
};
template <>
struct Id<PositionComponent> {
  operator int() { return Component::POSITION; }
};

class SpeedComponent : public Component {
 public:
  SpeedComponent();
  SpeedComponent(float vx, float vy);
  SpeedComponent(const id_type entity_id, float vx, float vy);

  void addSpeed(float v_x, float v_y);
  void setSpeed(float v_x, float v_y);

  float vx, vy;
};
template <>
struct Id<SpeedComponent> {
  operator int() { return Component::SPEED; }
};

class RenderComponent : public Component {
 public:
  RenderComponent();
  explicit RenderComponent(int texture_id, int width = 0, int height = 0,
                           int tx = 0, int ty = 0, bool rotated = false);
  RenderComponent(const id_type entity_id, int texture_id);

  int texture_id;
  int width;     // Width of the sprite.
  int height;    // Height of the sprite.
  int tx;        // X offset inside texture.
  int ty;        // Y offset inside texture.
  bool rotated;  // Is a rotated loaded image.

 private:
  void init();
};
template <>
struct Id<RenderComponent> {
  operator int() { return Component::RENDER; }
};

class World;
typedef std::function<void(World*, id_type)> Action;

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
template <>
struct Id<InputComponent> {
  operator int() { return Component::INPUT; }
};

class Generic {
 public:
  Generic();
  Generic(std::string type);

  template <class C>
  bool has() const {
    return components[Id<C>()];
  }

  void addComponent(const PositionComponent& component);
  void addComponent(const SpeedComponent& component);
  void addComponent(const RenderComponent& component);
  void addComponent(const InputComponent& component);

  std::string getType() const { return type; }

  template <class C>
  C& get() {
    throw std::exception("Error Generic::get - Unknown type");
  }

 private:
  std::bitset<Component::NUM_IDS> components;

  std::string type;

  PositionComponent position;
  SpeedComponent speed;
  RenderComponent render;
  InputComponent input;
};

#endif  // SERDA_ENTITY_H
