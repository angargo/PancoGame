#include "entity.h"

#include <iostream>

// Base component.
Component::Component() : entity_id(0) {}
Component::Component(id_type entity_id) : entity_id(entity_id) {}
Component::~Component() {}

// Entity.
Entity::Entity(id_type id) : id(id), components(), component_indices() {
  component_indices.fill(-1);
}
Entity::Entity() : Entity(0) {}
bool Entity::hasId() const { return id != 0; }

// Components.
PositionComponent::PositionComponent() : Component() {}
PositionComponent::PositionComponent(float x, float y, float z)
    : Component(), x(x), y(y), z(z) {}
PositionComponent::PositionComponent(id_type entity_id, float x, float y,
                                     float z)
    : Component(entity_id), x(x), y(y), z(z) {}

SpeedComponent::SpeedComponent() : Component() {}
SpeedComponent::SpeedComponent(float vx, float vy)
    : Component(), vx(vx), vy(vy) {}
SpeedComponent::SpeedComponent(id_type entity_id, float vx, float vy)
    : Component(entity_id), vx(vx), vy(vy) {}
void SpeedComponent::addSpeed(float v_x, float v_y) {
  vx += v_x;
  vy += v_y;
}
void SpeedComponent::setSpeed(float v_x, float v_y) {
  vx = v_x;
  vy = v_y;
}

// Render Component.
Frame::Frame() : texture_id(TextureId::NONE) { init(); }
Frame::Frame(int texture_id, int width, int height, int tx, int ty,
             bool rotated)
    : texture_id(texture_id),
      width(width),
      height(height),
      tx(tx),
      ty(ty),
      rotated(rotated) {}

Frame::Frame(int texture_id) : texture_id(texture_id) {}

void Frame::init() {
  tx = 0;
  ty = 0;
  width = -1;
  height = -1;
  rotated = false;
}

RenderComponent::RenderComponent() : Component() {}
RenderComponent::RenderComponent(Frame frame)
    : Component(), frame(std::move(frame)) {}
RenderComponent::RenderComponent(int texture_id, int width, int height, int tx,
                                 int ty, bool rotated)
    : Component(), frame(texture_id, width, height, tx, ty, rotated) {}

int RenderComponent::textureId() const { return frame.texture_id; }
int RenderComponent::width() const { return frame.width; }
int RenderComponent::height() const { return frame.height; }
int RenderComponent::tx() const { return frame.tx; }
int RenderComponent::ty() const { return frame.ty; }
bool RenderComponent::rotated() const { return frame.rotated; }
const Frame& RenderComponent::getFrame() const { return frame; }
Frame& RenderComponent::variableFrame() { return frame; }

// Animation Component.
AnimFrame::AnimFrame()
    : duration(sf::seconds(1.0f)), frame(), elapsed_time(sf::Time::Zero) {}
AnimFrame::AnimFrame(sf::Time duration, Frame frame)
    : duration(duration), frame(frame), elapsed_time(sf::Time::Zero) {}
AnimFrame::AnimFrame(float duration_secs, Frame frame)
    : duration(sf::seconds(duration_secs)),
      frame(frame),
      elapsed_time(sf::Time::Zero) {}

Animation::Animation() : frames() {}
Animation::Animation(const std::vector<AnimFrame>& frames, bool repeated)
    : frames(frames), repeated(repeated) {}

AnimComponent::AnimComponent() : Component(), index(0) {}
AnimComponent::AnimComponent(const Animation& animation)
    : index(0), animation(animation) {}
const Animation& AnimComponent::getAnimation() const { return animation; }
Animation& AnimComponent::variableAnimation() { return animation; }

// Input Component.
InputComponent::InputComponent() : Component(), script_id(-1) {}
InputComponent::InputComponent(int script_id)
    : Component(), script_id(script_id) {}
InputComponent::InputComponent(id_type entity_id, int script_id)
    : Component(entity_id), script_id(script_id) {}

// Logic Component.
LogicComponent::LogicComponent() : Component(), script_id(-1) {}
LogicComponent::LogicComponent(int script_id)
    : Component(), script_id(script_id) {}
LogicComponent::LogicComponent(id_type entity_id, int script_id)
    : Component(entity_id), script_id(script_id) {}

bool Hitbox::collidesWith(const Hitbox& hitbox) const { return false; }
CollisionComponent::CollisionComponent() : Component() {}

// Generic Entity.
Generic::Generic() : components() {}
Generic::Generic(std::string type) : type(std::move(type)), components() {}

std::string Generic::getType() const { return type; }

template <>
const PositionComponent& Generic::get<PositionComponent>() const {
  return position;
}

template <>
const SpeedComponent& Generic::get<SpeedComponent>() const {
  return speed;
}

template <>
const RenderComponent& Generic::get<RenderComponent>() const {
  return render;
}

template <>
const AnimComponent& Generic::get<AnimComponent>() const {
  return anim;
}

template <>
const InputComponent& Generic::get<InputComponent>() const {
  return input;
}

template <>
const LogicComponent& Generic::get<LogicComponent>() const {
  return logic;
}

template <>
const CollisionComponent& Generic::get<CollisionComponent>() const {
  return collision;
}
