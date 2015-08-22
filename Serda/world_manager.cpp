
#include "world_manager.h"

WorldManager::WorldManager(World* world) : world(world) {
}

void WorldManager::serialize(std::ostream& out, const Entity& entity) const {
}

void WorldManager::serialize(std::ostream& out, id_type entity_id) const {
  serialize(out, world->getEntity(entity_id));
}
