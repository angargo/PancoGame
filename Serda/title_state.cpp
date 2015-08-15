#include "title_state.h"

#include <cstdlib>

using namespace sf;

TitleState::TitleState(StateStack* stack, Context context) : State(stack, context),
  elapsed_time(sf::Time::Zero), color(Color::White) {}

bool TitleState::update(Time dt) {
  elapsed_time += dt;
  if (elapsed_time > seconds(1)) {
    elapsed_time = Time::Zero;
    color = Color(rand() % 256, rand() % 256, rand() % 256);
  }
  return true;
}

bool TitleState::handleEvent(const Event& event) {
  if (event.type == Event::KeyPressed and event.key.code == Keyboard::Escape) {
    requestStackPop();
  }
  return true;
}

void TitleState::draw() {
  const Context& context = getContext();
  RenderWindow& window = *context.window;
  window.clear(color);
}
