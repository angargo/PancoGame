#include "entity.h"

// Base component.
Component::Component(const id_type entity_id) : entity_id(entity_id) {}
Component::~Component() {}

// Entity.
Entity::Entity(const id_type id) : id(id), components(), component_indices() {}

bool Entity::hasComponent(Component::ComponentId id) {
  return components[id];
}

// Components.
PositionComponent::PositionComponent(const id_type entity_id, float x, float y)
    : Component(entity_id), x(x), y(y) {
}
