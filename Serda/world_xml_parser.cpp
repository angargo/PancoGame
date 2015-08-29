
#include "world_xml_parser.h"

#include <sstream>

WorldXmlParser::WorldXmlParser(World *world) : world(world) {}

void WorldXmlParser::deserialize_Entity(rapidxml::xml_node nodo){
  id_type id;
  if(nodo.first_attribute("id")){
    id = getFromString<int>(nodo.first_attribute("id")->value());
    world->createEntity(id);
  }
  else id = world->createEntity();
  if(nodo.first_node("position")) deserialize_Position(nodo.first_node("position"),id);
  if(nodo.first_node("speed")) deserialize_Speed(nodo.first_node("speed"),id);
  if(nodo.first_node("render")) deserialize_Render(nodo.first_node("render"),id);
  if(nodo.first_node("input")) deserialize_Input(nodo.first_node("input"),id);

}

void WorldXmlParser::deserialize_Position(rapidxml::xml_node nodo, id_type id){
  float x = getFromString<float>(nodo.first_attribute("x")->value());
  float y = getFromString<float>(nodo.first_attribute("y")->value());
  world->add(id,PositionComponent(id,x,y));
}

void WorldXmlParser::deserialize_Speed(rapidxml::xml_node nodo, id_type id){
  float vx = getFromString<float>(nodo.first_attribute("vx")->value());
  float vy = getFromString<float>(nodo.first_attribute("vy")->value());
  world->add(id,SpeedComponent(id,vx,vy));
}

void WorldXmlParser::deserialize_Render(rapidxml::xml_node nodo, id_type id){
  //TODO
}

void WorldXmlParser::deserialize_Input(rapidxml::xml_node nodo, id_type id){
  //TODO
}
