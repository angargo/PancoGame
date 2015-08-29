
#include "world_xml_parser.h"

WorldXmlParser::WorldXmlParser(World* world) : world(world) {}


void WorldXmlParser::serialize(std::ostream& out, const Entity& entity) const {
  out << "<entity";
  if (entity.hasId()) out << " id=" << entity.id;
  out << ">";

  if (entity.has<PositionComponent>())
    serialize(out, world->get<PositionComponent>(entity));

  if (entity.has<SpeedComponent>())
    serialize(out, world->get<SpeedComponent>(entity));

  if (entity.has<RenderComponent>())
    serialize(out, world->get<RenderComponent>(entity));

  if (entity.has<InputComponent>())
    serialize(out, world->get<InputComponent>(entity));

  out << "</entity>";
}
void WorldXmlParser::serialize(std::ostream& out, id_type entity_id) const {
  serialize(out, world->getEntity(entity_id));
}
std::string WorldXmlParser::serializeToString(const Entity& entity) const {
  std::stringstream ss;
  serialize(ss, entity);
  return ss.str();
}
std::string WorldXmlParser::serializeToString(id_type entity_id) const {
  return serializeToString(world->getEntity(entity_id));
}

void WorldXmlParser::serialize(std::ostream& out,
                             const PositionComponent& pos) const {
  out << "<position";
  out << " x=" << pos.x;
  out << " y=" << pos.y;
  out << "/>";
}

void WorldXmlParser::serialize(std::ostream& out,
                             const SpeedComponent& speed) const {
  out << "<speed";
  out << " vx=" << speed.vx;
  out << " vy=" << speed.vy;
  out << "/>";
}

void WorldXmlParser::serialize(std::ostream& out,
                             const RenderComponent& render) const {
  out << "<render";
  // TODO
  out << "/>";
}

void WorldXmlParser::serialize(std::ostream& out,
                             const InputComponent& input) const {
  out << "<input>";
  // TODO
  out << "</input>";
}

void WorldXmlParser::serialize(std::ostream& out, const Generic& generic) const {
  out << "<generic";
  out << " type=\"" << generic.getType() << '\"';

  out << ">";

  out << "</generic>";
}

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
