#ifndef SERDA_GAME_STATE_H
#define SERDA_GAME_STATE_H

#include <SFML/Graphics/Text.hpp>

#include "state.h"
#include "world.h"
#include "world_xml_parser.h"

class GameState : public State {
 public:
  class Save {};

  GameState(StateStack* stack, Context context, Save save);

  virtual bool update(sf::Time dt);
  virtual bool handleEvent(const sf::Event& event);
  virtual void draw();

  // ECS Systems.
  void motionSystem(sf::Time dt);
  void animSystem(sf::Time dt);
  void inputSystem(InputEvent input_event);
  void renderSystem();

 private:
  Save save;
  World world;
  WorldXmlParser wxp;
  // TODO: remove
  sf::Texture texture;
};

#endif  // SERDA_TITLE_STATE_H
