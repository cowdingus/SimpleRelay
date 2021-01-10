#include "Tasks/DATestTask.hpp"

#include "Relay.hpp"

#include <Arduino.h>

DATestTask::DATestTask(Relay& relay, uint16_t ledPin)
  :
  Task("DATest"),
  cToggleRelay(relay),
  cToggleLed(ledPin)
{
  delayedActions.setAction(&cToggleLed, 0, 0);
  delayedActions.setAction(&cToggleLed, 200, 1);
  delayedActions.setAction(&cToggleRelay, 0, 2);
  delayedActions.setAction(&cToggleRelay, 200, 3);

  delayedActions.setTurn(0);
}

void DATestTask::run(uint32_t deltaTime)
{
  delayedActions.update(deltaTime); 
}

DATestTask::CToggleRelay::CToggleRelay(Relay& relay)
  : relay(relay)
{

}

void DATestTask::CToggleRelay::invoke()
{
  if (relay.getState() == Relay::State::Down)
    relay.setUp();
  else
    relay.setDown();
}

DATestTask::CToggleLed::CToggleLed(uint16_t ledPin, uint8_t lastState)
  : ledPin(ledPin), ledState(lastState)
{

}

void DATestTask::CToggleLed::invoke()
{
  ledState = !ledState;
  digitalWrite(ledPin, ledState);
}