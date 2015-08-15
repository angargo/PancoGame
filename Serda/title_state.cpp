#include "title_state.h"

#include <cstdlib>

TitleState::TitleState(StateStack* stack, Context context) : State(stack, context),
  elapsed_time(sf::Time::Zero), color(sf::Color::White),
  font(context.media->getFont(Media::Font::Arial)),
  first_time(true) {

  text.setFont(*font);
  text.setString("The Legend of Serda: A Limp to the Panco");
  text.setCharacterSize(30);
  text.setColor(sf::Color::Blue);
  text.setStyle(sf::Text::Bold);


  // Center text.
  sf::FloatRect textRect = text.getLocalBounds();
  text.setOrigin(textRect.left + textRect.width / 2,
                 textRect.top  + textRect.height / 2);
  sf::Vector2i size(context.window->getSize());
  text.setPosition(size.x / 2, size.y / 2);
}

bool TitleState::update(sf::Time dt) {
  if (first_time) {
    requestStackPush(States::Troll);
    first_time = false;
  }

  elapsed_time += dt;
  if (elapsed_time > sf::seconds(3)) {
    
  }
  return false;
}

bool TitleState::handleEvent(const sf::Event& event) {
  if (event.type == sf::Event::KeyPressed and event.key.code == sf::Keyboard::Escape) {
    requestStackPop();
  }
  return false;
}

void TitleState::draw() {
  const Context& context = getContext();
  context.window->clear(sf::Color::White);
  context.window->draw(text);
}

