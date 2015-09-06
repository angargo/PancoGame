#ifndef WORLD_XML_PARSER_H
#define WORLD_XML_PARSER_H

#include <ostream>

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "rapidxml/rapidxml_print.hpp"

#include "entity.h"
#include "world.h"

typedef rapidxml::xml_node<> xml_node;

class WorldXmlParser {
 public:
  WorldXmlParser(World* world);

  void loadDictionary(const std::string& filename);

  void loadWorld(int world_id);
  void dumpWorld(const std::string& filename) const;

 private:
  // Serialize entities.
  void serialize(std::ostream& out, const Entity& entity) const;
  void serialize(std::ostream& out, id_type entity_id) const;
  std::string serializeToString(const Entity& entity) const;
  std::string serializeToString(id_type entity_id) const;

  // Serialize components.
  void serialize(std::ostream& out, const PositionComponent& pos) const;
  void serialize(std::ostream& out, const SpeedComponent& speed) const;
  void serialize(std::ostream& out, const RenderComponent& render) const;
  void serialize(std::ostream& out, const AnimComponent& render) const;
  void serialize(std::ostream& out, const InputComponent& input) const;
  void serialize(std::ostream& out, const LogicComponent& input) const;
  void serialize(std::ostream& out, const CollisionComponent& input) const;

  // Serialize generic.
  void serialize(std::ostream& out, const Generic& generic) const;

  // Read from XML.
  void deserializeEntity(const xml_node* node);
  void deserializeGeneric(const xml_node* node);
  void instantiateGeneric(const std::string& type, id_type id);

  PositionComponent deserializePosition(const xml_node* node) const;
  SpeedComponent deserializeSpeed(const xml_node* node) const;
  RenderComponent deserializeRender(const xml_node* node) const;
  AnimComponent deserializeAnim(const xml_node* node) const;
  InputComponent deserializeInput(const xml_node* node) const;
  LogicComponent deserializeLogic(const xml_node* node) const;
  CollisionComponent deserializeCollision(const xml_node* node) const;

  Frame deserializeFrame(const xml_node* node) const;
  Animation deserializeAnimation(const xml_node* node) const;
  AnimFrame deserializeAnimFrame(const xml_node* node) const;
  Hitbox deserializeHitbox(const xml_node* node) const;

  // Map attributes
  void deserializeMapBounds(const xml_node* node) const;

 private:
  World* world;
  std::map<std::string, Generic> generics;
  std::map<int, std::string> files;
};

#endif  // WORLD_XML_PARSER_H
