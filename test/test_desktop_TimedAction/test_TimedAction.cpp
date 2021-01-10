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

  class CDoNothing : public Callback
  {
  public:
    CDoNothing() {}
    
    void invoke() override {}
  };

  CDoNothing cDoNothing;

  timedAction.setAction(&cDoNothing);

  TEST_ASSERT_EQUAL(&cDoNothing, timedAction.getAction());
}

void test_action_execution()
{
  TimedAction timedAction;

  class CIncrement : public Callback
  {
  public:
    CIncrement(int& var) : var(var) {}

    void invoke() override { ++var; }
  private:
    int& var;
  };

  int executionCount = 0;
  CIncrement cIncrement(executionCount);

  timedAction.setAction(&cIncrement);
  timedAction.setInterval(2000);
  timedAction.setActive(true);

  timedAction.update(1000);
  TEST_ASSERT_EQUAL(0, executionCount);

  timedAction.update(1000);
  TEST_ASSERT_EQUAL(1, executionCount);

  timedAction.update(1000);
  TEST_ASSERT_EQUAL(1, executionCount);

  timedAction.update(1000);
  TEST_ASSERT_EQUAL(2, executionCount);
}

void test_overtimecompensation_behavior()
{
  TimedAction timedAction;

  class CIncrement : public Callback
  {
  public:
    CIncrement(int& var) : var(var) {}

    void invoke() override { ++var; }
  private:
    int& var;
  };

  int executionCount = 0;
  CIncrement cIncrement(executionCount);

  timedAction.setAction(&cIncrement);
  timedAction.setInterval(2000);
  timedAction.setActive(true);

  // Default behavior, Overtime Compensation is activated
  TEST_ASSERT_EQUAL(true, timedAction.getOvertimeCompensation());

  timedAction.update(4000);
  TEST_ASSERT_EQUAL(2, executionCount);

  timedAction.update(6000);
  TEST_ASSERT_EQUAL(5, executionCount);

  // Overtime Compensation off
  executionCount = 0;
  timedAction.setOvertimeCompensation(false);

  timedAction.update(100000);
  TEST_ASSERT_EQUAL(1, executionCount);
}

void test_empty_constructed_behavior()
{
  TimedAction timedAction;

  TEST_ASSERT_EQUAL(false, timedAction.isActive());
  TEST_ASSERT_EQUAL(nullptr, timedAction.getAction());
  TEST_ASSERT_EQUAL(1000, timedAction.getInterval());
  TEST_ASSERT_EQUAL(true, timedAction.getOvertimeCompensation());
}

void test_reset_clock()
{
  TimedAction timedAction;

  class CIncrement : public Callback
  {
  public:
    CIncrement(int& var) : var(var) {}

    void invoke() override { ++var; }
  private:
    int& var;
  };

  int executionCount = 0;
  CIncrement cIncrement(executionCount);

  timedAction.setAction(&cIncrement);
  timedAction.setInterval(2000);
  timedAction.setActive(true);

  timedAction.update(1900);
  timedAction.resetClock();
  TEST_ASSERT_EQUAL(0, executionCount);

  timedAction.update(2100);
  TEST_ASSERT_EQUAL(1, executionCount);
}

void test_full_constructed_behavior()
{
  class CDoNothing : public Callback
  {
  public:
    CDoNothing() {}
    
    void invoke() override {}
  };

  CDoNothing cDoNothing;

  TimedAction timedAction(&cDoNothing, 2000);
  TEST_ASSERT_EQUAL(&cDoNothing, timedAction.getAction());
  TEST_ASSERT_EQUAL(2000, timedAction.getInterval());
  TEST_ASSERT_EQUAL(true, timedAction.isActive());
  TEST_ASSERT_EQUAL(true, timedAction.getOvertimeCompensation());
}

void test_activeness()
{
  TimedAction timedAction;

  class CIncrement : public Callback
  {
  public:
    CIncrement(int& var) : var(var) {}

    void invoke() override { ++var; }
  private:
    int& var;
  };

  int executionCount = 0;
  CIncrement cIncrement(executionCount);

  timedAction.setAction(&cIncrement);
  timedAction.setInterval(2000);
  timedAction.setActive(false);

  timedAction.update(3000);
  TEST_ASSERT_EQUAL(0, executionCount);

  timedAction.setActive(true);
  timedAction.update(1000);
  TEST_ASSERT_EQUAL(1, executionCount);
}

int main(int argc, char **argv)
{
  UNITY_BEGIN();
  RUN_TEST(test_interval_modifier);
  RUN_TEST(test_active_modifier);
  RUN_TEST(test_overtimecompensation_modifier);
  RUN_TEST(test_action_modifier);
  RUN_TEST(test_action_execution);
  RUN_TEST(test_overtimecompensation_behavior);
  RUN_TEST(test_empty_constructed_behavior);
  RUN_TEST(test_full_constructed_behavior);
  RUN_TEST(test_reset_clock);
  RUN_TEST(test_activeness);
  UNITY_END();

  return 0;
}