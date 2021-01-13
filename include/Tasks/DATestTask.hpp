#pragma once

#include "Task.hpp"
#include "DelayedActions.hpp"

class Relay;

class DATestTask : public Task
{
public:
  DATestTask(uint8_t led1Pin, uint8_t led2Pin);

  void run(uint32_t deltaTime);

private:
  DelayedActions<4> delayedActions;
  
  class CToggleLed : public Callback
  {
  public:
    CToggleLed(uint8_t ledPin);

    void invoke() override;

  private:
    uint8_t ledPin = 0;
    uint8_t ledState = 0;
  };

  CToggleLed cToggleLed1;
  CToggleLed cToggleLed2;
};