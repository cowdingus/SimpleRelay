#include "RepeatingAction.hpp"
#include "Callback.hpp"

#include <unity.h>

using namespace tda;

void test_interval_modifier()
{
  RepeatingAction repeatingAction;

  repeatingAction.setInterval(3000);
  TEST_ASSERT_EQUAL(3000, repeatingAction.getInterval());
}

void test_active_modifier()
{
  RepeatingAction repeatingAction;

  repeatingAction.setActive(true);
  TEST_ASSERT_EQUAL(true, repeatingAction.isActive());
}

void test_overtimecompensation_modifier()
{
  RepeatingAction repeatingAction;

  repeatingAction.setOvertimeCompensation(false);
  TEST_ASSERT_EQUAL(false, repeatingAction.getOvertimeCompensation());

  repeatingAction.setOvertimeCompensation(true);
  TEST_ASSERT_EQUAL(true, repeatingAction.getOvertimeCompensation());
}

void test_action_modifier()
{
  RepeatingAction repeatingAction;

  class CDoNothing : public Callback
  {
  public:
    CDoNothing() {}
    
    void invoke() override {}
  };

  CDoNothing cDoNothing;

  repeatingAction.setAction(&cDoNothing);

  TEST_ASSERT_EQUAL(&cDoNothing, repeatingAction.getAction());
}

void test_action_execution()
{
  RepeatingAction repeatingAction;

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

  repeatingAction.setAction(&cIncrement);
  repeatingAction.setInterval(2000);
  repeatingAction.setActive(true);

  repeatingAction.update(1000);
  TEST_ASSERT_EQUAL(0, executionCount);

  repeatingAction.update(1000);
  TEST_ASSERT_EQUAL(1, executionCount);

  repeatingAction.update(1000);
  TEST_ASSERT_EQUAL(1, executionCount);

  repeatingAction.update(1000);
  TEST_ASSERT_EQUAL(2, executionCount);
}

void test_overtimecompensation_behavior()
{
  RepeatingAction repeatingAction;

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

  repeatingAction.setAction(&cIncrement);
  repeatingAction.setInterval(2000);
  repeatingAction.setActive(true);

  // Default behavior, Overtime Compensation is activated
  TEST_ASSERT_EQUAL(true, repeatingAction.getOvertimeCompensation());

  repeatingAction.update(4000);
  TEST_ASSERT_EQUAL(2, executionCount);

  repeatingAction.update(6000);
  TEST_ASSERT_EQUAL(5, executionCount);

  // Overtime Compensation off
  executionCount = 0;
  repeatingAction.setOvertimeCompensation(false);

  repeatingAction.update(100000);
  TEST_ASSERT_EQUAL(1, executionCount);
}

void test_empty_constructed_behavior()
{
  RepeatingAction repeatingAction;

  TEST_ASSERT_EQUAL(false, repeatingAction.isActive());
  TEST_ASSERT_EQUAL(nullptr, repeatingAction.getAction());
  TEST_ASSERT_EQUAL(1000, repeatingAction.getInterval());
  TEST_ASSERT_EQUAL(true, repeatingAction.getOvertimeCompensation());
}

void test_reset_clock()
{
  RepeatingAction repeatingAction;

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

  repeatingAction.setAction(&cIncrement);
  repeatingAction.setInterval(2000);
  repeatingAction.setActive(true);

  repeatingAction.update(1900);
  repeatingAction.resetClock();
  TEST_ASSERT_EQUAL(0, executionCount);

  repeatingAction.update(2100);
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

  RepeatingAction repeatingAction(&cDoNothing, 2000);
  TEST_ASSERT_EQUAL(&cDoNothing, repeatingAction.getAction());
  TEST_ASSERT_EQUAL(2000, repeatingAction.getInterval());
  TEST_ASSERT_EQUAL(true, repeatingAction.isActive());
  TEST_ASSERT_EQUAL(true, repeatingAction.getOvertimeCompensation());
}

void test_activeness()
{
  RepeatingAction repeatingAction;

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

  repeatingAction.setAction(&cIncrement);
  repeatingAction.setInterval(2000);
  repeatingAction.setActive(false);

  repeatingAction.update(3000);
  TEST_ASSERT_EQUAL(0, executionCount);

  repeatingAction.setActive(true);
  repeatingAction.update(1000);
  TEST_ASSERT_EQUAL(1, executionCount);
}

int main()
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