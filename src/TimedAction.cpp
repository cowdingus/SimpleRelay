#include "TimedAction.hpp"

TimedAction::TimedAction(unsigned long intervalMillis, Callback* callback)
  : intervalDuration(intervalMillis), actionCallback(callback)
{
  
}

TimedAction::TimedAction()
  : active(false)
{
  
}

void TimedAction::update(unsigned long deltaTime)
{
  timeElapsed += deltaTime;
  while (timeElapsed >= intervalDuration)
  {
    timeElapsed -= intervalDuration;

    if (active)
    {
      if (actionCallback)
        actionCallback->invoke();
    
      if (actionCompleteCallback)
        actionCompleteCallback->invoke();
    }
  }
}

void TimedAction::setIntervalDuration(unsigned long intervalMillis)
{
  intervalDuration = intervalMillis;
}

unsigned long TimedAction::getIntervalDuration() const
{
  return intervalDuration;
}

void TimedAction::setActive(bool active)
{
  this->active = active;
}

bool TimedAction::isActive() const
{
  return active;
}

void TimedAction::attachActionCallback(Callback* callback)
{
  actionCallback = callback;
}

void TimedAction::detachActionCallback()
{
  actionCallback = nullptr;
}

void TimedAction::attachActionCompleteCallback(Callback* callback)
{
  actionCompleteCallback = callback;
}

void TimedAction::detachActionCompleteCallback()
{
  actionCompleteCallback = nullptr;
}