#include "game.h"

#include "game_state.h"
#include "title_state.h"
#include "menu_state.h"
#include "troll_state.h"

using namespace sf;
// 256x224 is default SNES resolution
Game::Game()
    : window(VideoMode(256, 224), "Serda"),
      textures("media/images/dictionary"),
      fonts("media/fonts/dictionary"),
      scripts("media/lua/dictionary"),
      stack(State::Context(&window, &textures, &fonts, &scripts)) {
  window.setSize(Vector2u(window.getSize().x * 2, window.getSize().y * 2));

  window.setKeyRepeatEnabled(false);

  registerStates();
  stack.pushState(States::Title);
}

Game::~Game() {}

void Game::run() {
  Clock clock;
  Time time_since_last_update = Time::Zero;
  // TODO: create options, get time_per_frame from the options.
  Time time_per_frame = seconds(1. / 60.);

  while (window.isOpen()) {
    Time dt = clock.restart();
    time_since_last_update += dt;
    while (time_since_last_update > time_per_frame) {
      time_since_last_update -= time_per_frame;

      processEvents();
      update(time_per_frame);

      if (stack.isEmpty()) window.close();
    }

    render();
  }
}

void Game::processEvents() {
  Event event;
  while (window.pollEvent(event)) {
    stack.handleEvent(event);

    if (event.type == Event::Closed) window.close();
  }
}

void Game::update(Time dt) { stack.update(dt); }

void Game::render() {
  window.clear();

  stack.draw();

  window.display();
}

void Game::registerStates() {
  stack.registerState<TrollState>(States::Troll);
  stack.registerState<TitleState>(States::Title);
  stack.registerState<MenuState>(States::Menu);
  // TODO: load saved games.
  stack.registerState<GameState>(States::Game, GameState::Save());
}
