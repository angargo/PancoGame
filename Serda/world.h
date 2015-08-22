#ifndef SERDA_WORLD_H
#define SERDA_WORLD_H

#include <unordered_map>

#include "entity.h"

class World {
  // TODO: cache problems, maybe.
  std::unordered_map<id_type, Entity> entities;

  // Components go here.
};

#endif // SERDA_WORLD_H
