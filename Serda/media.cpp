#include "media.h"

#include <cassert>

Media::Media() {
  font_files[Font::Arial] = "media/fonts/arial.ttf";
}

const sf::Font* Media::getFont(Font::ID fontID) {
  if (fonts.count(fontID) == 0) {
    auto it = font_files.find(fontID);
    assert(it != font_files.end());
    fonts[fontID].reset(new sf::Font);
    fonts[fontID]->loadFromFile(it->second);
  }
  return fonts[fontID].get();
}
