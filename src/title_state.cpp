#include "title_state.h"

#include <cstdlib>

TitleState::TitleState(StateStack* stack, Context context)
    : State(stack, context),
      first_time(true) {
  banner.setTexture(*context.textures->get(1));

  // Center text.
  sf::FloatRect rect = banner.getLocalBounds();
  banner.setOrigin(rect.left + rect.width / 2,
                 rect.top + rect.height / 2);
  sf::Vector2f size(context.window->getView().getSize());
  banner.setPosition(size.x / 2, size.y / 2);
}

bool TitleState::update(sf::Time dt) {
  if (first_time) {
    requestStackPush(States::Troll);
    first_time = false;
  }
  return false;
}

bool TitleState::handleEvent(const sf::Event& event) {
  if (event.type == sf::Event::KeyPressed) {
    if (event.key.code == sf::Keyboard::Escape) {
      requestStackPop();
    } else if (event.key.code == sf::Keyboard::Return) {
      requestStackClear();
      requestStackPush(States::Game);
    }
  }
  return false;
}

void TitleState::draw() {
  const Context& context = getContext();
  context.window->clear(sf::Color::White);
  context.window->draw(banner);
}
