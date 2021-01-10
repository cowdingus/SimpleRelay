#include "TimedAction.hpp"
#include "Callback.hpp"

#include <unity.h>

void test_interval_modifier()
{
  TimedAction timedAction;

  timedAction.setInterval(3000);
  TEST_ASSERT_EQUAL(3000, timedAction.getInterval());
}

void test_active_modifier()
{
  TimedAction timedAction;

  timedAction.setActive(true);
  TEST_ASSERT_EQUAL(true, timedAction.isActive());
}

void test_overtimecompensation_modifier()
{
  TimedAction timedAction;

  timedAction.setOvertimeCompensation(false);
  TEST_ASSERT_EQUAL(false, timedAction.getOvertimeCompensation());

  timedAction.setOvertimeCompensation(true);
  TEST_ASSERT_EQUAL(true, timedAction.getOvertimeCompensation());
}

void test_action_modifier()
{
  TimedAction timedAction;

  class CCallback : public Callback
  {

  };
}

int main(int argc, char **argv)
{
  UNITY_BEGIN();
  RUN_TEST(test_interval_modifier);
  RUN_TEST(test_active_modifier);
  RUN_TEST(test_overtimecompensation_modifier);
  UNITY_END();

  return 0;
}