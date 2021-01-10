#include <Arduino.h>
#include <unity.h>

#include <limits.h>

class millis_mockup
{
public:
  static void set(unsigned long val)
  {
    timeNow = val;
  }

  static unsigned long now()
  {
    return timeNow += 10;
  }
private:
  millis_mockup();

  static unsigned long timeNow;
};

unsigned long millis_mockup::timeNow = 0;

void test_rollover_safety()
{
  unsigned long previousTime, timeNow, deltaTime;
  previousTime = 0;
  timeNow = 0;
  deltaTime = 0;

  millis_mockup::set(ULONG_MAX - 30);
  
  TEST_ASSERT_EQUAL(ULONG_MAX - 20, millis_mockup::now());

  previousTime = ULONG_MAX - 20;
  timeNow = millis_mockup::now();
  deltaTime = timeNow - previousTime;

  TEST_ASSERT_EQUAL(ULONG_MAX - 10, timeNow);
  TEST_ASSERT_EQUAL(10, deltaTime);

  previousTime = timeNow;
  timeNow = millis_mockup::now();
  deltaTime = timeNow - previousTime;

  TEST_ASSERT_EQUAL(ULONG_MAX, timeNow);
  TEST_ASSERT_EQUAL(10, deltaTime);
  
  previousTime = timeNow;
  timeNow = millis_mockup::now();
  deltaTime = timeNow - previousTime;

  TEST_ASSERT_EQUAL(9, timeNow);
  TEST_ASSERT_EQUAL(10, deltaTime);

  previousTime = timeNow;
  timeNow = millis_mockup::now();
  deltaTime = timeNow - previousTime;

  TEST_ASSERT_EQUAL(19, timeNow);
  TEST_ASSERT_EQUAL(10, deltaTime);

  TEST_ASSERT_EQUAL(29, millis_mockup::now());
}

void setup() {
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);

    UNITY_BEGIN();
    RUN_TEST(test_rollover_safety);
    UNITY_END();
}

void loop() {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
}