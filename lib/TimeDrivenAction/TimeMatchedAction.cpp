#ifndef NATIVE_UNIT_TEST

#include "TimeMatchedAction.hpp"
#include "Callback.hpp"

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
  TimeMatchedAction::TimeMatchedAction()
    : complete(true)
  {

  }

  TimeMatchedAction::TimeMatchedAction(Callback* callback, const DateTime& invocationDateTime, MatchCriteria criteria)
  {
    setAction(callback);
    setInvocationDate(invocationDateTime);
    setMatchCriteria(criteria);
  }

  void TimeMatchedAction::update(const DateTime& previousTime, const DateTime& currentTime)
  {
    if (shouldInvoke(previousTime, currentTime))
    {
      if (action)
        action->invoke();

      if (!repeat || matchCriteria == MatchCriteria::Year)
        setComplete(true);
    }
  }

  void TimeMatchedAction::setAction(Callback* callback)
  {
    action = callback;
  }

  Callback* TimeMatchedAction::getAction() const
  {
    return action;
  }

  void TimeMatchedAction::setInvocationDate(const DateTime& dateTime)
  {
    invocationTime = dateTime;
  }

  const DateTime& TimeMatchedAction::getInvocationDate() const
  {
    return invocationTime;
  }

  void TimeMatchedAction::setMatchCriteria(MatchCriteria criteria)
  {
    matchCriteria = criteria;
  }

  MatchCriteria TimeMatchedAction::getMatchCriteria() const
  {
    return matchCriteria;
  }

  void TimeMatchedAction::setRepeat(bool repeat)
  {
    this->repeat = repeat;
  }

  bool TimeMatchedAction::onRepeat() const
  {
    return repeat;
  }

  void TimeMatchedAction::setComplete(bool complete)
  {
    this->complete = complete;
  }

  bool TimeMatchedAction::isComplete() const
  {
    return complete;
  }

  bool TimeMatchedAction::isTimeToInvoke(const DateTime& previousTime, const DateTime& currentTime)
  {
    switch(matchCriteria)
    {
      case MatchCriteria::Year:
        return inside(invocationTime, previousTime, currentTime);
      case MatchCriteria::Month:
        // To prevent misses if the parameter is only one (only the currentTime)
        // and currentTime is offset to the future a little bit, so can't use
        // plain equal operator, it will miss if deltaTime is too big.
        // Geometric interpretation of timeline made me think of this solution.
        if (inside(invocationTime.month(), previousTime.month(), currentTime.month()))
        {
          // It's obvious if now is one month or plus after the supposed invocation time it should be invoked.
          if (currentTime.month() > invocationTime.month())
            return true;
          // If the month is equal, then fall through to compare smaller units (like day, hour, etc)
          // Note that currentTime can't be less / lower than invocationTime because that wouldn't pass
          // the `inside` if statement, that's why I ommited that check.
        }
      case MatchCriteria::DayOfMonth:
        if (inside(invocationTime.day(), previousTime.day(), currentTime.day()))
        {
          if (currentTime.day() > invocationTime.day())
            return true;
        }
      case MatchCriteria::DayOfWeek:
        // Skip in case of fall through, because day of week can be different on each year / month
        if (matchCriteria == MatchCriteria::DayOfWeek)
        {
          if (inside(invocationTime.dayOfTheWeek(), previousTime.dayOfTheWeek(), currentTime.dayOfTheWeek()))
          {
            if (currentTime.dayOfTheWeek() > invocationTime.dayOfTheWeek())
              return true;
          }
        }
      case MatchCriteria::Hour:
        if (inside(invocationTime.hour(), previousTime.hour(), currentTime.hour()))
        {
          if (currentTime.hour() > invocationTime.hour())
            return true;
        }
      case MatchCriteria::Minute:
        if (inside(invocationTime.minute(), previousTime.minute(), currentTime.minute()))
        {
          if (currentTime.minute() > invocationTime.minute())
            return true;
        }
      case MatchCriteria::Second:
        // There's no need to do any fallthrough checks here, it's already the end.
        return inside(invocationTime.second(), previousTime.second(), currentTime.second());
    }

    assert(false && F("Reached impossible state at TimeMatchedAction::isTimeToInvoke()"));
    return false;
  }

  bool TimeMatchedAction::shouldInvoke(const DateTime& previousTime, const DateTime& currentTime)
  {
    // If one of the date time operands is not valid just return false
    if ((!invocationTime.isValid()) || (!previousTime.isValid()) || (!currentTime.isValid()))
      return false;

    if (complete) return false;
    
    return isTimeToInvoke(previousTime, currentTime);
  }
}

#endif