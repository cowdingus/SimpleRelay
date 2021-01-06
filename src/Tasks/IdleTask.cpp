#include "Tasks/IdleTask.hpp"

#include <Arduino.h>

IdleTask::IdleTask()
  : Task("Idle")
{
  
}

void IdleTask::run(unsigned long deltaTime)
{
  delay(250);
}