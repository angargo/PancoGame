#ifndef SERDA_MEDIA_H
#define SERDA_MEDIA_H

#include <map>
#include <memory>

#include <SFML/Graphics/Font.hpp>

class Media {
  public:
    Media();

    struct Font {
      enum ID {
        Error,
        Arial
      };
    };

    const sf::Image* getImage(int imageID);
    const sf::Font* getFont(Font::ID fontID);

  private:
    void loadDictionary(const std::string& path,
                        std::map<int, std::string>& files);

    // Sounds.
    // TODO.
    std::map<int, std::string> sound_files;

    // Music.
    // TODO. There will only be one music object.
    std::map<int, std::string> music_files;

    // Images
    std::map<int, std::string> image_files;
    std::map<int, std::unique_ptr<sf::Image>> images;

    // Fonts.
    std::map<Font::ID, std::string> font_files;
    std::map<Font::ID, std::unique_ptr<sf::Font>> fonts;
};

#endif // SERDA_MEDIA_H
