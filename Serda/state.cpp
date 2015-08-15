#include "state.h"

#include "state_stack.h"

using namespace sf;

State::Context::Context(RenderWindow* window, Media* media) : window(window), media(media) {}

State::State(StateStack* stack, Context context) : stack(stack), context(std::move(context)) {}

State::~State() {}

void State::activate() {}

void State::destroy() {}

void State::requestStackPush(States::ID stateID) {
  stack->pushState(stateID);
}

void State::requestStackPop() {
  stack->popState();
}

void State::requestStackClear() {
  stack->clearStates();
}

State::Context State::getContext() const {
  return context;
}
