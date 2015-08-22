#ifndef SERDA_STATE_STACK_H
#define SERDA_STATE_STACK_H

#include <functional>
#include <map>
#include <memory>
#include <tuple>
#include <vector>

#include "state.h"
#include "state_ids.h"

class StateStack {
  public:
    enum Action {
      Push,
      Pop,
      Clear
    };

    explicit StateStack(State::Context context);
    ~StateStack();

    // TODO: make this work with variadic templates.
    template<typename T> void registerState(States::ID stateID);
    template<typename T, typename Arg> void registerState(States::ID stateID, Arg arg);

    void update(sf::Time dt);
    void handleEvent(const sf::Event& event);
    void draw();

    void pushState(States::ID stateID);
    void popState();
    void clearStates();

    bool isEmpty() const;

  private:
    std::unique_ptr<State> createState(States::ID stateID);
    void applyPendingChanges();

    struct PendingChange {
        explicit PendingChange(Action action, States::ID stateID = States::None);
        Action action;
        States::ID stateID;
    };

    std::vector<std::unique_ptr<State>> stack;
    std::vector<PendingChange> pending_list;
    State::Context context;
    std::map<States::ID, std::function<std::unique_ptr<State>()>> factories;
};

template<typename T>
void StateStack::registerState(States::ID stateID) {
  factories[stateID] = [this]() {
    return std::unique_ptr<T>(new T(this, context));
  };
}

template<typename T, typename Arg>
void StateStack::registerState(States::ID stateID, Arg arg) {
  factories[stateID] = [this, arg]() {
    return std::unique_ptr<T>(new T(this, context, arg));
  };
}

#endif // SERDA_STATE_STACK_H
