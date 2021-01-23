#include "RepeatingAction_Millis.hpp"

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
  RepeatingAction_Millis::RepeatingAction_Millis(Callback* callback, uint32_t intervalms, bool overtimeCompensation)
  {
    setInterval(intervalms);
    setAction(callback);
    setOvertimeCompensation(overtimeCompensation);
  }

  RepeatingAction_Millis::RepeatingAction_Millis()
  {
    setActive(false);
  }

  void RepeatingAction_Millis::update(uint32_t deltaTime)
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

  void RepeatingAction_Millis::resetClock()
  {
    timeElapsed = 0;
  }

  void RepeatingAction_Millis::setInterval(uint32_t intervalms)
  {
    assert(intervalms > RAMS_MINIMUM_INTERVAL && F("Interval must be more than TA_MINIMUM_INTERVAL"));
    executionInterval = intervalms;
  }

  uint32_t RepeatingAction_Millis::getInterval() const
  {
    return executionInterval;
  }

  void RepeatingAction_Millis::setActive(bool active)
  {
    this->active = active;
  }

  bool RepeatingAction_Millis::isActive() const
  {
    return active;
  }

  void RepeatingAction_Millis::setAction(Callback* callback)
  {
    actionCallback = callback;
  }

  Callback* RepeatingAction_Millis::getAction() const
  {
    return actionCallback;
  }

  void RepeatingAction_Millis::setOvertimeCompensation(bool compensation)
  {
    overtimeCompensation = compensation;
  }

  bool RepeatingAction_Millis::getOvertimeCompensation() const
  {
    return overtimeCompensation;
  }
}