#include "Tasks/NormalTask.hpp"
#include "Relay.hpp"

#include <Arduino.h>

NormalTask::NormalTask(Relay& relay1, Relay& relay2)
  :
  Task("Normal"),
  relay1(relay1),
  relay2(relay2),
  actionCallback1(relay1),
  actionCallback2(relay2)
{
  relay1Action.setIntervalDuration(1000);
  relay1Action.attachActionCallback(&actionCallback1);

  relay2Action.setIntervalDuration(1000);
  relay2Action.attachActionCallback(&actionCallback2);

  relay1Action.setActive(true);
  relay2Action.setActive(true);
}

void NormalTask::run(unsigned long deltaTime)
{
  relay1Action.update(deltaTime);
  relay2Action.update(deltaTime);
}

NormalTask::CToggleRelay::CToggleRelay(Relay& relay)
  : relay(relay)
{

}

void NormalTask::CToggleRelay::invoke()
{
  if (relay.getState() == Relay::State::Down)
    relay.setUp();
  else
    relay.setDown();
}