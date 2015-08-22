#include "game_state.h"

#include <cstdlib>

GameState::GameState(StateStack* stack, Context context, Save save)
    : State(stack, context), save(std::move(save)) {
  // Temporal code for testing. TODO: remove.
  auto window = context.window;
  id_type link_id = world.createEntity();

  sf::Vector2u size = window->getSize();
  world.addComponent(link_id, PositionComponent(size.x / 2, size.y / 2));

  texture.loadFromFile("media/img/link_front1.png");
  world.addComponent(link_id, RenderComponent(sf::Sprite(texture)));
}

bool GameState::update(sf::Time dt) {
  motionSystem(dt);

  //world.update();

  return false;
}

bool GameState::handleEvent(const sf::Event& event) {
  if (event.type == sf::Event::KeyPressed) {
    inputSystem(event.key.code);
  }
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
  
  for (const Entity& entity : world.getEntities()) {
    if ((entity.components & skey) == skey) {

    }
  }
}

void GameState::inputSystem(sf::Keyboard::Key key) {
  static std::bitset<Component::NUM_IDS> skey(createBitset(Component::INPUT));
}

void GameState::renderSystem() {
  static std::bitset<Component::NUM_IDS> skey(createBitset(
        Component::POSITION, Component::RENDER));

  sf::RenderWindow* window = getContext().window;
  for (const Entity& entity : world.getEntities()) {
    if ((entity.components & skey) == skey) {
      auto& render = static_cast<RenderComponent&>(
          world.mutableComponent(entity, Component::RENDER));
      const auto& position = static_cast<const PositionComponent&>(
          world.getComponent(entity, Component::POSITION));
      render.sprite.setPosition(position.x, position.y);
      window->draw(render.sprite);
    }
  }
}
