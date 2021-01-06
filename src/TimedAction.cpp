#include "TimedAction.hpp"

TimedAction::TimedAction(uint32_t intervalMillis, Callback* callback)
  : intervalDuration(intervalMillis), actionCallback(callback)
{
  
}

TimedAction::TimedAction()
  : active(false)
{
  
}

void TimedAction::update()
{
  uint32_t currentTime = millis();

  if (currentTime - lastActionCompleteTime >= intervalDuration)
  {
    lastActionCompleteTime = currentTime;

    if (actionCallback)
      actionCallback->invoke();
    
    if (actionCompleteCallback)
      actionCompleteCallback->invoke();
  }
}

void TimedAction::setIntervalDuration(uint32_t intervalMillis)
{
  intervalDuration = intervalMillis;
}

uint32_t TimedAction::getIntervalDuration() const
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