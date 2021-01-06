#pragma once

#include "Callback.hpp"

#include <Arduino.h>

#include <stdint.h>

class TimedAction
{
public:
  TimedAction(uint32_t intervalms, Callback* callback);
  TimedAction();

  void update();

  void setIntervalDuration(uint32_t intervalms);
  uint32_t getIntervalDuration() const;

  void setActive(bool active);
  bool isActive() const;

  void attachActionCallback(Callback* callback);
  void detachActionCallback();

  void attachActionCompleteCallback(Callback* callback);
  void detachActionCompleteCallback();

private:
  uint32_t intervalDuration = 100000;
  uint32_t lastActionCompleteTime = millis();

  bool active = true;

  Callback* actionCallback = nullptr;
  Callback* actionCompleteCallback = nullptr;
};