#define SERIAL_ALLOWED !defined(DEBUG) && !defined(UNIT_TEST)

#if SERIAL_ALLOWED
  #define __ASSERT_USE_STDERR
#endif

#include "AlarmAction.hpp"
#include "DelayedActions.hpp"

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

RTC_DS3231 rtc;

class InvokeFunction : public tda::Callback
{
public:
  InvokeFunction(void (*function)())
    : function(function)
  {
    
  }

  void invoke() override
  {
    if (function)
      function();
  }

private:
  void (*function)() = nullptr;
};

const uint8_t wateringRelay = 4;

tda::AlarmAction alarmAction;
tda::DelayedActions<3> wateringSequence;

TimeSpan wateringDuration = TimeSpan(10);

void startWateringSequence()
{
  wateringSequence.setPause(false);
  Serial.write("Watering sequence started \n");
}
InvokeFunction cStartWateringLoop(startWateringSequence);

void endWateringSequence()
{
  wateringSequence.setPause(true);
  Serial.write("Watering sequence stopped\n");
}
InvokeFunction cBreakWateringLoop(endWateringSequence);

void openWateringRelay()
{
  digitalWrite(wateringRelay, HIGH);
  Serial.write("Watering relay opened\n");
}
InvokeFunction cOpenWateringRelay(openWateringRelay);

void closeWateringRelay()
{
  digitalWrite(wateringRelay, LOW);
  Serial.write("Watering relay closed\n");
}
InvokeFunction cCloseWateringRelay(closeWateringRelay);

void updateDeltaTime()
{
  previousTime = timeNow;
  timeNow = millis();
  deltaTime = timeNow - previousTime;
}

void updateDeltaDate()
{
  previousDate = dateNow;
  dateNow = rtc.now();
  deltaDate = dateNow - previousDate;
}

void setup() 
{
  #if SERIAL_ALLOWED
  Serial.begin(9600);
  Serial.write("Serial: Test");
  #endif
  
  #ifdef DEBUG
  debug_init();
  #endif

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  while (!rtc.begin())
  {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(200);
  }

  if (rtc.lostPower())
  {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  Serial.write(rtc.now().timestamp().c_str());

  updateDeltaTime();
  updateDeltaDate();

  pinMode(wateringRelay, OUTPUT);

  alarmAction.setAction(&cStartWateringLoop);
  tda::setAtHour(11, dateNow, alarmAction);
  alarmAction.setActive(true);

  wateringSequence.setAction(&cOpenWateringRelay, 50, 0);
  wateringSequence.setAction(&cCloseWateringRelay, 10000, 1);
  wateringSequence.setAction(&cBreakWateringLoop, 50, 2);
}

void loop()
{
  updateDeltaTime();
  updateDeltaDate();

  alarmAction.update(dateNow);
  wateringSequence.update(deltaTime);
}