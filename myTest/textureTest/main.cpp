#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

int main(){
  RenderWindow window(VideoMode(640,480), "TextureTest");
  Texture linkTexture;
  if(!linkTexture.loadFromFile("link.png",IntRect(98,1025,22,28))){
    cerr << "Loading Error" << endl;
    window.close();
  }
  Sprite link(linkTexture);
  link.setPosition(100.f, 100.f);
  while(window.isOpen()){
    window.clear();
    window.draw(link);
    window.display();
  }
}
