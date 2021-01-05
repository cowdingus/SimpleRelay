#pragma once

#include "Callback.hpp"

class TimedAction
{
public:
  TimedAction(unsigned long intervalMillis, Callback* callback);
  TimedAction();

  void update(unsigned long deltaTime);

  void setIntervalDuration(unsigned long intervalMillis);
  unsigned long getIntervalDuration() const;

  void setActive(bool active);
  bool isActive() const;

  void attachActionCallback(Callback* callback);
  void detachActionCallback();

  void attachActionCompleteCallback(Callback* callback);
  void detachActionCompleteCallback();

private:
  unsigned long timeElapsed = 0;
  unsigned long intervalDuration = 100000;

  bool active = true;

  Callback* actionCallback = nullptr;
  Callback* actionCompleteCallback = nullptr;
};