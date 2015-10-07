#include "media.h"

#include <cassert>
#include <fstream>
#include <sstream>

Media::Media() {
  font_files[Font::Arial] = "media/fonts/arial.ttf";

  loadDictionary("media/sounds/dictionary", sound_files);
  loadDictionary("media/music/dictionary", sound_files);
  loadDictionary("media/images/dictionary", sound_files);
}

const sf::Image* Media::getImage(int imageID) {
  if (images.count(imageID) == 0) {
    auto it = image_files.find(imageID);
    assert(it != image_files.end());
    images[imageID].reset(new sf::Image);
    images[imageID]->loadFromFile(it->second);
  }
  return images[imageID].get();
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

void Media::loadDictionary(const std::string& path,
                           std::map<int, std::string>& files) {
  std::ifstream in(path);
  assert(in.good());
  std::string line;
  while (getline(in, line)) {
    std::stringstream ss(line);
    int id;
    std::string file;
    ss >> id >> file;
    files.emplace(id, file);
  }
}
