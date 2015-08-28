#include "game_state.h"

#include <cstdlib>

#include "keyboard.h"

GameState::GameState(StateStack* stack, Context context, Save save)
    : State(stack, context), save(std::move(save)) {
  // Temporal code for testing. TODO: remove.
  auto window = context.window;
  id_type link_id = world.createEntity();

  sf::Vector2f size = window->getView().getSize();
  world.add(link_id, PositionComponent(size.x / 2, size.y / 2));

  sf::Vector2u link_size = context.textures->get(2)->getSize();
  world.add(link_id, RenderComponent(2, link_size.x, link_size.y));
  world.add(link_id, SpeedComponent(0, 0));
  // Setup link test animation.
  std::vector<AnimFrame> frames;
  const int num_frames = 8;
  const sf::Time frame_time(sf::seconds(0.08));
  for (int i = 0; i < num_frames; ++i) {
    frames.push_back(AnimFrame(frame_time, Frame(3, 16, 25, 3 + 30*i, 32)));
  }
  world.add(link_id, AnimComponent(Animation(frames, true)));

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
  animSystem(dt);

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

void GameState::animSystem(sf::Time dt) {
  static const std::bitset<Component::NUM_IDS> skey(
      createBitset(Component::RENDER, Component::ANIM));
  for (const Entity& entity : world.getEntities()) {
    if ((entity.components & skey) == skey) {
      auto& anim = world.variable<AnimComponent>(entity);
      auto& render = world.variable<RenderComponent>(entity);
      Animation& animation = anim.getAnimation();
      sf::Time remaining = dt;
      while (remaining > sf::Time::Zero &&
             unsigned(anim.index) < animation.frames.size()) {
        AnimFrame& frame = animation.frames[anim.index];
        sf::Time diff =
            std::min(frame.duration - frame.elapsed_time, remaining);
        remaining -= diff;
        frame.elapsed_time += diff;
        // Advance frame.
        if (frame.elapsed_time >= frame.duration) {
          frame.elapsed_time = sf::Time::Zero;
          ++anim.index;
          if (unsigned(anim.index) >= animation.frames.size()) {
            --anim.index;
            if (animation.repeated) {
              anim.index = 0;
            }
          }
          // Set new frame.
          render.getFrame() = animation.frames[anim.index].frame;
        }
      }
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
  const auto& textures = *getContext().textures;
  for (const Entity& entity : world.getEntities()) {
    if ((entity.components & skey) == skey) {
      auto& render = world.variable<RenderComponent>(entity);
      const auto& position = world.get<PositionComponent>(entity);
      sf::Sprite sprite(*textures.get(render.textureId()));
      sprite.setTextureRect(
          sf::IntRect(render.tx(), render.ty(), render.width(), render.height()));
      sprite.setPosition(position.x, position.y);
      window->draw(sprite);
    }
  }
}
