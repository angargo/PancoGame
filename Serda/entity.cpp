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
bool Entity::hasComponent(Component::Id id) const {
  return components[id];
}



// Components.
PositionComponent::PositionComponent(float x, float y)
    : Component(), x(x), y(y) {}
PositionComponent::PositionComponent(const id_type entity_id, float x, float y)
    : Component(entity_id), x(x), y(y) {}

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

RenderComponent::RenderComponent(sf::Sprite sprite)
    : Component(), sprite(std::move(sprite)) {}
RenderComponent::RenderComponent(const id_type entity_id, sf::Sprite sprite)
    : Component(entity_id), sprite(std::move(sprite)) {}

InputComponent::InputComponent() : Component() {}
InputComponent::InputComponent(const id_type entity_id)
    : Component(entity_id) {}


// Generic Entity.
GenericEntity::GenericEntity() : components() {}

bool GenericEntity::hasComponent(Component::Id id) const {
  return component_bitset[id];
}

void GenericEntity::addComponent(const PositionComponent& component) {
  component_bitset.set(Component::POSITION);
  components[Component::POSITION].reset(new PositionComponent(component));
}
void GenericEntity::addComponent(const SpeedComponent& component) {
  component_bitset.set(Component::SPEED);
  components[Component::SPEED].reset(new SpeedComponent(component));
}
void GenericEntity::addComponent(const RenderComponent& component) {
  component_bitset.set(Component::RENDER);
  components[Component::RENDER].reset(new RenderComponent(component));
}
void GenericEntity::addComponent(const InputComponent& component) {
  component_bitset.set(Component::INPUT);
  components[Component::INPUT].reset(new InputComponent(component));
}

template<class Comp>
const Comp& GenericEntity::get() const {
  Comp* comp;
  for (const auto& ptr : components) {
    if ((comp = dynamic_cast<Comp*>(ptr.get())))
      return *comp;
  }
  std::cerr << "Generic doesn't have requested component" << std::endl;
  exit(0);
}


