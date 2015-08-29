#ifndef WORLD_XML_PARSER_H
#define WORLD_XML_PARSER_H

#include <istream>
#include <sstream>

#include "entity.h"
#include "world.h"

#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"

class WorldXmlParser {
 public:
  WorldXmlParser(World* world);

  void deserialize_Entity(rapidxml::xml_node nodo);

  void deserialize_Position(rapidxml::xml_node nodo,id_type id);
  void deserialize_Speed(rapidxml::xml_node nodo,id_type id);
  void deserialize_Render(rapidxml::xml_node nodo,id_type id);
  void deserialize_Input(rapidxml::xml_node nodo,id_type id);

 private:
  World* world;

  template <typename C>
  C getFromString(char* value){
    std::stringstream ss(value);
    C aux;
    ss >> aux;
    return aux;
  }
};

#endif // WORLD_XML_PARSER_H
