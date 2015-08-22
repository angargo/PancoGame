#include "entity.h"

Entity::Entity(const id_type id) : id(id), components(), component_indices() {}

Component::Component(const id_type entity_id) : entity_id(entity_id) {}
Component::~Component() {}

PositionComponent::PositionComponent(const id_type entity_id, float x, float y)
    : Component(entity_id), x(x), y(y) {}
