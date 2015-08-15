#include "game.h"

#include "troll_state.h"

using namespace sf;

Game::Game() : window(VideoMode(640, 480), "Serda"),
               stack(State::Context(&window, &media)) {
  registerStates();
  stack.pushState(States::Troll);
}

Game::~Game() {}

void Game::run() {
  Clock clock;
  Time time_since_last_update = Time::Zero;
  // TODO: create options, get time_per_frame from the options.
  Time time_per_frame = seconds(1./60.);

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

void Game::update(Time dt) {
  stack.update(dt);
}

void Game::render() {
  window.clear();

  stack.draw();

  window.display();
}

void Game::registerStates() {
  stack.registerState<TrollState>(States::Troll);
}
