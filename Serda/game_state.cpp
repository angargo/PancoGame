#include "game_state.h"

#include <cstdlib>

#include "keyboard.h"

GameState::GameState(StateStack* stack, Context context, Save save)
    : State(stack, context), save(std::move(save)), wxp(&world) {
  // Temporal code for testing. TODO: remove.
  auto window = context.window;
  id_type link_id = world.createEntity();
  world.variableLinkId() = link_id;

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
  // Setup background.
  id_type map = world.createEntity();
  world.add(map, PositionComponent(0, 0));
  sf::Vector2u map_size = context.textures->get(5)->getSize();
  world.add(map, RenderComponent(5, map_size.x, map_size.y));
  // Set boundaries.
  world.variableUpperBounds().x = 6.0f * window->getView().getSize().x;
  world.variableUpperBounds().y = 4.0f * window->getView().getSize().y;

  InputComponent input(2);
  world.add(link_id, input);
}

bool GameState::update(sf::Time dt) {
  motionSystem(dt);
  animSystem(dt);

  return false;
}

bool GameState::handleEvent(const sf::Event& event) {
  if (event.type == sf::Event::KeyPressed ||
      event.type == sf::Event::KeyReleased) {
    InputEvent::KeyAction action = InputEvent::KEY_PRESSED;
    if (event.type == sf::Event::KeyReleased) action = InputEvent::KEY_RELEASED;
    InputEvent input_event(action, event.key.code);
    inputSystem(input_event);
  } else if (event.type == sf::Event::LostFocus ||
             event.type == sf::Event::GainedFocus) {
    // On focus gain, send all keys to be pressed, on focus lost, send all keys
    // to be released, to avoid weird behaviour.
    for (int idx = 0; idx < sf::Keyboard::Key::KeyCount; ++idx) {
      InputEvent::KeyAction ievent = event.type == sf::Event::GainedFocus ?
          InputEvent::KEY_PRESSED : InputEvent::KEY_RELEASED;
      sf::Keyboard::Key key = sf::Keyboard::Key(idx);
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(key))) {
        inputSystem(InputEvent(ievent, key));
      }
    }
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
      Animation& animation = anim.variableAnimation();
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
          render.variableFrame() = animation.frames[anim.index].frame;
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
      const auto& input = world.variable<InputComponent>(entity);
      getContext().scripts->runScript(world.getL(), input.script_id, entity.id,
                                      KeyToStr.at(event.key),
                                      event.key_pressed);
    }
  }
}

void GameState::renderSystem() {
  static const std::bitset<Component::NUM_IDS> skey(
      createBitset(Component::POSITION, Component::RENDER));

  // Get link position to calculate offset.
  sf::RenderWindow* window = getContext().window;
  const sf::Vector2f pos(world.get<PositionComponent>(world.linkId()).x,
                         world.get<PositionComponent>(world.linkId()).y);
  const sf::Vector2f bounds(world.upperBounds().x, world.upperBounds().y);
  const sf::Vector2f view(window->getView().getSize());
  sf::Vector2f offset(
      std::max(std::min(world.lowerBounds().x, view.x / 2.0f - pos.x),
               view.x - bounds.x),
      std::max(std::min(world.lowerBounds().y, view.y / 2.0f - pos.y),
               view.y - bounds.y));
  const auto& textures = *getContext().textures;
  for (const Entity& entity : world.getEntities()) {
    if ((entity.components & skey) == skey) {
      auto& render = world.variable<RenderComponent>(entity);
      const auto& position = world.get<PositionComponent>(entity);
      sf::Sprite sprite(*textures.get(render.textureId()));
      sprite.setTextureRect(
          sf::IntRect(render.tx(), render.ty(), render.width(), render.height()));
      sprite.setPosition(position.x + offset.x, position.y + offset.y);
      window->draw(sprite);
    }
  }
}
