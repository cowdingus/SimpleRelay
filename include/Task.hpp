#pragma once

#include <WString.h>

class Task
{
public:
  Task(String name);

  const String& getName() const;

private:
  const String name = "Unnamed Task";
};