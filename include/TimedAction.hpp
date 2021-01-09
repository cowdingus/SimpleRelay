#pragma once

#include "Callback.hpp"

#include <Arduino.h>
#include <stdint.h>

#define TA_MINIMUM_INTERVAL 25

class TimedAction
{
public:
  TimedAction(Callback* callback, uint32_t intervalms, bool overtimeCompensation = true);
  TimedAction();

  void update(uint32_t elapsedTime);
  void resetClock();

  void setInterval(uint32_t intervalms);
  uint32_t getInterval() const;

  void setActive(bool active);
  bool isActive() const;

  void setAction(Callback* callback);

  void setOvertimeCompensation(bool compensation);

private:
  uint32_t executionInterval = 1000;
  uint32_t timeElapsed = 0;

  bool active = true;
  bool overtimeCompensation = true;

  Callback* actionCallback = nullptr;
};