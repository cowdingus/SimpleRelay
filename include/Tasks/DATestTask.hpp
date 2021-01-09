#pragma once

#include "Task.hpp"
#include "DelayedActions.hpp"

class Relay;

class DATestTask : public Task
{
public:
  DATestTask(Relay& relay, uint16_t ledPin);

  void run(uint32_t deltaTime);

private:
  DelayedActions<4> delayedActions;
  
  class CToggleLed : public Callback
  {
  public:
    CToggleLed(uint16_t ledPin, uint8_t lastState = 0x0);

    void invoke() override;

  private:
    uint16_t ledPin = 0;
    uint8_t ledState = 0x0;
  };

  class CToggleRelay : public Callback
  {
  public:
    CToggleRelay(Relay& relay);

    void invoke() override;
  
  private:
    Relay& relay;
  };

  CToggleRelay cToggleRelay;
  CToggleLed cToggleLed;
};