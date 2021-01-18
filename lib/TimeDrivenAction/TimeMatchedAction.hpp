#ifndef NATIVE_UNIT_TEST

#pragma once

#include "RTClib.h"

namespace tda
{
  class Callback;
 
  enum class MatchCriteria
  {
    Second,
    Minute,
    Hour,
    DayOfWeek,
    DayOfMonth,
    Month,
    Year
  };
  
  class TimeMatchedAction
  {
  public:
    TimeMatchedAction();
    TimeMatchedAction(Callback* action, const DateTime& dateTime, MatchCriteria criteria);

    void update(const DateTime& previousTime, const DateTime& currentTime);
    
    void setAction(Callback* callback);
    Callback* getAction() const;

    void setInvocationDate(const DateTime& dateTime);
    const DateTime& getInvocationDate() const;

    void setMatchCriteria(MatchCriteria matchCriteria);
    MatchCriteria getMatchCriteria() const;

    void setRepeat(bool repeat);
    bool onRepeat() const;

    void setComplete(bool complete);
    bool isComplete() const;

  private:
    Callback* action = nullptr;
    MatchCriteria matchCriteria = MatchCriteria::Year;
    DateTime invocationTime = DateTime(0, 0, 0, 0, 0, 0);

    bool repeat = true;
    bool complete = false;

    bool isTimeToInvoke(const DateTime& previousTime, const DateTime& currentTime);
    bool shouldInvoke(const DateTime& previousTime, const DateTime& currentTime);

    template<typename T>
    bool inside(const T& value, const T& lowerBound, const T& upperBound);
  };

  template<typename T>
  bool TimeMatchedAction::inside(const T& value, const T& lowerBound, const T& upperBound)
  {
    return (lowerBound <= value) && (value <= upperBound);
  }
}

#endif
