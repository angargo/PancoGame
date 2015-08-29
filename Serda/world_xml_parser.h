#ifndef WORLD_XML_PARSER_H
#define WORLD_XML_PARSER_H

#include <istream>
#include <ostream>
#include <sstream>

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "rapidxml/rapidxml_print.hpp"

#include "entity.h"
#include "world.h"

typedef rapidxml::xml_node<> xml_node;

class WorldXmlParser {
 public:
  WorldXmlParser(World* world);

  void serialize(std::ostream& out, const Entity& entity) const;
  void serialize(std::ostream& out, id_type entity_id) const;
  std::string serializeToString(const Entity& entity) const;
  std::string serializeToString(id_type entity_id) const;

  void serialize(std::ostream& out, const PositionComponent& pos) const;
  void serialize(std::ostream& out, const SpeedComponent& speed) const;
  void serialize(std::ostream& out, const RenderComponent& render) const;
  void serialize(std::ostream& out, const InputComponent& input) const;

  void serialize(std::ostream& out, const Generic& generic) const;

  void deserializeEntity(const xml_node* node);

  PositionComponent deserializePosition(const xml_node* node);
  SpeedComponent deserializeSpeed(const xml_node* node);
  RenderComponent deserializeRender(const xml_node* node);
  InputComponent deserializeInput(const xml_node* node);

 private:
  World* world;
  std::vector<Generic> generics;

  template <typename C>
  C getFromString(const char* value) {
    std::stringstream ss(value);
    C aux;
    ss >> aux;
    return aux;
  }
};

#endif  // WORLD_XML_PARSER_H
