#include "Relay.hpp"
#include "TimedAction.hpp"
#include "Tasks/NormalTask.hpp"

#include <Arduino.h>

void updateDeltaTime();

Relay relay1(2);
Relay relay2(4);

unsigned long previousTime = 0;
unsigned long time = 0;
unsigned long deltaTime = time - previousTime;

NormalTask normalTask(relay1, relay2);

void setup() 
{
  Serial.begin(9600);
}

void loop()
{
  updateDeltaTime();

  normalTask.run(deltaTime);
}

void updateDeltaTime()
{
  previousTime = time;
  time = millis();
  deltaTime = time - previousTime;
}
