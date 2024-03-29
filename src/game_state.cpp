#include "game_state.h"

#include <algorithm>
#include <cstdlib>
#include <set>
#include <vector>

#include "keyboard.h"
#include "joystick.h"

GameState::GameState(StateStack* stack, Context context, Save save)
    : State(stack, context), save(std::move(save)), wxp(&world) {
  // TODO: I don't like this.
  const int lua_init_id = 4;
  // We need to run the init script before all other scripts.
  context.scripts->loadScript(world.getL(), lua_init_id);
  wxp.loadWorld(1);
  world.variableLinkId() = 1;
}

bool GameState::update(sf::Time dt) {
  logicSystem(dt);
  motionSystem(dt);
  animSystem(dt);
  collisionSystem();

  return false;
}

bool GameState::handleEvent(const sf::Event& event) {
  if (event.type == sf::Event::KeyPressed ||
      event.type == sf::Event::KeyReleased) {
    InputEvent::KeyAction action = InputEvent::KEY_PRESSED;
    if (event.type == sf::Event::KeyReleased) action = InputEvent::KEY_RELEASED;
    InputEvent input_event(action, event.key.code);
    inputSystem(input_event);
  } else if (event.type == sf::Event::JoystickMoved) {
    InputEvent input_event(event.joystickMove);
    inputSystem(input_event);
  } else if (event.type == sf::Event::LostFocus ||
             event.type == sf::Event::GainedFocus) {
    // On focus gain, send all keys to be pressed, on focus lost, send all keys
    // to be released, to avoid weird behaviour.
    for (int idx = 0; idx < sf::Keyboard::Key::KeyCount; ++idx) {
      InputEvent::KeyAction ievent = event.type == sf::Event::GainedFocus
                                         ? InputEvent::KEY_PRESSED
                                         : InputEvent::KEY_RELEASED;
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

void GameState::logicSystem(sf::Time dt) {
  static const std::bitset<Component::NUM_IDS> skey(
      createBitset(Component::LOGIC));

  for (const Entity& entity : world.getEntities()) {
    if ((entity.components & skey) == skey) {
      const auto& logic = world.get<LogicComponent>(entity);
      getContext().scripts->runScript(world.getL(), logic.script_id, entity.id);
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
      // If RenderComponent has no frame set, set the first from Animation.
      // This is detected by texture_id being -1.
      if (render.getFrame().texture_id == Frame::NONE) {
        render.variableFrame() = animation.frames.begin()->frame;
      }
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
      const auto& input = world.get<InputComponent>(entity);
      if (event.key_pressed == InputEvent::JOY_MOVED) {
        if (JoyToStr.count(event.axis))
          getContext().scripts->runScript(world.getL(), input.script_id,
                                          entity.id, JoyToStr.at(event.axis),
                                          event.position);
      } else {
        if (KeyToStr.count(event.key))
          getContext().scripts->runScript(world.getL(), input.script_id,
                                          entity.id, KeyToStr.at(event.key),
                                          static_cast<bool>(event.key_pressed));
      }
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
  std::vector<sf::Sprite> sprites;
  // Aux vector to sort sprites by priority before drawing.
  std::vector<std::pair<std::pair<float, float>, int>> sorted_sprites;
  for (const Entity& entity : world.getEntities()) {
    if ((entity.components & skey) == skey) {
      // TODO: put this into a function.
      auto& render = world.variable<RenderComponent>(entity);
      const auto& position = world.get<PositionComponent>(entity);
      sf::Sprite sprite(*textures.get(render.textureId()));
      sprite.setTextureRect(sf::IntRect(render.tx(), render.ty(),
                                        render.width(), render.height()));
      sprite.setPosition(position.x + offset.x, position.y + offset.y);
      sprites.push_back(sprite);
      sorted_sprites.push_back(std::pair<std::pair<float, float>, int>(
          std::pair<float, float>(position.z, position.y),
          sorted_sprites.size()));
    }
  }
  std::sort(sorted_sprites.begin(), sorted_sprites.end());
  for (int i = 0; i < int(sprites.size()); ++i) {
    window->draw(sprites[sorted_sprites[i].second]);
  }
}

// CollisionInfo
GameState::CollisionInfo::CollisionInfo(const Entity& entity_a,
                                        const Entity& entity_b)
    : entity_a(entity_a.id), entity_b(entity_b.id) {}

GameState::CollisionInfo::CollisionInfo(id_type entity_a, id_type entity_b)
    : entity_a(entity_a), entity_b(entity_b) {}

bool GameState::CollisionInfo::operator<(const CollisionInfo& other) const {
  return std::minmax(entity_a, entity_b) <
         std::minmax(other.entity_a, other.entity_b);
}

void GameState::collisionSystem() {
  static const std::bitset<Component::NUM_IDS> skey(
      createBitset(Component::POSITION, Component::COLLISION));

  std::set<CollisionInfo> collisions;
  for (auto entity_pair : world.getEntityPairs()) {
    const Entity& entity_a = entity_pair.first;
    const Entity& entity_b = entity_pair.second;
    if ((entity_a.components & skey) != skey) {
      continue;
    }
    if ((entity_b.components & skey) != skey) {
      continue;
    }
    // TODO update hitbox to contain absolute position.
    const CollisionComponent& collision_a =
        world.get<CollisionComponent>(entity_a);
    Hitbox hitbox_a = collision_a.hitbox;
    // Hitbox lHitbox = lCollision.add(world.get<PositionComponent>(lhs);

    const CollisionComponent& collision_b =
        world.get<CollisionComponent>(entity_b);
    Hitbox hitbox_b = collision_b.hitbox;
    // Hitbox rHitbox = rCollision.add(world.get<PositionComponent>(rhs);
    if (hitbox_a.collidesWith(hitbox_b)) {
      // Calculate any desired information about collision.
      collisions.insert(CollisionInfo(entity_a, entity_b));
    }
  }
  // Now, treat collisions.
}
