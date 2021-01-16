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

    void update(const DateTime& dateTime);
    
    void setAction(Callback* callback);
    Callback* getAction() const;

    void setInvocationDate(const DateTime& dateTime, MatchCriteria matchCriteria);
    const DateTime& getInvocationDate() const;

    void setActive(bool active);
    bool isActive() const;

    void markAsComplete();

  private:
    Callback* action = nullptr;
    MatchCriteria matchCriteria = MatchCriteria::Year;
    DateTime invocationTime = DateTime(2200, 12, 20, 23, 59, 59);
    DateTime previousInvocationTime = DateTime(2200, 12, 20, 23, 59, 59);

    bool repeat = true;
    bool complete = false;

    bool active = true;

    bool isTimeToInvoke(const DateTime& currentTime);
    bool shouldInvoke(const DateTime& currentTime);
    void invokeAction(const DateTime& currentTime);
  };
}

#endif