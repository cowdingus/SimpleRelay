#pragma once

#include "Task.hpp"
#include "TimedAction.hpp"
#include "Callback.hpp"

class Relay;

class NormalTask : public Task
{
public:
  NormalTask(Relay& relay1, Relay& relay2);

  void run(uint32_t deltaTime);

private:
  class CToggleRelay : public Callback
  {
  public:
    CToggleRelay(Relay& relay);

    void invoke() override;

  private:
    Relay& relay;
  };

  TimedAction relay1TimedAction;
  TimedAction relay2TimedAction;

  Relay& relay1;
  Relay& relay2;

  CToggleRelay toggleRelay1;
  CToggleRelay toggleRelay2;
};