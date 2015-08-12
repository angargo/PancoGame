#include<SFML/Graphics.hpp>


int main(){
  sf::RenderWindow window(sf::VideoMode (640,420), "Draws a circle");
  sf::CircleShape circle;
  circle.setRadius(40.f);
  circle.setPosition(200.f,200.f);
  circle.setFillColor(sf::Color::Cyan);
  while(window.isOpen()){
    sf::Event event;
    while(window.pollEvent(event)){
      if (event.type == sf::Event::Closed) window.close();
      window.clear();
      window.draw(circle);
      window.display();
    }
  }
}
