#include "Relay.hpp"
#include "TimedAction.hpp"

#include "Tasks/NormalTask.hpp"
#include "Tasks/IdleTask.hpp"

#include <Wire.h>

#include <Arduino.h>

#ifdef DEBUG
  #include <avr8-stub.h>
#endif

Relay relay1(2);
Relay relay2(4);

NormalTask normalTask(relay1, relay2);
IdleTask idleTask;

String currentTask = "Idle";

void setup() 
{
  #ifdef DEBUG
  debug_init();
  #else
  Serial.begin(9600);
  #endif
}

void processInput()
{
  #ifndef DEBUG
  String input = Serial.readStringUntil('(');

  if (input.equals("setTask"))
  {
    String taskName = Serial.readStringUntil(')');

    if (taskName.equalsIgnoreCase("Normal"))
      currentTask = "Normal";
    else if (taskName.equalsIgnoreCase("Idle"))
      currentTask = "Idle";
    else
      Serial.println("No such registered task \'" + taskName + "\'");
  }
  #endif
}

void executeCurrentTask()
{
  if (currentTask.equals("Normal"))
    normalTask.run();
  else if (currentTask.equals("Idle"))
    idleTask.run();
  else
    idleTask.run();
}

void loop()
{
  processInput();
  executeCurrentTask();
}