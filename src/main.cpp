#define SERIAL_ALLOWED !defined(DEBUG) && !defined(UNIT_TEST)

#if SERIAL_ALLOWED
  #define __ASSERT_USE_STDERR
#endif

#include "RepeatingAction.hpp"

#include <RTClib.h>
#include <Arduino.h>

#ifdef DEBUG
  #include <avr8-stub.h>
#endif

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

uint32_t previousTime = 0;
uint32_t timeNow = 0;
uint32_t deltaTime = timeNow - previousTime;

DateTime previousDate = DateTime();
DateTime dateNow = DateTime();
TimeSpan deltaDate = dateNow - previousDate;

const uint8_t relayPin1 = 2;
const uint8_t relayPin2 = 3;

RTC_DS3231 rtc;

void setup() 
{
  #if SERIAL_ALLOWED
  Serial.begin(9600);
  #endif
  
  #ifdef DEBUG
  debug_init();
  #endif

  pinMode(LED_BUILTIN, OUTPUT);

  if (!rtc.begin())
  {
    while (true)
    {
      delay(300);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(300);
      digitalWrite(LED_BUILTIN, LOW);
    }
  }

  if (rtc.lostPower())
  {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  pinMode(relayPin1, OUTPUT);
  pinMode(relayPin2, OUTPUT);
}

void updateDeltaTime()
{
  previousTime = timeNow;
  timeNow = millis();
  deltaTime = timeNow - previousTime;
}

void updateDeltaDate()
{
  previousDate = dateNow;
  dateNow;
}

void loop()
{
  updateDeltaTime();
}