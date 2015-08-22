#ifndef SERDA_GAME_STATE_H
#define SERDA_GAME_STATE_H

#include <SFML/Graphics/Text.hpp>

#include "state.h"
#include "world.h"

class GameState : public State {
  public:
    class Save {

    };

    GameState(StateStack* stack, Context context, Save save);

    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event& event);
    virtual void draw();

    // ECS Systems.
    void motionSystem(sf::Time dt);
    void renderSystem();

  private:
    Save save;
    World world;
};

#endif // SERDA_TITLE_STATE_H
