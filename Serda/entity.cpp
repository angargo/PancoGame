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
PositionComponent::PositionComponent(float x, float y)
    : Component(), x(x), y(y) {}
PositionComponent::PositionComponent(id_type entity_id, float x, float y)
    : Component(entity_id), x(x), y(y) {}

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

RenderComponent::RenderComponent() : Component() {}
RenderComponent::RenderComponent(sf::Sprite sprite)
    : Component(), sprite(std::move(sprite)) {}
RenderComponent::RenderComponent(id_type entity_id, sf::Sprite sprite)
    : Component(entity_id), sprite(std::move(sprite)) {}

InputComponent::InputComponent() : Component(), script_id(-1) {}
InputComponent::InputComponent(int script_id)
    : Component(), script_id(script_id) {}
InputComponent::InputComponent(id_type entity_id, int script_id)
    : Component(entity_id), script_id(script_id) {}

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
