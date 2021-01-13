#pragma once

#include "Callback.hpp"

#include <stdint.h>

#define RA_MINIMUM_INTERVAL 25

class RepeatingAction
{
public:
  RepeatingAction(Callback* callback, uint32_t intervalms, bool overtimeCompensation = true);
  RepeatingAction();

  void update(uint32_t elapsedTime);
  void resetClock();

  void setInterval(uint32_t intervalms);
  uint32_t getInterval() const;

  void setActive(bool active);
  bool isActive() const;

  void setAction(Callback* callback);
  Callback* getAction() const;

  void setOvertimeCompensation(bool compensation);
  bool getOvertimeCompensation() const;

private:
  uint32_t executionInterval = 1000;
  uint32_t timeElapsed = 0;

  bool active = true;
  bool overtimeCompensation = true;

  Callback* actionCallback = nullptr;
};