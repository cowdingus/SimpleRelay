#ifndef NATIVE_UNIT_TEST

#include "TimeMatchedAction.hpp"
#include "Callback.hpp"

namespace tda
{
  bool TimeMatchedAction::isTimeToInvoke(const DateTime& currentDateTime)
  {
    switch (matchCriteria)
    {
      case MatchCriteria::Year:
      {
        return currentDateTime.secondstime() >= invocationTime.secondstime();
      }
      case MatchCriteria::Month:
      {
        if (currentDateTime.month() > invocationTime.month())
          return true;
        if (currentDateTime.month() < invocationTime.month())
          return false;
      }
      case MatchCriteria::DayOfMonth:
      {
        if (currentDateTime.day() > invocationTime.day())
          return true;
        if (currentDateTime.day() < invocationTime.day())
          return false;
      }
      case MatchCriteria::DayOfWeek:
      {
        if (matchCriteria == MatchCriteria::DayOfWeek)
        {
          if (currentDateTime.dayOfTheWeek() > invocationTime.dayOfTheWeek())
            return true;
          if (currentDateTime.dayOfTheWeek() < invocationTime.dayOfTheWeek())
            return false;
        }
      }
      case MatchCriteria::Hour:
      {
        if (currentDateTime.hour() > invocationTime.hour())
          return true;
        if (currentDateTime.hour() < invocationTime.hour())
          return false;
      }
      case MatchCriteria::Minute:
      {
        if (currentDateTime.minute() > invocationTime.minute())
          return true;
        if (currentDateTime.minute() < invocationTime.minute())
          return false;
      }
      case MatchCriteria::Second:
      {
        return currentDateTime.second() >= invocationTime.second();
      }
    }
  }

  bool TimeMatchedAction::shouldInvoke(const DateTime& currentDateTime)
  {
    // If one of the date time operands is not valid just return false
    if ((!invocationTime.isValid()) || (!currentDateTime.isValid()))
      return false;

    // Return false if task is already completed and task is not on repeat
    if (complete && !repeat) return false;

    // Mark complete to false if repeat is true
    if (complete && repeat) complete = false;
    
    return !complete && isTimeToInvoke(currentDateTime);
  }

  void TimeMatchedAction::invokeAction(const DateTime& currentTime)
  {
    action->invoke();

    previousInvocationTime = currentTime;

    if (!repeat)
      markAsComplete();
  }
}

#endif