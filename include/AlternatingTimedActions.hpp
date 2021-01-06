#pragma once

#include "TimedAction.hpp"

template <unsigned int size>
class AlternatingTimedActions
{
public:
  AlternatingTimedActions();

  void update(unsigned long deltaTime);

  unsigned int getTurn() const;
  void setTurn(unsigned int turn);

  unsigned int getSize() const;

  TimedAction*& at(unsigned int index);
  TimedAction*& operator[](unsigned int index);

private:
  ActionDatum* actionData[size] = {nullptr};

  unsigned int turn = 0;
};