#ifndef SERDA_TITLE_STATE_H
#define SERDA_TITLE_STATE_H

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "state.h"

class TitleState : public State {
 public:
  TitleState(StateStack* stack, Context context);

  virtual bool update(sf::Time dt);
  virtual bool handleEvent(const sf::Event& event);
  virtual void draw();

 private:
  sf::Sprite banner;
  sf::Texture texture;
  bool first_time;
};

#endif  // SERDA_TITLE_STATE_H
