#include "RepeatingAction.hpp"

#if !defined(DEBUG) && !defined(UNIT_TEST)
  #define __ASSERT_USE_STDERR
#endif

#ifdef NATIVE_UNIT_TEST
  #define F(x) x
#else
  #include <WString.h>
#endif

#include <assert.h>

namespace tda
{
  RepeatingAction::RepeatingAction(Callback* callback, uint32_t intervalms, bool overtimeCompensation)
  {
    setInterval(intervalms);
    setAction(callback);
    setOvertimeCompensation(overtimeCompensation);
  }

  RepeatingAction::RepeatingAction()
  {
    setActive(false);
  }

  void RepeatingAction::update(uint32_t deltaTime)
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

  void RepeatingAction::resetClock()
  {
    timeElapsed = 0;
  }

  void RepeatingAction::setInterval(uint32_t intervalms)
  {
    assert(intervalms > RA_MINIMUM_INTERVAL && F("Interval must be more than TA_MINIMUM_INTERVAL"));
    executionInterval = intervalms;
  }

  uint32_t RepeatingAction::getInterval() const
  {
    return executionInterval;
  }

  void RepeatingAction::setActive(bool active)
  {
    this->active = active;
  }

  bool RepeatingAction::isActive() const
  {
    return active;
  }

  void RepeatingAction::setAction(Callback* callback)
  {
    actionCallback = callback;
  }

  Callback* RepeatingAction::getAction() const
  {
    return actionCallback;
  }

  void RepeatingAction::setOvertimeCompensation(bool compensation)
  {
    overtimeCompensation = compensation;
  }

  bool RepeatingAction::getOvertimeCompensation() const
  {
    return overtimeCompensation;
  }
}