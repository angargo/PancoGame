#ifndef SERDA_WORLD_MANAGER_H
#define SERDA_WORLD_MANAGER_H

#include <ostream>

#include "entity.h"
#include "world.h"
#include "world_xml_parser.h"

class WorldManager {
 public:
  WorldManager(World* world);

  void serialize(std::ostream& out, const Entity& entity) const;
  void serialize(std::ostream& out, id_type entity_id) const;
  std::string serializeToString(const Entity& entity) const;
  std::string serializeToString(id_type entity_id) const;

  void serialize(std::ostream& out, const PositionComponent& pos) const;
  void serialize(std::ostream& out, const SpeedComponent& speed) const;
  void serialize(std::ostream& out, const RenderComponent& render) const;
  void serialize(std::ostream& out, const InputComponent& input) const;

  void serialize(std::ostream& out, const Generic& generic) const;

 private:
  World* world;
  std::vector<Generic> generics;
};

#endif  // SERDA_WORLD_MANAGER_H
