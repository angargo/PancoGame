
#include "world_xml_parser.h"

WorldXmlParser::WorldXmlParser(World* world) : world(world) {}

void WorldXmlParser::deserializeEntity(const xml_node* node) {
  id_type id;
  if (node->first_attribute("id")) {
    id = getFromString<int>(node->first_attribute("id")->value());
    world->createEntity(id);
  } else {
    id = world->createEntity();
  }

  if (const auto* cnode = node->first_node("position"))
    world->add(id, deserializePosition(cnode));

  if (const auto* cnode = node->first_node("speed"))
    world->add(id, deserializeSpeed(cnode));

  if (const auto* cnode = node->first_node("render"))
    world->add(id, deserializeRender(cnode));

  if (const auto* cnode = node->first_node("input"))
    world->add(id, deserializeInput(cnode));
}

PositionComponent WorldXmlParser::deserializePosition(const xml_node* node) {
  float x = getFromString<float>(node->first_attribute("x")->value());
  float y = getFromString<float>(node->first_attribute("y")->value());
  return PositionComponent(x, y);
}

SpeedComponent WorldXmlParser::deserializeSpeed(const xml_node* node) {
  float vx = getFromString<float>(node->first_attribute("vx")->value());
  float vy = getFromString<float>(node->first_attribute("vy")->value());
  return SpeedComponent(vx, vy);
}

RenderComponent WorldXmlParser::deserializeRender(const xml_node* node) {
  // TODO
}

InputComponent WorldXmlParser::deserializeInput(const xml_node* node) {
  // TODO
}
