#include "troll_state.h"

#include <cstdlib>

using namespace sf;

TrollState::TrollState(StateStack* stack, Context context) : State(stack, context),
  elapsed_time(sf::Time::Zero), color(Color::White) {}

bool TrollState::update(Time dt) {
  elapsed_time += dt;
  if (elapsed_time > seconds(1)) {
    elapsed_time = Time::Zero;
    color = Color(rand() % 256, rand() % 256, rand() % 256);
  }
  return true;
}

bool TrollState::handleEvent(const Event& event) {
  if (event.type == Event::KeyPressed and event.key.code == Keyboard::Escape) {
    requestStackPop();
  }
  return true;
}

void TrollState::draw() {
  const Context& context = getContext();
  RenderWindow& window = *context.window;
  window.clear(color);
}
