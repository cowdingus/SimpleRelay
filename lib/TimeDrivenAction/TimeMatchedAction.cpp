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
        if (!inside(invocationTime.month(), previousTime.month(), currentTime.month()))
          return false;
      case MatchCriteria::DayOfMonth:
        if (!inside(invocationTime.day(), previousTime.day(), currentTime.day()))
          return false;
      case MatchCriteria::DayOfWeek:
        // Skip in case of fall through
        if (matchCriteria == MatchCriteria::DayOfWeek)
        {
          if (!inside(invocationTime.dayOfTheWeek(), previousTime.dayOfTheWeek(), currentTime.dayOfTheWeek()))
            return false;
        }
      case MatchCriteria::Hour:
        if (!inside(invocationTime.hour(), previousTime.hour(), currentTime.hour()))
          return false;
      case MatchCriteria::Minute:
        if (!inside(invocationTime.minute(), previousTime.minute(), currentTime.minute()))
          return false;
      case MatchCriteria::Second:
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