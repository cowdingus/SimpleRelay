#pragma once

#include "Task.hpp"
#include "TimedAction.hpp"
#include "Callback.hpp"

class Relay;

class NormalTask : public Task
{
public:
  NormalTask(Relay& relay1, Relay& relay2);

  void run();

private:
  class CToggleRelay : public Callback
  {
  public:
    CToggleRelay(Relay& relay);

    void invoke() override;

  private:
    Relay& relay;
  };

  TimedAction relay1Action;
  TimedAction relay2Action;

  Relay& relay1;
  Relay& relay2;

  CToggleRelay actionCallback1;
  CToggleRelay actionCallback2;
};