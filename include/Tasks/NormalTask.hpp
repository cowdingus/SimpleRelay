#pragma once

#include "Task.hpp"
#include "TimedAction.hpp"

class NormalTask : public Task
{
public:
  NormalTask();

  void run(unsigned long deltaTime);

private:
  TimedAction relay1Action;
  TimedAction relay2Action;
};