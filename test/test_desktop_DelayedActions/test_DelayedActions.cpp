#include "DelayedActions.hpp"
#include "Callback.hpp"

#include <unity.h>

using namespace tda;

class CIncrement : public Callback
{
public:
  CIncrement(int& incVariable)
    : variable(incVariable)
  {
    
  }

  void invoke() override
  {
    ++variable;
  }

private:
  int& variable;
};

void test_set_action()
{
  int executionCount = 0;
  CIncrement cIncrement(executionCount);

  DelayedActions<2> delayedActions;
  delayedActions.setAction(&cIncrement, 1000, 0);
  delayedActions.setAction(&cIncrement, 500, 1);

  TEST_ASSERT_EQUAL(&cIncrement, delayedActions.getAction(0));
  TEST_ASSERT_EQUAL(&cIncrement, delayedActions.getAction(1));

  TEST_ASSERT_EQUAL(1000, delayedActions.getDelay(0));
  TEST_ASSERT_EQUAL(500, delayedActions.getDelay(1));
}

void test_set_delay()
{
  int executionCount = 0;
  CIncrement cIncrement(executionCount);

  DelayedActions<2> delayedActions;
  delayedActions.setDelay(1000, 0);
  delayedActions.setDelay(5000, 1);

  TEST_ASSERT_EQUAL(1000, delayedActions.getDelay(0));
  TEST_ASSERT_EQUAL(5000, delayedActions.getDelay(1));
}

void test_find_next_valid_action()
{
  int executionCount = 0;
  CIncrement cIncrement(executionCount);

  DelayedActions<3> delayedActions;
  delayedActions.setAction(&cIncrement, 1000, 0);
  delayedActions.setAction(&cIncrement, 2000, 2);

  TEST_ASSERT_EQUAL(0, delayedActions.getTurn());

  delayedActions.removeAction(0);
  TEST_ASSERT_EQUAL(2, delayedActions.getTurn());
}

void test_reset_clock()
{
  int executionCount = 0;
  CIncrement cIncrement(executionCount);

  DelayedActions<3> delayedActions;
  delayedActions.setAction(&cIncrement, 1000, 0);

  delayedActions.update(900);
  delayedActions.resetClock();
  delayedActions.update(1200);

  TEST_ASSERT_EQUAL(1, executionCount);
}

void test_quiet_when_empty()
{
  DelayedActions<3> delayedActions;

  for (int x = 100; x < 100; ++x)
  {
    delayedActions.update(100);
    TEST_ASSERT_EQUAL(0, delayedActions.getTurn());
  }
}

void test_skip_to_next_valid_action_when_current_is_not()
{
  int executionCount = 0;
  CIncrement cIncrement(executionCount);

  DelayedActions<2> delayedActions;
  delayedActions.setAction(&cIncrement, 500, 1);

  // Calling update will trigger if-empty callback check
  delayedActions.update(0);

  TEST_ASSERT_EQUAL(1, delayedActions.getTurn());
}

void test_turn_transfer()
{
  int executionCount = 0;
  CIncrement cIncrement(executionCount);

  DelayedActions<2> delayedActions;
  delayedActions.setAction(&cIncrement, 1000, 0);
  delayedActions.setAction(&cIncrement, 500, 1);

  TEST_ASSERT_EQUAL(0, executionCount);
  TEST_ASSERT_EQUAL(0, delayedActions.getTurn());

  delayedActions.setTurn(0);

  for (int x = 0; x < 10; ++x)
  {
    delayedActions.update(100);
  }

  TEST_ASSERT_EQUAL(1, executionCount);
  TEST_ASSERT_EQUAL(1, delayedActions.getTurn());

  for (int x = 0; x < 5; ++x)
  {
    delayedActions.update(100);
  }

  TEST_ASSERT_EQUAL(2, executionCount);
  TEST_ASSERT_EQUAL(0, delayedActions.getTurn());

  for (int x = 0; x < 10; ++x)
  {
    delayedActions.update(100);
  }

  TEST_ASSERT_EQUAL(3, executionCount);
  TEST_ASSERT_EQUAL(1, delayedActions.getTurn());

  for (int x = 0; x < 5; ++x)
  {
    delayedActions.update(100);
  }

  TEST_ASSERT_EQUAL(4, executionCount);
  TEST_ASSERT_EQUAL(0, delayedActions.getTurn());
}

void test_remove_action()
{
  int executionCount = 0;
  CIncrement cIncrement(executionCount);

  DelayedActions<2> delayedActions;
  delayedActions.setAction(&cIncrement, 1000, 0);
  delayedActions.setAction(&cIncrement, 500, 1);

  delayedActions.removeAction(1);

  TEST_ASSERT_EQUAL(nullptr, delayedActions.getAction(1));
}

int main(int argc, char **argv)
{
  UNITY_BEGIN();
  RUN_TEST(test_set_action);
  RUN_TEST(test_remove_action);
  RUN_TEST(test_set_delay);
  RUN_TEST(test_skip_to_next_valid_action_when_current_is_not);
  RUN_TEST(test_find_next_valid_action);
  RUN_TEST(test_turn_transfer);
  RUN_TEST(test_reset_clock);
  RUN_TEST(test_quiet_when_empty);
  UNITY_END();

  return 0;
}