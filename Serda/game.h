#ifndef SERDA_GAME_H
#define SERDA_GAME_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window.hpp>

#include "state_stack.h"

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
    Media media;
    StateStack stack;
};

#endif // SERDA_GAME_H
