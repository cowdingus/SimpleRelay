#include "Tasks/NormalTask.hpp"
#include "Relay.hpp"

#include <Arduino.h>

NormalTask::NormalTask(Relay& relay1, Relay& relay2)
  :
  Task("Normal"),
  relay1(relay1),
  relay2(relay2),
  toggleRelay1(relay1),
  toggleRelay2(relay2)
{
  relay1TimedAction.setInterval(1000);
  relay1TimedAction.setAction(&toggleRelay1);

  relay2TimedAction.setInterval(1000);
  relay2TimedAction.setAction(&toggleRelay2);

  relay1TimedAction.setActive(true);
  relay2TimedAction.setActive(true);
}

void NormalTask::run(uint32_t deltaTime)
{
  relay1TimedAction.update(deltaTime);
  relay2TimedAction.update(deltaTime);
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