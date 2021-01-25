#ifndef NATIVE_UNIT_TEST

#include "RepeatingAction_Second.hpp"

namespace tda
{
  RepeatingAction_Second::RepeatingAction_Second(Callback* callback, TimeSpan interval, bool overtimeCompensation)
  {
    setInterval(interval);
    setAction(callback);
    setOvertimeCompensation(overtimeCompensation);
  }

  RepeatingAction_Second::RepeatingAction_Second()
  {
    setActive(false);
  }

  void RepeatingAction_Second::update(TimeSpan deltaTime)
  {
    timeElapsed = timeElapsed + deltaTime;

    while (timeElapsed.totalseconds() >= executionInterval.totalseconds())
    {
      // If callback invocation is ignored there's no need to do overtime compensation
      if (overtimeCompensation && active)
      {
        timeElapsed = timeElapsed - executionInterval;
      }
      else
      {
        timeElapsed = timeElapsed.totalseconds() % executionInterval.totalseconds();
      }
      
      if (active && actionCallback)
        actionCallback->invoke();
    }
  }

  void RepeatingAction_Second::resetClock()
  {
    timeElapsed = 0;
  }

  void RepeatingAction_Second::setInterval(TimeSpan interval)
  {
    assert(interval.totalseconds() > RAS_MINIMUM_INTERVAL && F("Interval must be more than TA_MINIMUM_INTERVAL"));
    executionInterval = interval;
  }

  TimeSpan RepeatingAction_Second::getInterval() const
  {
    return executionInterval;
  }

  void RepeatingAction_Second::setActive(bool active)
  {
    this->active = active;
  }

  bool RepeatingAction_Second::isActive() const
  {
    return active;
  }

  void RepeatingAction_Second::setAction(Callback* callback)
  {
    actionCallback = callback;
  }

  Callback* RepeatingAction_Second::getAction() const
  {
    return actionCallback;
  }

  void RepeatingAction_Second::setOvertimeCompensation(bool compensation)
  {
    overtimeCompensation = compensation;
  }

  bool RepeatingAction_Second::getOvertimeCompensation() const
  {
    return overtimeCompensation;
  }
}

#endif