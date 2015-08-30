#include "world_xml_parser.h"

#include <istream>
#include <sstream>

#include <SFML/System/Vector2.hpp>

namespace {

template <typename C>
C getFromString(const char* value) {
  std::stringstream ss(value);
  C aux;
  ss >> aux;
  return aux;
}

template <>
bool getFromString(const char* value) {
  if (std::string(value) == "true") return true;
  return false;
}

template <typename C>
C getAttribOrDefault(const xml_node* node, const char* name, C def) {
  const rapidxml::xml_attribute<>* attr = node->first_attribute(name);
  if (attr) return getFromString<C>(attr->value());
  return def;
}

template <typename C>
C getAttrib(const xml_node* node, const char* name) {
  const rapidxml::xml_attribute<>* attr = node->first_attribute(name);
  if (not attr)
    throw std::runtime_error("Attribute " + std::string(name) +
                             " missing in node " + std::string(node->name()));
  return getFromString<C>(attr->value());
}

std::string loadFile(const std::string& filename) {
  std::ifstream in(filename);
  std::stringstream ss;
  ss << in.rdbuf();
  return ss.str();
}

}  // anonymous namespace

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
    } else if (name == "map_bounds") {
      deserializeMapBounds(node);
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
  out << " z=" << pos.z;
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
  id_type id;
  if (const auto* attr = node->first_attribute("id")) {
    id = getFromString<id_type>(attr->value());
    world->createEntity(id);
  } else {
    id = world->createEntity();
  }

  if (const auto* attr = node->first_attribute("type"))
    instantiateGeneric(std::string(attr->value()), id);

  if (const auto* cnode = node->first_node("position"))
    world->add(id, deserializePosition(cnode));

  if (const auto* cnode = node->first_node("speed"))
    world->add(id, deserializeSpeed(cnode));

  if (const auto* cnode = node->first_node("render"))
    world->add(id, deserializeRender(cnode));

  if (const auto* cnode = node->first_node("anim"))
    world->add(id, deserializeAnim(cnode));

  if (const auto* cnode = node->first_node("input"))
    world->add(id, deserializeInput(cnode));

  if (const auto* cnode = node->first_node("logic"))
    world->add(id, deserializeLogic(cnode));
}

void WorldXmlParser::deserializeGeneric(const xml_node* node) {
  const std::string& type(getAttrib<std::string>(node, "type"));
  generics.emplace(type, Generic(type));
  Generic& generic = generics[type];

  if (const auto* cnode = node->first_node("position"))
    generic.add(deserializePosition(cnode));

  if (const auto* cnode = node->first_node("speed"))
    generic.add(deserializeSpeed(cnode));

  if (const auto* cnode = node->first_node("render"))
    generic.add(deserializeRender(cnode));

  if (const auto* cnode = node->first_node("anim"))
    generic.add(deserializeAnim(cnode));

  if (const auto* cnode = node->first_node("input"))
    generic.add(deserializeInput(cnode));

  if (const auto* cnode = node->first_node("logic"))
    generic.add(deserializeLogic(cnode));
}

void WorldXmlParser::instantiateGeneric(const std::string& type, id_type id) {
  assert(generics.count(type));
  const Generic& generic = generics[type];

  if (generic.has<PositionComponent>())
    world->add(id, generic.get<PositionComponent>());
  if (generic.has<SpeedComponent>())
    world->add(id, generic.get<SpeedComponent>());
  if (generic.has<RenderComponent>())
    world->add(id, generic.get<RenderComponent>());
  if (generic.has<AnimComponent>())
    world->add(id, generic.get<AnimComponent>());
  if (generic.has<InputComponent>())
    world->add(id, generic.get<InputComponent>());
  if (generic.has<LogicComponent>())
    world->add(id, generic.get<LogicComponent>());
}

PositionComponent WorldXmlParser::deserializePosition(
    const xml_node* node) const {
  auto z = getAttrib<float>(node, "z");
  auto x = getAttrib<float>(node, "x");
  auto y = getAttrib<float>(node, "y");
  return PositionComponent(x, y, z);
}

SpeedComponent WorldXmlParser::deserializeSpeed(const xml_node* node) const {
  auto vx = getAttribOrDefault<float>(node, "vx", 0);
  auto vy = getAttribOrDefault<float>(node, "vy", 0);
  return SpeedComponent(vx, vy);
}

RenderComponent WorldXmlParser::deserializeRender(const xml_node* node) const {
  return RenderComponent(deserializeFrame(node->first_node("frame")));
}

AnimComponent WorldXmlParser::deserializeAnim(const xml_node* node) const {
  return AnimComponent(deserializeAnimation(node->first_node("animation")));
}

InputComponent WorldXmlParser::deserializeInput(const xml_node* node) const {
  auto id = getAttrib<int>(node, "script_id");
  return InputComponent(id);
}

LogicComponent WorldXmlParser::deserializeLogic(const xml_node* node) const {
  auto id = getAttrib<int>(node, "script_id");
  return LogicComponent(id);
}

Frame WorldXmlParser::deserializeFrame(const xml_node* node) const {
  auto tex_id = getAttrib<int>(node, "tex_id");
  auto width = getAttrib<int>(node, "width");
  auto height = getAttrib<int>(node, "height");
  auto tx = getAttribOrDefault<int>(node, "tx", 0);
  auto ty = getAttribOrDefault<int>(node, "ty", 0);
  return Frame(tex_id, width, height, tx, ty);
}

Animation WorldXmlParser::deserializeAnimation(const xml_node* node) const {
  std::vector<AnimFrame> frames;
  for (const auto* snode = node->first_node("animframe"); snode;
       snode = snode->next_sibling()) {
    frames.push_back(deserializeAnimFrame(snode));
  }
  auto repeated = getAttribOrDefault<bool>(node, "repeated", false);
  return Animation(frames, repeated);
}

AnimFrame WorldXmlParser::deserializeAnimFrame(const xml_node* node) const {
  auto duration_secs = getAttrib<float>(node, "duration");
  return AnimFrame(duration_secs, deserializeFrame(node->first_node("frame")));
}

void WorldXmlParser::deserializeMapBounds(const xml_node* node) const {
  sf::Vector2f lower(getAttrib<float>(node, "lower_x"),
                     getAttrib<float>(node, "lower_y"));
  sf::Vector2f upper(getAttrib<float>(node, "upper_x"),
                     getAttrib<float>(node, "upper_y"));
  world->variableLowerBounds() = lower;
  world->variableUpperBounds() = upper;
}
