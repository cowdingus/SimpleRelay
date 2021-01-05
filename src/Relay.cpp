#include "Relay.hpp"

#include <Arduino.h>

Relay::Relay(unsigned int pin, Relay::State state)
  : pin(pin), state(state)
{
  pinMode(pin, OUTPUT);

  if (state == State::Down) setDown(); else setUp();
}

void Relay::setUp() 
{
  digitalWrite(pin, HIGH);
  state = State::Up;
}

void Relay::setDown()
{
  digitalWrite(pin, LOW);
  state = State::Down;
}

Relay::State Relay::getState() const
{
  return state;
}

String stringify(const Relay& relay) 
{
  return String() + "RLY " + relay.pin + ", " + static_cast<int>(relay.state);
}
