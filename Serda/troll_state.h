#ifndef SERDA_TROLL_STATE_H
#define SERDA_TROLL_STATE_H

#include <vector>

#include <SFML/Graphics.hpp>

#include "state.h"

class TrollState : public State {
  public:
    TrollState(StateStack* stack, Context context);

    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event& event);
    virtual void draw();

  private:
    sf::Time elapsed_time;

    const int num_circles;
    std::vector<sf::CircleShape> circles;
    std::vector<float> angles;

    float speed;
    int width, height;
};

#endif // SERDA_TROLL_STATE_H
