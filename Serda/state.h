#ifndef SERDA_STATE_H
#define SERDA_STATE_H

#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

#include "state_ids.h"
#include "media.h"

// Forward declaration.
class StateStack;

class State {
  public:
    struct Context {
      explicit Context(sf::RenderWindow* window, Media* media);
      sf::RenderWindow* window;
      Media* media;
    };

    State(StateStack* stack, Context context);
    virtual ~State();

    virtual void draw() = 0;
    virtual bool update(sf::Time dt) = 0;
    virtual bool handleEvent(const sf::Event& event) = 0;

    virtual void activate();
    virtual void destroy();

  protected:
    void requestStackPush(States::ID stateID);
    void requestStackPop();
    void requestStackClear();

    Context getContext() const;

  private:
    StateStack* stack;
    Context context;
};

#endif // SERDA_STATE_H
