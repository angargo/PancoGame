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
  vx += v_x; vy += v_y;
}
void SpeedComponent::setSpeed(float v_x, float v_y) {
  vx = v_x; vy = v_y;
}

RenderComponent::RenderComponent() : Component() {}
RenderComponent::RenderComponent(sf::Sprite sprite)
    : Component(), sprite(std::move(sprite)) {}
RenderComponent::RenderComponent(const id_type entity_id, sf::Sprite sprite)
    : Component(entity_id), sprite(std::move(sprite)) {}

InputComponent::InputComponent() : Component() {}
InputComponent::InputComponent(const id_type entity_id)
    : Component(entity_id) {}


// Generic Entity.
GenericEntity::GenericEntity() : components() {}

template<> PositionComponent&
  GenericEntity::get<PositionComponent>() { return position; }

template<> SpeedComponent&
  GenericEntity::get<SpeedComponent>() { return speed; }

template<> RenderComponent&
  GenericEntity::get<RenderComponent>() { return render; }

template<> InputComponent&
  GenericEntity::get<InputComponent>() { return input; }


