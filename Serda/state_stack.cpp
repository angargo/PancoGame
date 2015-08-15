#include "state_stack.h"

#include <cassert>

StateStack::StateStack(State::Context context) : context(std::move(context)) {}

StateStack::~StateStack() {}

void StateStack::update(sf::Time dt) {
  for (auto itr = stack.rbegin(); itr != stack.rend(); ++itr) {
    if (not (*itr)->update(dt)) break;
  }

  applyPendingChanges();
}

void StateStack::handleEvent(const sf::Event& event) {
  for (auto itr = stack.rbegin(); itr != stack.rend(); ++itr) {
    if (not (*itr)->handleEvent(event)) break;
  }

  applyPendingChanges();
}

void StateStack::draw() {
  for (auto& state : stack) {
    state->draw();
  }
}

void StateStack::pushState(States::ID stateID) {
  pending_list.push_back(PendingChange(Push, stateID));
}

void StateStack::popState() {
  pending_list.push_back(PendingChange(Pop));
}

void StateStack::clearStates() {
  pending_list.push_back(PendingChange(Clear));
}

bool StateStack::isEmpty() const {
  return stack.empty();
}

std::unique_ptr<State> StateStack::createState(States::ID stateID) {
  assert(factories.count(stateID));
  return factories[stateID]();
}

void StateStack::applyPendingChanges() {
  for (PendingChange& change : pending_list) {
    switch (change.action) {
      case Push:
        stack.push_back(createState(change.stateID));
        break;

      case Pop:
        assert(not isEmpty());
        stack.back()->destroy();
        stack.pop_back();

        if (not isEmpty()) stack.back()->activate();
        break;

      case Clear:
        for (auto itr = stack.rbegin(); itr != stack.rend(); ++itr) {
          (*itr)->destroy();
        }
        stack.clear();
        break;
    }
  }

  pending_list.clear();
}

StateStack::PendingChange::PendingChange(Action action, States::ID stateID) : action(action),
  stateID(stateID) {}

