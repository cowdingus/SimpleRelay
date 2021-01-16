#pragma once

#include "Callback.hpp"

#include <RTClib.h>

namespace tda
{
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

  struct Task
  { 
    Callback* action = nullptr;
    MatchCriteria matchCriteria = MatchCriteria::Year;
    DateTime invocationTime = DateTime(2200, 12, 20, 23, 59, 59);

    bool taskDone = false;
  };
}