#if !defined(DEBUG) && !defined(UNIT_TEST)
  #define __ASSERT_USE_STDERR
#endif

#include "Relay.hpp"
#include "TimedAction.hpp"

#include "Tasks/NormalTask.hpp"
#include "Tasks/IdleTask.hpp"
#include "Tasks/DATestTask.hpp"

#include "Utility/setMillis.hpp"

#include <Arduino.h>

#if !defined(DEBUG) && !defined(UNIT_TEST)
#include <assert.h>

void __assert(const char *__func, const char *__file, int __lineno, const char *__sexp) {
    // transmit diagnostic informations through serial link.
    Serial.println(__func);
    Serial.println(__file);
    Serial.println(__lineno, DEC);
    Serial.println(__sexp);
    Serial.flush();
    // abort program execution.
    abort();
}
#endif

#ifdef DEBUG
  #include <avr8-stub.h>
#endif

Relay relay1(2);
Relay relay2(4);

NormalTask normalTask(relay1, relay2);
IdleTask idleTask;
DATestTask daTestTask(relay2, 6);

String currentTask = "DATest";

uint32_t previousTime = 0;
uint32_t timeNow = 0;
uint32_t deltaTime = timeNow - previousTime;

void setup() 
{
  #if !defined(DEBUG) && !defined(UNIT_TEST)
  Serial.begin(9600);
  #endif
  
  #ifdef DEBUG
  debug_init();
  #endif

  pinMode(6, OUTPUT);
}

void processInput()
{
  #if !defined(DEBUG) && !defined(UNIT_TEST)
  String input = Serial.readStringUntil('(');

  if (input.equals("setTask"))
  {
    String taskName = Serial.readStringUntil(')');

    if (taskName.equalsIgnoreCase(normalTask.getName()))
      currentTask = "Normal";
    else if (taskName.equalsIgnoreCase(idleTask.getName()))
      currentTask = "Idle";
    else if (taskName.equalsIgnoreCase(daTestTask.getName()))
      currentTask = "DATest";
    else
      Serial.println("No such registered task \'" + taskName + "\'");
  }
  #endif
}

void executeCurrentTask()
{
  if (currentTask.equals("Normal"))
    normalTask.run(deltaTime);
  else if (currentTask.equals("Idle"))
    idleTask.run();
  else if (currentTask.equals("DATest"))
    daTestTask.run(deltaTime);
  else
    idleTask.run();
}

void updateDeltaTime()
{
  previousTime = timeNow;
  timeNow = millis();
  deltaTime = timeNow - previousTime;
}

void loop()
{
  updateDeltaTime();
  processInput();
  executeCurrentTask();
}