#include "menu_state.h"

using namespace sf;

MenuState::MenuState(StateStack* stack, Context context)
    : State(stack, context),
      font(*context.fonts->get(Fonts::Arial)),
      index(0),
      options{{"Start", font}, {"Exit", font}} {
  sf::Vector2i size(context.window->getView().getSize());

  if (not options.empty()) {
    sf::FloatRect textRect = options[0].getLocalBounds();
    int total_height = (2 * options.size() - 1) * textRect.height;
    int offset = (size.y - total_height) / 2;
    int entry_step = 2 * textRect.height;

    for (auto& option : options) {
      option.setOrigin(textRect.left + textRect.width / 2,
                       textRect.top + textRect.height / 2);
      option.setPosition(size.x / 2, offset);
      offset += entry_step;
    }
  }
}

bool MenuState::update(Time dt) {
  for (auto& option : options) option.setColor(sf::Color::White);
  options[index].setColor(sf::Color::Red);
  return false;
}

bool MenuState::handleEvent(const Event& event) {
  if (event.type == Event::KeyPressed) {
    switch (event.key.code) {
      case sf::Keyboard::Escape:
        requestStackPop();
        break;
      case sf::Keyboard::Return:
        requestStackClear();
        if (index == 0) {
          requestStackPush(States::Game);
        }
        break;
      case sf::Keyboard::Up:
        index = (index + 1) % options.size();
        break;
      case sf::Keyboard::Down:
        index = (index + options.size() - 1) % options.size();
        break;
      default:
        break;
    }
  }

  return true;
}

void MenuState::draw() {
  const auto& window = getContext().window;
  window->clear(sf::Color::Black);
  for (auto& option : options) window->draw(option);
}
