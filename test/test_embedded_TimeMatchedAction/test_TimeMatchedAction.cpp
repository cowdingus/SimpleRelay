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
  tma.setInvocationDate(DateTime(2011, 2, 15, 12, 30, 30));
  tma.setComplete(false);

  // Month matches invocation date
  tma.update(DateTime(0, 2, 15, 12, 29, 20), DateTime(0, 2, 15, 12, 31, 10));

  TEST_ASSERT_TRUE(x == 1);

  // Month doesn't match invocation date (not yet)
  tma.update(DateTime(0, 1, 15, 12, 29, 20), DateTime(0, 1, 16, 12, 31, 10));

  TEST_ASSERT_TRUE(x == 1);

  // Month doesn't match invocation date (in the future)
  tma.update(DateTime(0, 5, 15, 12, 29, 20), DateTime(0, 6, 15, 12, 31, 10));

  TEST_ASSERT_TRUE(x == 1);

  // Month matches invocation date but subunits doesn't match the invocation date (Not yet)
  tma.update(DateTime(0, 2, 13, 12, 30, 30), DateTime(0, 2, 14, 12, 30, 30));

  TEST_ASSERT_TRUE(x == 1);

  // Month matches invocation date but subunits doesn't match the invocation date (In the future)
  tma.update(DateTime(0, 2, 16, 12, 30, 30), DateTime(0, 2, 17, 12, 30, 30));

  TEST_ASSERT_TRUE(x == 1);

  // Month matches invocation date (again)
  tma.update(DateTime(0, 2, 15, 12, 29, 20), DateTime(0, 2, 15, 12, 31, 10));

  TEST_ASSERT_TRUE(x == 2);
}

void test_criteria_day_of_month()
{
  int x = 0;
  CIncrement incrementer(x);

  TimeMatchedAction tma;
  tma.setAction(&incrementer);
  tma.setMatchCriteria(MatchCriteria::DayOfMonth);
  tma.setInvocationDate(DateTime(2011, 2, 15, 12, 30, 30));
  tma.setComplete(false);

  // Day of month matches invocation time
  tma.update(DateTime(1, 1, 15, 12, 30, 30), DateTime(2, 2, 15, 13, 30, 10));

  TEST_ASSERT_TRUE(x == 1);
  
  // Day of month matches invocation time
  tma.update(DateTime(3, 5, 15, 10, 30, 20), DateTime(6, 5, 15, 13, 30, 10));

  TEST_ASSERT_TRUE(x == 2);

  // Day of month doesn't match the invocation time (not yet)
  tma.update(DateTime(3, 4, 12, 14, 30, 30), DateTime(4, 5, 13, 16, 30, 30));

  TEST_ASSERT_TRUE(x == 2);

  // Day of month doesn't match the invocation time (in the future)
  tma.update(DateTime(1, 6, 16, 12, 30, 30), DateTime(5, 7, 17, 12, 30, 30));

  TEST_ASSERT_TRUE(x == 2);

  // Day of month matches the invocation time but its subunits doesn't match (not yet)
  tma.update(DateTime(2, 6, 15, 10, 30, 30), DateTime(6, 7, 15, 11, 30, 59));

  TEST_ASSERT_TRUE(x == 2);

  // Day of month matches the invocation time but its subunits doesn't match (in the future)
  tma.update(DateTime(2, 6, 15, 13, 30, 20), DateTime(7, 7, 15, 14, 30, 20));

  TEST_ASSERT_TRUE(x == 2);
}

void test_criteria_day_of_week()
{
  int x = 0;
  CIncrement incrementer(x);

  TimeMatchedAction tma;
  tma.setAction(&incrementer);
  tma.setMatchCriteria(MatchCriteria::DayOfWeek);
  tma.setInvocationDate(DateTime(2011, 2, 15, 12, 30, 30));
  tma.setComplete(false);

  // Day of week matches invocation time (in the past)
  tma.update(DateTime(2010, 1, 19, 12, 30, 30), DateTime(2010, 1, 19, 13, 20, 10));

  TEST_ASSERT_TRUE(x == 1);

  // Day of week matches invocation time (in the future)
  tma.update(DateTime(2012, 1, 24, 12, 30, 30), DateTime(2012, 1, 24, 12, 30, 30));

  TEST_ASSERT_TRUE(x == 2);

  // Day of week matches invocation time (in the future)
  tma.update(DateTime(2012, 1, 25, 12, 30, 30), DateTime(2012, 1, 26, 13, 20, 10));

  TEST_ASSERT_TRUE(x == 3);

  // Day of week doesn't match invocation time (in the past)
  tma.update(DateTime(2012, 1, 20, 12, 30, 30), DateTime(2012, 1, 21, 13, 20, 10));

  TEST_ASSERT_TRUE(x == 3);

  // Day of week doesn't match invocation time (in the future) (exact different)
  tma.update(DateTime(2012, 1, 23, 12, 30, 30), DateTime(2012, 1, 24, 12, 30, 30));

  TEST_ASSERT_TRUE(x == 3);
}

void test_invocation_criteria_hour()
{
  int x = 0;
  CIncrement incrementer(x);

  TimeMatchedAction tma;
  tma.setAction(&incrementer);
  tma.setMatchCriteria(MatchCriteria::Hour);
  tma.setInvocationDate(DateTime(2011, 2, 15, 12, 30, 30));
  tma.setComplete(false);

  // Hour matches the invocation date (exact match)
  tma.update(DateTime(1, 1, 1, 12, 30, 30), DateTime(1, 1, 1, 12, 30, 30));

  TEST_ASSERT_TRUE(x == 1);

  // Hour matches the invocation date
  tma.update(DateTime(1, 1, 1, 12, 16, 20), DateTime(1, 1, 2, 10, 45, 10));

  TEST_ASSERT_TRUE(x == 2);

  // Hour matches the invocation date but subunits doesn't match (in the future)
  tma.update(DateTime(1, 1, 1, 12, 30, 31), DateTime(1, 1, 1, 12, 30, 32));

  TEST_ASSERT_TRUE(x == 2);

  // Hour matches the invocation date but subunits doesn't match (in the past)
  tma.update(DateTime(1, 1, 1, 12, 29, 30), DateTime(1, 1, 1, 12, 29, 59));

  TEST_ASSERT_TRUE(x == 2);

  // Hour doesn't match the invocation date (in the future)
  tma.update(DateTime(1, 1, 1, 13, 30, 31), DateTime(1, 1, 1, 13, 30, 32));

  TEST_ASSERT_TRUE(x == 2);
}

void test_invocation_criteria_second()
{
  int x = 0;
  CIncrement incrementer(x);

  TimeMatchedAction tma;
  tma.setAction(&incrementer);
  tma.setMatchCriteria(MatchCriteria::Second);
  tma.setInvocationDate(DateTime(2011, 2, 15, 12, 30, 30));
  tma.setComplete(false);

  // Exact match
  tma.update(DateTime(1, 1, 1, 1, 1, 30), DateTime(1, 1, 1, 1, 1, 30));
  TEST_ASSERT_TRUE(x == 1);

  // Matches
  tma.update(DateTime(1, 1, 1, 1, 1, 29), DateTime(1, 1, 1, 1, 1, 31));
  TEST_ASSERT_TRUE(x == 2);

  // Doesn't match (future) (exact)
  tma.update(DateTime(1, 1, 1, 1, 1, 31), DateTime(1, 1, 1, 1, 1, 31));

  // Doesn't match (in the past) (exact)
  tma.update(DateTime(1, 1, 1, 1, 1, 29), DateTime(1, 1, 1, 1, 1, 29));

  // Doesn't match (future)
  tma.update(DateTime(1, 1, 1, 1, 1, 31), DateTime(1, 1, 1, 1, 1, 32));

  // Doesn't match (in the past)
  tma.update(DateTime(1, 1, 1, 1, 1, 28), DateTime(1, 1, 1, 1, 1, 29));
  
  TEST_ASSERT_TRUE(x == 2);
}

void test_modifiers_and_intialization_state()
{
  TimeMatchedAction tma;
  TEST_ASSERT_TRUE(tma.isComplete());
  TEST_ASSERT_TRUE(tma.onRepeat());
  TEST_ASSERT_FALSE(tma.getInvocationDate().isValid());

  int x = 0;
  CIncrement incrementer(x);

  tma.setAction(&incrementer);
  TEST_ASSERT_EQUAL(&incrementer, tma.getAction());

  tma.setComplete(false);
  TEST_ASSERT_FALSE(tma.isComplete());

  tma.setMatchCriteria(MatchCriteria::Month);
  TEST_ASSERT_EQUAL(MatchCriteria::Month, tma.getMatchCriteria());

  tma.setInvocationDate(DateTime(2000, 10, 10, 10, 10, 10));
  TEST_ASSERT_TRUE(DateTime(2000, 10, 10, 10, 10, 10) == tma.getInvocationDate());

  tma.setRepeat(false);
  TEST_ASSERT_FALSE(tma.onRepeat());

  tma.setRepeat(true);
  TEST_ASSERT_TRUE(tma.onRepeat());

  TimeMatchedAction tma_full(&incrementer, DateTime(2000, 2, 2, 2, 2, 2), MatchCriteria::Month);
  TEST_ASSERT_EQUAL_PTR(&incrementer, tma_full.getAction());
  TEST_ASSERT_TRUE(DateTime(2000, 2, 2, 2, 2, 2) == tma_full.getInvocationDate());
  TEST_ASSERT_EQUAL(MatchCriteria::Month, tma_full.getMatchCriteria());
  TEST_ASSERT_FALSE(tma_full.isComplete());
  TEST_ASSERT_TRUE(tma_full.onRepeat());
}

void setup() {
  // NOTE!!! Wait for >2 secs
  // if board doesn't support software reset via Serial.DTR/RTS
  delay(2000);

  UNITY_BEGIN();
  RUN_TEST(test_match_criteria_year);
  RUN_TEST(test_match_criteria_month);
  RUN_TEST(test_criteria_day_of_month);
  RUN_TEST(test_criteria_day_of_week);
  RUN_TEST(test_invocation_criteria_hour);
  RUN_TEST(test_invocation_criteria_second);
  RUN_TEST(test_modifiers_and_intialization_state);
  UNITY_END();
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
}