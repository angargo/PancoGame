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
#include <SFML/System/Time.hpp>

typedef int id_type;

// Component base class.
class Component {
 public:
  enum Id { POSITION, SPEED, RENDER, ANIM, INPUT, NUM_IDS };

  Component();
  explicit Component(id_type entity_id);

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
  explicit Entity(id_type id);
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
  PositionComponent(id_type entity_id, float x, float y);

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
  SpeedComponent(id_type entity_id, float vx, float vy);

  void addSpeed(float v_x, float v_y);
  void setSpeed(float v_x, float v_y);

  float vx, vy;
};
template <>
struct Id<SpeedComponent> {
  operator int() { return Component::SPEED; }
};

// Frame Class for RenderComponent.
struct Frame {
  Frame();
  explicit Frame(int texture_id);
  explicit Frame(int texture_id, int width = 0, int height = 0,
                           int tx = 0, int ty = 0, bool rotated = false);
  void init();
  int texture_id;
  int width;     // Width of the sprite.
  int height;    // Height of the sprite.
  int tx;        // X offset inside texture.
  int ty;        // Y offset inside texture.
  bool rotated;  // Is a rotated loaded image.
};
class RenderComponent : public Component {
 public:
  RenderComponent();
  explicit RenderComponent(int texture_id, int width = 0, int height = 0,
                           int tx = 0, int ty = 0, bool rotated = false);
  RenderComponent(id_type entity_id, int texture_id);

  int textureId() const;
  int width() const;
  int height() const;
  int tx() const;
  int ty() const;
  bool rotated() const;
  const Frame& getFrame() const;
  Frame& getFrame();

 private:
  void init();
  Frame frame;
};

template <>
struct Id<RenderComponent> {
  operator int() { return Component::RENDER; }
};
// Animation.
struct AnimFrame {
  AnimFrame();
  AnimFrame(sf::Time duration, Frame frame);
  sf::Time duration;
  Frame frame;
  sf::Time elapsed_time;
};
struct Animation {
  Animation();
  Animation(const std::vector<AnimFrame>& frames, bool repeated = false);

  std::vector<AnimFrame> frames;
  bool repeated;  // Whether this animation repeats infinitely.
};
class AnimComponent : public Component {
 public:
  AnimComponent();
  AnimComponent(const Animation& animation);

  const Animation& getAnimation() const;
  Animation& getAnimation();

  int index;
 private:
  Animation animation;
};

template<>
struct Id<AnimComponent> {
  operator int() { return Component::ANIM; }
};


struct InputEvent {
  enum KeyAction {
    KEY_RELEASED = 0,
    KEY_PRESSED = 1,
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
  explicit InputComponent(int script_id);
  InputComponent(id_type entity_id, int script_id);

  int script_id;
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
  void addComponent(const AnimComponent& component);
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
  AnimComponent anim;
  InputComponent input;
};

#endif  // SERDA_ENTITY_H
