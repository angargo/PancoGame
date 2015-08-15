#ifndef SERDA_TITLE_STATE_H
#define SERDA_TITLE_STATE_H

#include <SFML/Graphics/Text.hpp>

#include "state.h"

class TitleState : public State {
  public:
    TitleState(StateStack* stack, Context context);

    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event& event);
    virtual void draw();

  private:
    sf::Time elapsed_time;
    sf::Color color;
    const sf::Font* font;
    sf::Text text;
    bool first_time;
};

#endif // SERDA_TITLE_STATE_H
