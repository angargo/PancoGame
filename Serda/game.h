#ifndef SERDA_GAME_H
#define SERDA_GAME_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window.hpp>

#include "state_stack.h"
#include "resource_manager.h"
#include "SFML/Graphics.hpp"

class Game {
 public:
  Game();
  ~Game();

  void run();

  void processEvents();
  void update(sf::Time dt);
  void render();

  void registerStates();

 private:
  sf::RenderWindow window;
  // Resources.
  TextureManager textures;
  FontManager fonts;
  LuaManager scripts;

  // Stack needs to be last member, because previous members have to be
  // initialized.
  StateStack stack;
};

#endif  // SERDA_GAME_H
