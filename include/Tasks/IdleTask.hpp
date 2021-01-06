#pragma once

#include "Task.hpp"

class IdleTask : public Task
{
public:
  IdleTask();

  void run();
};