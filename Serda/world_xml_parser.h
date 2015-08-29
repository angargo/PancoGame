#ifndef WORLD_XML_PARSER_H
#define WORLD_XML_PARSER_H

#include <istream>
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

  void deserializeEntity(const xml_node* node);

  PositionComponent deserializePosition(const xml_node* node);
  SpeedComponent deserializeSpeed(const xml_node* node);
  RenderComponent deserializeRender(const xml_node* node);
  InputComponent deserializeInput(const xml_node* node);

 private:
  World* world;

  template <typename C>
  C getFromString(const char* value) {
    std::stringstream ss(value);
    C aux;
    ss >> aux;
    return aux;
  }
};

#endif  // WORLD_XML_PARSER_H
