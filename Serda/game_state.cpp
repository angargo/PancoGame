#include "game_state.h"

#include <cstdlib>

GameState::GameState(StateStack* stack, Context context, Save save)
  : State(stack, context), save(std::move(save)) {
}

bool GameState::update(sf::Time dt) {
  return false;
}

bool GameState::handleEvent(const sf::Event& event) {
  return false;
}

void GameState::draw() {
  const Context& context = getContext();
  context.window->clear(sf::Color::Green);
}

