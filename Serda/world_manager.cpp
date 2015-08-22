
#include "world_manager.h"

#include <sstream>

WorldManager::WorldManager(World* world) : world(world) {
}

void WorldManager::serialize(std::ostream& out, const Entity& entity) const {
  out << "<entity";
  if (entity.hasId()) out << " id=" << entity.id;
  out << ">";

  if (entity.hasComponent(Component::POSITION))
    serialize(out, world->getPositionComponent(entity));
  if (entity.hasComponent(Component::SPEED))
    serialize(out, world->getSpeedComponent(entity));
  if (entity.hasComponent(Component::RENDER))
    serialize(out, world->getRenderComponent(entity));
  if (entity.hasComponent(Component::INPUT))
    serialize(out, world->getInputComponent(entity));

  out << "</entity>";
}
void WorldManager::serialize(std::ostream& out, id_type entity_id) const {
  serialize(out, world->getEntity(entity_id));
}
std::string WorldManager::serializeToString(const Entity& entity) const {
  std::stringstream ss;
  serialize(ss, entity);
  return ss.str();
}
std::string WorldManager::serializeToString(id_type entity_id) const {
  return serializeToString(world->getEntity(entity_id));
}

void WorldManager::serialize(std::ostream& out,
                             const PositionComponent& pos) const {
  out << "<position";
  out << " x=" << pos.x;
  out << " y=" << pos.y;
  out << "/>";
}

void WorldManager::serialize(std::ostream& out,
                             const SpeedComponent& speed) const {
  out << "<speed";
  out << " vx=" << speed.vx;
  out << " vy=" << speed.vy;
  out << "/>";
}

void WorldManager::serialize(std::ostream& out,
                             const RenderComponent& render) const {
  out << "<render";
  // TODO
  out << "/>";
}

void WorldManager::serialize(std::ostream& out,
                             const InputComponent& input) const {
  out << "<input";
  // TODO
  out << "/>";
}
