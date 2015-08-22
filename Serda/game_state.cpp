#include "game_state.h"

#include <cstdlib>

GameState::GameState(StateStack* stack, Context context, Save save)
  : State(stack, context), save(std::move(save)) {
}

bool GameState::update(sf::Time dt) {
  motionSystem(dt);

  return false;
}

bool GameState::handleEvent(const sf::Event& event) {
  return false;
}

void GameState::draw() {
  const Context& context = getContext();
  context.window->clear(sf::Color::Green);

  renderSystem();
}

static std::bitset<Component::NUM_IDS> createBitset() {
  return std::bitset<Component::NUM_IDS>();
}

template<typename... Args>
static std::bitset<Component::NUM_IDS> createBitset(Component::Id first,
                                                    Args... args) {
  auto result = createBitset(args...);
  result.set(first);
  return result;
}

void GameState::motionSystem(sf::Time dt) {
  static std::bitset<Component::NUM_IDS> skey(createBitset(
        Component::POSITION, Component::SPEED));
  
}

void GameState::renderSystem() {
  static std::bitset<Component::NUM_IDS> skey(createBitset(
        Component::POSITION, Component::RENDER));
}
