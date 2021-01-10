#include "TimedAction.hpp"

#if !defined(DEBUG) && !defined(UNIT_TEST)
  #define __ASSERT_USE_STDERR
#endif

#include <assert.h>

TimedAction::TimedAction(Callback* callback, uint32_t intervalms, bool overtimeCompensation)
{
  setInterval(intervalms);
  setAction(callback);
  setOvertimeCompensation(overtimeCompensation);
}

TimedAction::TimedAction()
{
  setActive(false);
}

void TimedAction::update(uint32_t deltaTime)
{
  timeElapsed += deltaTime;

  while (timeElapsed >= executionInterval)
  {
    // If callback invocation is ignored there's no need to do overtime compensation
    if (overtimeCompensation && active)
    {
      timeElapsed -= executionInterval;
    }
    else
    {
      timeElapsed = timeElapsed % executionInterval;
    }
    
    if (active && actionCallback)
      actionCallback->invoke();
  }
}

void TimedAction::resetClock()
{
  timeElapsed = 0;
}

void TimedAction::setInterval(uint32_t intervalms)
{
  assert(intervalms > TA_MINIMUM_INTERVAL && "Interval must be more than TA_MINIMUM_INTERVAL");
  executionInterval = intervalms;
}

uint32_t TimedAction::getInterval() const
{
  return executionInterval;
}

void TimedAction::setActive(bool active)
{
  this->active = active;
}

bool TimedAction::isActive() const
{
  return active;
}

void TimedAction::setAction(Callback* callback)
{
  actionCallback = callback;
}

Callback* TimedAction::getAction() const
{
  return actionCallback;
}

void TimedAction::setOvertimeCompensation(bool compensation)
{
  overtimeCompensation = compensation;
}

bool TimedAction::getOvertimeCompensation() const
{
  return overtimeCompensation;
}