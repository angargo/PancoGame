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

RenderComponent::RenderComponent() : texture_id(-1) { init(); }
RenderComponent::RenderComponent(int texture_id, int width, int height, int tx,
                                 int ty, bool rotated)
    : Component(),
      texture_id(texture_id),
      width(width),
      height(height),
      tx(tx),
      ty(ty),
      rotated(rotated) {}

RenderComponent::RenderComponent(const id_type entity_id, int texture_id)
    : Component(entity_id), texture_id(texture_id) {}

void RenderComponent::init() {
  tx = 0;
  ty = 0;
  width = -1;
  height = -1;
  rotated = false;
}

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
InputComponent& Generic::get<InputComponent>() {
  return input;
}
