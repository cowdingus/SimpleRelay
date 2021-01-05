#pragma once

#include <WString.h>

class Relay
{
public:
  enum class State
  {
    Down,
    Up
  };

  Relay(unsigned int pin, State state = State::Down);

  void setUp();

  void setDown();

  State getState() const;

  friend String stringify(const Relay& relay);
  
private:
  unsigned int pin = -1;
  State state = State::Down;
};

String stringify(const Relay& relay);