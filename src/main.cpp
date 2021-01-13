#define SERIAL_ALLOWED !defined(DEBUG) && !defined(UNIT_TEST)

#if SERIAL_ALLOWED
  #define __ASSERT_USE_STDERR
#endif

#include "RepeatingAction.hpp"

#include "Tasks/IdleTask.hpp"
#include "Tasks/DATestTask.hpp"

#include <Wire.h>
#include <RTClib.h>

#include <Arduino.h>

#if SERIAL_ALLOWED
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

IdleTask idleTask;
DATestTask daTestTask(4, 6);

String currentTask = "DATest";

uint32_t previousTime = 0;
uint32_t timeNow = 0;
uint32_t deltaTime = timeNow - previousTime;

RTC_DS3231 rtc;

void setup() 
{
  #if SERIAL_ALLOWED
  Serial.begin(9600);
  #endif
  
  #ifdef DEBUG
  debug_init();
  #endif

  Wire.begin();
  
  if (!rtc.begin())
  {
  #if SERIAL_ALLOWED
    Serial.println("Failed to begin connection with RTC_DS3231");
  #endif
  }

  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  pinMode(6, OUTPUT);
}

void processInput()
{
  #if SERIAL_ALLOWED
  String input = Serial.readStringUntil('(');

  if (input.equals("setTask"))
  {
    String taskName = Serial.readStringUntil(')');

    if (taskName.equalsIgnoreCase(idleTask.getName()))
      currentTask = "Idle";
    else if (taskName.equalsIgnoreCase(daTestTask.getName()))
      currentTask = "DATest";
    else
      Serial.println("No such registered task \'" + taskName + "\'");
  }
  if (input.equals("getTime"))
  {
    String param = Serial.readStringUntil(')');

    DateTime now = rtc.now();
    Serial.println(now.toString("YYYY/MM/DD"));
  }
  #endif
}

void executeCurrentTask()
{
  if (currentTask.equals("Idle"))
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