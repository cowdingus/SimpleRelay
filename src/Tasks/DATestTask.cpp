#include "Tasks/DATestTask.hpp"

#include <Arduino.h>

DATestTask::DATestTask(uint8_t led1Pin, uint8_t led2Pin)
  :
  Task("DATest"),
  cToggleLed1(led1Pin),
  cToggleLed2(led2Pin)
{
  delayedActions.setAction(&cToggleLed1, 0, 0);
  delayedActions.setAction(&cToggleLed1, 200, 1);
  delayedActions.setAction(&cToggleLed2, 0, 2);
  delayedActions.setAction(&cToggleLed2, 200, 3);

  delayedActions.setTurn(0);
}

void DATestTask::run(uint32_t deltaTime)
{
  delayedActions.update(deltaTime); 
}

DATestTask::CToggleLed::CToggleLed(uint8_t ledPin)
  : ledPin(ledPin)
{
  pinMode(ledPin, OUTPUT);
  
  ledState = LOW;
  digitalWrite(ledPin, ledState);
}

void DATestTask::CToggleLed::invoke()
{
  ledState = !ledState;
  digitalWrite(ledPin, ledState);
}