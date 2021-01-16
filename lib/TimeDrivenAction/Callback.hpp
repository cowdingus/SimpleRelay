#pragma once

namespace tda
{
  class Callback
  {
  public:
    virtual void invoke() = 0;
  };
}