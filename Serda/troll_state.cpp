#include "troll_state.h"

#include <cmath>
#include <cstdlib>

using namespace sf;

TrollState::TrollState(StateStack* stack, Context context)
    : State(stack, context),
      elapsed_time(sf::Time::Zero),
      num_circles(10),
      circles(num_circles),
      angles(num_circles) {
  Vector2u size = context.window->getSize();
  width = size.x;
  height = size.y;
  for (int i = 0; i < num_circles; ++i) {
    circles[i].setPosition(rand() % width, rand() % height);
    circles[i].setFillColor(sf::Color::Black);
    angles[i] = M_PI * float(rand() % 360) / 360.;
    circles[i].setRadius(10);
  }
  speed = 2;
}

bool TrollState::update(Time dt) {
  elapsed_time += dt;
  for (auto& circle : circles) {
    if (rand() % 60 == 0) {
      circle.setFillColor(Color(rand() % 256, rand() % 256, rand() % 256));
    }
  }

  for (int i = 0; i < num_circles; ++i) {
    angles[i] += float(rand() % 100 - 50) / 10000.;
    sf::Vector2f pos = circles[i].getPosition();
    pos.x += speed * cos(angles[i]);
    pos.y += speed * sin(angles[i]);
    while (pos.x < 0) pos.x += width;
    while (pos.y < 0) pos.y += height;
    pos.x -= width * int(int(pos.x) / width);
    pos.y -= height * int(int(pos.y) / height);
    circles[i].setPosition(pos);
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
  for (const auto& circle : circles) window.draw(circle);
}
