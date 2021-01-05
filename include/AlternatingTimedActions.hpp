#pragma once

#include "TimedAction.hpp"

template <unsigned int size>
class AlternatingTimedActions
{
public:
  AlternatingTimedActions();

  const TimedAction* getAction(unsigned int actionId) const;
  unsigned int addAction(TimedAction* timedAction);
  void removeAction(unsigned int actionId);

  void update(unsigned long deltaTime);

  unsigned int getTurn() const;
  void setTurn(unsigned int turn);

  unsigned int getMaxSize() const;
  unsigned int getSize() const;

private:
  struct ActionData
  {
    const unsigned int id = 0;
    TimedAction* timedAction;
  };

  ActionData* actionData[size] = {nullptr};

  TimedAction* getTimedActionFromId(unsigned int id);

  unsigned int turn = 0;
  unsigned int occupiedSize = 0;
};