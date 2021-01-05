#include "Relay.hpp"
#include "TimedAction.hpp"

#include <Arduino.h>

void processInput();
void doNormal();
void doIdle();
void updateDeltaTime();

Relay relay1(2);
Relay relay2(4);

enum class Task
{
  Idle,
  Normal
};

Task currentTask = Task::Normal;

unsigned long previousTime = 0;
unsigned long time = 0;
unsigned long deltaTime = time - previousTime;

void setup() 
{
  Serial.begin(9600);
}

void executeCurrentTask()
{
  switch (currentTask)
  {
    case Task::Idle:
      doIdle();
      break;
    case Task::Normal:
      doNormal();
      break;
    default:
      doIdle();
      break;
  }
}

void loop()
{
  updateDeltaTime();
  processInput();
  executeCurrentTask();
}

void processInput()
{
  String input = Serial.readStringUntil(';');
  Serial.flush();
  if (input.equalsIgnoreCase("Idle"))
    currentTask = Task::Idle;
  else if (input.equalsIgnoreCase("Normal"))
    currentTask = Task::Normal;
  else if (input.equalsIgnoreCase("getRelay(1).state"))
    Serial.println(stringify(relay1));
  else if (input.equalsIgnoreCase("getRelay(2).state"))
    Serial.println(stringify(relay2));
}

void doNormal()
{
  static TimedAction setUpDownR1(
    1000, 
    []() 
    {
      if (relay1.getState() == Relay::State::Down)
        relay1.setUp(); 
      else
      {
        relay1.setDown();
      }
    }
  );
  
  static TimedAction setUpDownR2(
    1000,
    []()
    {
      if (relay2.getState() == Relay::State::Down)
        relay2.setUp();
      else
      {
        relay2.setDown();
      }
    }
  );

  setUpDownR1.update(deltaTime);
  setUpDownR2.update(deltaTime);
}

void doIdle()
{
  delay(100);
}

void updateDeltaTime()
{
  previousTime = time;
  time = millis();
  deltaTime = time - previousTime;
}
