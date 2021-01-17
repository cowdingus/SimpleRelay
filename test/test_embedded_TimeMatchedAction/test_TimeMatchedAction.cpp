#include "TimeMatchedAction.hpp"
#include "Callback.hpp"

#include <unity.h>

using namespace tda;

class CIncrement : public Callback
{
public:
    CIncrement(int& var)
        : var(var)
    {

    }

    void invoke() override
    {
        ++var;
    }

private:
    int& var;
};

void test_match_criteria_year()
{
  int x = 0;
  CIncrement incrementer(x);

  TimeMatchedAction tma;
  tma.setAction(&incrementer);
  tma.setMatchCriteria(MatchCriteria::Year);
  tma.setInvocationDate(DateTime(2000, 2, 15, 12, 30, 30));
  tma.setComplete(false);

  tma.update(DateTime(2000, 2, 15, 12, 30, 29), DateTime(2000, 2, 15, 12, 30, 31));

  TEST_ASSERT_TRUE(x == 1);
  TEST_ASSERT_TRUE(tma.isComplete());

  tma.setInvocationDate(DateTime(2000, 2, 15, 13, 45, 45));
  tma.setComplete(false);

  tma.update(DateTime(2000, 2, 15, 13, 46, 45), DateTime(2000, 2, 15, 13, 47, 45));

  TEST_ASSERT_TRUE(x == 1);
  TEST_ASSERT_FALSE(tma.isComplete());

  tma.update(DateTime(2000, 2, 15, 13, 43, 45), DateTime(2000, 2, 15, 13, 44, 45));

  TEST_ASSERT_TRUE(x == 1);
  TEST_ASSERT_FALSE(tma.isComplete());

  tma.setInvocationDate(DateTime(2000, 2, 15, 13, 45, 45));

  TEST_ASSERT_TRUE(DateTime(2000, 2, 15, 13, 45, 45) == tma.getInvocationDate());

  tma.update(DateTime(2000, 2, 15, 13, 44, 39), DateTime(2000, 2, 15, 13, 46, 10));

  TEST_ASSERT_TRUE(x == 2);
  TEST_ASSERT_TRUE(tma.isComplete());
}

void test_match_criteria_month()
{
  int x = 0;
  CIncrement incrementer(x);

  TimeMatchedAction tma;
  tma.setAction(&incrementer);
  tma.setMatchCriteria(MatchCriteria::Month);
  tma.setInvocationDate(DateTime(2000, 2, 15, 12, 30, 30));
  tma.setComplete(false);

  tma.update(DateTime(2000, 2, 15, 12, 29, 20), DateTime(2000, 2, 15, 12, 31, 10));

  TEST_ASSERT_TRUE(x == 1);
}

void setup() {
  // NOTE!!! Wait for >2 secs
  // if board doesn't support software reset via Serial.DTR/RTS
  delay(2000);

  UNITY_BEGIN();
  RUN_TEST(test_match_criteria_year);
  RUN_TEST(test_match_criteria_month);
  UNITY_END();
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
}