#include "entity.h"

// Base component.
Component::Component(const id_type entity_id) : entity_id(entity_id) {}
Component::~Component() {}

// Entity.
Entity::Entity(const id_type id) : id(id), components(), component_indices() {}

bool Entity::hasComponent(Component::Id id) {
  return components[id];
}

// Components.
PositionComponent::PositionComponent(const id_type entity_id, float x, float y)
    : Component(entity_id), x(x), y(y) {
}

SpeedComponent::SpeedComponent(const id_type entity_id, float vx, float vy)
    : Component(entity_id), vx(vx), vy(vy) {
}

RenderComponent::RenderComponent(const id_type entity_id, sf::Sprite sprite)
    : Component(entity_id), sprite(std::move(sprite)) {
}
