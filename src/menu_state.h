#ifndef SERDA_MENU_STATE_H
#define SERDA_MENU_STATE_H

#include <vector>

#include <SFML/Graphics.hpp>

#include "state.h"

class MenuState : public State {
 public:
  MenuState(StateStack* stack, Context context);

  virtual bool update(sf::Time dt);
  virtual bool handleEvent(const sf::Event& event);
  virtual void draw();

 private:
  const sf::Font& font;
  unsigned index;
  std::vector<sf::Text> options;
};

#endif  // SERDA_MENU_STATE_H
