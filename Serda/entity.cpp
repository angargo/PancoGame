#include "entity.h"

#include <iostream>

// Base component.
Component::Component() : entity_id(0) {}
Component::Component(const id_type entity_id) : entity_id(entity_id) {}
Component::~Component() {}

// Entity.
Entity::Entity(const id_type id) : id(id), components(), component_indices() {
  component_indices.fill(-1);
}
Entity::Entity() : Entity(0) {}
bool Entity::hasId() const { return id != 0; }

// Components.
PositionComponent::PositionComponent() : Component() {}
PositionComponent::PositionComponent(float x, float y)
    : Component(), x(x), y(y) {}
PositionComponent::PositionComponent(const id_type entity_id, float x, float y)
    : Component(entity_id), x(x), y(y) {}

SpeedComponent::SpeedComponent() : Component() {}
SpeedComponent::SpeedComponent(float vx, float vy)
    : Component(), vx(vx), vy(vy) {}
SpeedComponent::SpeedComponent(const id_type entity_id, float vx, float vy)
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
Frame::Frame() : texture_id(-1) { init(); }
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
RenderComponent::RenderComponent(int texture_id, int width, int height, int tx,
                                 int ty, bool rotated)
    : Component(), frame(texture_id, width, height, tx, ty, rotated) {}

int RenderComponent::textureId() const { return frame.texture_id; }
int RenderComponent::width() const { return frame.width; }
int RenderComponent::height() const { return frame.height; }
int RenderComponent::tx() const { return frame.tx; }
int RenderComponent::ty() const { return frame.ty; }
bool RenderComponent::rotated() const { return frame.rotated; }

// Animation Component.
AnimFrame::AnimFrame()
    : duration(sf::seconds(1.0f)), frame(), elapsed_time(sf::Time::Zero) {}
AnimFrame::AnimFrame(sf::Time duration, Frame frame)
    : duration(duration), frame(frame), elapsed_time(sf::Time::Zero) {}

Animation::Animation() : frames() {}
Animation::Animation(const std::vector<AnimFrame>& frames, bool repeated)
    : frames(frames), repeated(repeated) {}

AnimComponent::AnimComponent() : Component() {}
AnimComponent::AnimComponent(const Animation& animation)
    : animation(animation) {}
const Animation& AnimComponent::getAnimation() const { return animation; }
Animation& AnimComponent::getAnimation() { return animation; }

// Input Component.
InputComponent::InputComponent() : Component() {}
InputComponent::InputComponent(const id_type entity_id)
    : Component(entity_id) {}

// Generic Entity.
Generic::Generic() : components() {}
Generic::Generic(std::string type) : type(std::move(type)), components() {}

template <>
PositionComponent& Generic::get<PositionComponent>() {
  return position;
}

template <>
SpeedComponent& Generic::get<SpeedComponent>() {
  return speed;
}

template <>
RenderComponent& Generic::get<RenderComponent>() {
  return render;
}

template <>
AnimComponent& Generic::get<AnimComponent>() {
  return anim;
}

template <>
InputComponent& Generic::get<InputComponent>() {
  return input;
}
