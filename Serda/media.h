#ifndef SERDA_MEDIA_H
#define SERDA_MEDIA_H

#include <map>
#include <memory>

#include <SFML/Graphics/Font.hpp>

class Media {
  public:
    Media();
    struct Sound {
      enum ID {
        Error
      };
    };

    struct Music {
      enum ID {
        Error
      };
    };

    struct Font {
      enum ID {
        Error,
        Arial
      };
    };

    struct Texture {
      enum ID {
        Error
      };
    };

    const sf::Font* getFont(Font::ID fontID);

  private:
    // Sounds.
    // TODO.

    // Music.
    // TODO. There will only be one music object.

    // Textures.
    // TODO.

    // Fonts.
    std::map<Font::ID, std::string> font_files;
    std::map<Font::ID, std::unique_ptr<sf::Font>> fonts;
};

#endif // SERDA_MEDIA_H
