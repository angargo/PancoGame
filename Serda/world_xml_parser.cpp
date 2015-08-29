#include "world_xml_parser.h"

#include <istream>
#include <sstream>

template <typename C>
static C getFromString(const char* value) {
  std::stringstream ss(value);
  C aux;
  ss >> aux;
  return aux;
}

template <typename C>
static C getAttribOrDefault(const xml_node* node, const char* name, C def) {
  const rapidxml::xml_attribute<> *attr = node->first_attribute(name);
  if (attr) return getFromString<C>(attr->value());
  return def;
}

static std::string loadFile(const std::string& filename) {
  std::ifstream in(filename);
  std::stringstream ss;
  ss << in.rdbuf();
  return ss.str();
}

WorldXmlParser::WorldXmlParser(World* world) : world(world) {
  loadDictionary("media/maps/dictionary");
}

void WorldXmlParser::loadDictionary(const std::string& filename) {
  std::ifstream in(filename);
  if (!in.good()) {
    throw std::runtime_error("WorldXmlParser - cannot load dictionary " +
                             filename);
  }
  std::string line;
  while (getline(in, line)) {
    std::stringstream ss(line);
    int world_id;
    std::string file;
    ss >> world_id >> file;
    files[world_id] = file;
  }
}

void WorldXmlParser::loadWorld(int world_id) {
  // Reset everything.
  generics.clear();
  world->reset();
  world->variableLinkId() = 1 << 31 - 1;

  const auto it = files.find(world_id);
  if (it == files.end()) {
    throw std::runtime_error("WorldXmlParser - Cannot find map " +
                             std::to_string(world_id));
  }

  const std::string& filename = it->second;
  std::string worldstr = loadFile(filename);
  rapidxml::xml_document<> doc;
  doc.parse<0>(&worldstr[0]);

  for (const auto* node = doc.first_node(); node; node = node->next_sibling()) {
    std::string name = node->name();
    if (name == "entity") {
      deserializeEntity(node);
    } else if (name == "generic") {
      deserializeGeneric(node);
    }
  }
}

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

void WorldXmlParser::serialize(std::ostream& out,
                               const Generic& generic) const {
  out << "<generic";
  out << " type=\"" << generic.getType() << '\"';

  out << ">";

  out << "</generic>";
}

void WorldXmlParser::deserializeEntity(const xml_node* node) {
  // TODO: remove.
  if (node->first_attribute("type")) return;
  id_type id;
  if (node->first_attribute("id")) {
    id = getFromString<id_type>(node->first_attribute("id")->value());
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

void WorldXmlParser::deserializeGeneric(const xml_node* node) {}

PositionComponent WorldXmlParser::deserializePosition(
    const xml_node* node) const {
  float x = getFromString<float>(node->first_attribute("x")->value());
  float y = getFromString<float>(node->first_attribute("y")->value());
  return PositionComponent(x, y);
}

SpeedComponent WorldXmlParser::deserializeSpeed(const xml_node* node) const {
  auto vx = getAttribOrDefault<float>(node, "vx", 0);
  auto vy = getAttribOrDefault<float>(node, "vy", 0);
  return SpeedComponent(vx, vy);
}

RenderComponent WorldXmlParser::deserializeRender(const xml_node* node) const {
  return RenderComponent(deserializeFrame(node->first_node("frame")));
}

InputComponent WorldXmlParser::deserializeInput(const xml_node* node) const {
  int id = getFromString<int>(node->first_attribute("script_id")->value());
  return InputComponent(id);
}

LogicComponent WorldXmlParser::deserializeLogic(const xml_node* node) const {
  int id = getFromString<int>(node->first_attribute("script_id")->value());
  return LogicComponent(id);
}

Frame WorldXmlParser::deserializeFrame(const xml_node* node) const {
  auto tex_id = getFromString<int>(node->first_attribute("tex_id")->value());
  auto width = getFromString<int>(node->first_attribute("width")->value());
  auto height = getFromString<int>(node->first_attribute("height")->value());
  auto tx = getAttribOrDefault<int>(node, "tx", 0);
  auto ty = getAttribOrDefault<int>(node, "ty", 0);
  return Frame(tex_id, width, height, tx, ty);
}
