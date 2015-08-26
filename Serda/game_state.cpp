#include "game_state.h"

#include <cstdlib>

GameState::GameState(StateStack* stack, Context context, Save save)
    : State(stack, context), save(std::move(save)) {
  // Temporal code for testing. TODO: remove.
  auto window = context.window;
  id_type link_id = world.createEntity();

  sf::Vector2u size = window->getSize();
  world.add(link_id, PositionComponent(size.x / 2, size.y / 2));

  texture.loadFromFile("media/images/link_front1.png");
  world.add(link_id, RenderComponent(sf::Sprite(texture)));

  world.add(link_id, SpeedComponent(0, 0));

  InputComponent input;
  const int sp = 100;
  input.bindings[InputEvent(InputEvent::KEY_PRESSED, sf::Keyboard::Up)] =
      [](World* world, id_type link) {
        world->variable<SpeedComponent>(link).addSpeed(0, -sp);
      };
  input.bindings[InputEvent(InputEvent::KEY_RELEASED, sf::Keyboard::Up)] =
      [](World* world, id_type link) {
        world->variable<SpeedComponent>(link).addSpeed(0, sp);
      };
  input.bindings[InputEvent(InputEvent::KEY_PRESSED, sf::Keyboard::Down)] =
      [](World* world, id_type link) {
        world->variable<SpeedComponent>(link).addSpeed(0, sp);
      };
  input.bindings[InputEvent(InputEvent::KEY_RELEASED, sf::Keyboard::Down)] =
      [](World* world, id_type link) {
        world->variable<SpeedComponent>(link).addSpeed(0, -sp);
      };
  input.bindings[InputEvent(InputEvent::KEY_PRESSED, sf::Keyboard::Right)] =
      [](World* world, id_type link) {
        world->variable<SpeedComponent>(link).addSpeed(sp, 0);
      };
  input.bindings[InputEvent(InputEvent::KEY_RELEASED, sf::Keyboard::Right)] =
      [](World* world, id_type link) {
        world->variable<SpeedComponent>(link).addSpeed(-sp, 0);
      };
  input.bindings[InputEvent(InputEvent::KEY_PRESSED, sf::Keyboard::Left)] =
      [](World* world, id_type link) {
        world->variable<SpeedComponent>(link).addSpeed(-sp, 0);
      };
  input.bindings[InputEvent(InputEvent::KEY_RELEASED, sf::Keyboard::Left)] =
      [](World* world, id_type link) {
        world->variable<SpeedComponent>(link).addSpeed(sp, 0);
      };
  world.add(link_id, input);
}

bool GameState::update(sf::Time dt) {
  motionSystem(dt);

  // world.update();

  return false;
}

bool GameState::handleEvent(const sf::Event& event) {
  if (event.type == sf::Event::KeyPressed ||
      event.type == sf::Event::KeyReleased) {
    InputEvent::KeyAction action = InputEvent::KEY_PRESSED;
    if (event.type == sf::Event::KeyReleased) action = InputEvent::KEY_RELEASED;
    InputEvent input_event(action, event.key.code);
    inputSystem(input_event);
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

template <typename... Args>
static std::bitset<Component::NUM_IDS> createBitset(Component::Id first,
                                                    Args... args) {
  auto result = createBitset(args...);
  result.set(first);
  return result;
}

void GameState::motionSystem(sf::Time dt) {
  static const std::bitset<Component::NUM_IDS> skey(
      createBitset(Component::POSITION, Component::SPEED));

  for (const Entity& entity : world.getEntities()) {
    if ((entity.components & skey) == skey) {
      auto& pos = world.variable<PositionComponent>(entity);
      const auto& speed = world.get<SpeedComponent>(entity);
      pos.x += (dt.asSeconds() * speed.vx);
      pos.y += (dt.asSeconds() * speed.vy);
    }
  }
}

void GameState::inputSystem(InputEvent event) {
  static const std::bitset<Component::NUM_IDS> skey(
      createBitset(Component::INPUT));

  for (const Entity& entity : world.getEntities()) {
    if ((entity.components & skey) == skey) {
      auto& input = world.variable<InputComponent>(entity).bindings;
      if (input.find(event) != input.end()) {
        input[event](&world, entity.id);
      }
    }
  }
}

void GameState::renderSystem() {
  static const std::bitset<Component::NUM_IDS> skey(
      createBitset(Component::POSITION, Component::RENDER));

  sf::RenderWindow* window = getContext().window;
  for (const Entity& entity : world.getEntities()) {
    if ((entity.components & skey) == skey) {
      auto& render = world.variable<RenderComponent>(entity);
      const auto& position = world.get<PositionComponent>(entity);
      render.sprite.setPosition(position.x, position.y);
      window->draw(render.sprite);
    }
  }
}
