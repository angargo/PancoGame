#ifndef SERDA_TROLL_STATE_H
#define SERDA_TROLL_STATE_H

#include "state.h"

class TrollState : public State {
  public:
    TrollState(StateStack* stack, Context context);

    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event& event);
    virtual void draw();

  private:
    sf::Time elapsed_time;
    sf::Color color;
};

#endif // SERDA_TROLL_STATE_H
