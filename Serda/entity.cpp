#include "entity.h"

Entity::Entity(const int id) : id(id), components(), component_indices() {} 

Component::Component(const int entity_id) : entity_id(entity_id) {}
Component::~Component() {}

PositionComponent::PositionComponent(const int entity_id, float x, float y)
    : Component(entity_id), x(x), y(y) {
}
