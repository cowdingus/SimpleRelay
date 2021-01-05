#include "Task.hpp"

Task::Task(String name)
  : name(name)
{

}

const String& Task::getName() const
{
  return name;
}