#pragma once

#include <stddef.h>
#include <stdint.h>

#if !defined(DEBUG) && !defined(UNIT_TEST)
  #define __ASSERT_USE_STDERR
#endif

#ifdef NATIVE_UNIT_TEST
  #define F(x) x
#else
  #include <WString.h>
#endif

#include <assert.h>

#include "Callback.hpp"

namespace tda
{
  template<size_t actionsCount>
  class DelayedActions
  {
  public:
    DelayedActions();

    void update(uint32_t deltaTime);
    
    void setTurn(size_t index);
    size_t getTurn() const;

    void setPause(bool pause);
    bool isPaused() const;

    void setRepeat(bool repeat);
    bool onRepeat() const;

    void resetClock();

    void setAction(Callback* callback, uint32_t delayms, size_t index);
    void removeAction(size_t index);
    Callback* getAction(size_t index) const;

    void setDelay(uint32_t delayms, size_t index);
    uint32_t getDelay(size_t index) const;

    bool isEmpty() const;

  private:
    Callback* actions[actionsCount] = {nullptr};
    uint32_t actionsDelay[actionsCount] = {1000};

    size_t nextAction = 0;

    bool onPause = false;
    bool repeat = true;

    size_t findNextValidAction(size_t index) const __attribute__ ((warn_unused_result));
    bool isOnLastTurn() const;

    uint32_t timeElapsed = 0;
  };

  template<size_t actionsCount>
  DelayedActions<actionsCount>::DelayedActions()
  {

  }

  template<size_t actionsCount>
  void DelayedActions<actionsCount>::update(uint32_t deltaTime)
  {
    if (onPause || isEmpty()) return;

    // Go directly to least valid action when current is not valid
    if (!actions[nextAction])
      nextAction = findNextValidAction(nextAction);

    timeElapsed += deltaTime;

    while (timeElapsed >= actionsDelay[nextAction])
    {
      timeElapsed -= actionsDelay[nextAction];

      assert(actions[nextAction] && F("Reached impossible state, unexpected: callback is nullptr"));
      actions[nextAction]->invoke();

      if (isOnLastTurn() && !onRepeat()) setPause(true); 

      nextAction = findNextValidAction(nextAction);
    }
  }

  template<size_t actionsCount>
  void DelayedActions<actionsCount>::setAction(Callback* callback, uint32_t delayms, size_t index)
  {
    assert(index < actionsCount && F("Out-of-bound in DelayedActions<>::setAction"));
    
    actions[index] = callback;
    actionsDelay[index] = delayms;
  }

  template<size_t actionsCount>
  void DelayedActions<actionsCount>::removeAction(size_t index)
  {
    assert(index < actionsCount && F("Out-of-bound in DelayedActions<>::removeAction"));

    actions[index] = nullptr;

    if (nextAction == index)
      nextAction = findNextValidAction(index);
  }

  template<size_t actionsCount>
  Callback* DelayedActions<actionsCount>::getAction(size_t index) const
  {
    assert(index < actionsCount && F("Out-of-bound in DelayedActions<>::getAction"));

    return actions[index]; 
  }

  template<size_t actionsCount>
  void DelayedActions<actionsCount>::setPause(bool pause)
  {
    onPause = pause;
  }

  template<size_t actionsCount>
  bool DelayedActions<actionsCount>::isPaused() const
  {
    return onPause;
  }

  template<size_t actionsCount>
  void DelayedActions<actionsCount>::setRepeat(bool repeat)
  {
    this->repeat = repeat;
  }

  template<size_t actionsCount>
  bool DelayedActions<actionsCount>::onRepeat() const
  {
    return repeat;
  }

  template<size_t actionsCount>
  void DelayedActions<actionsCount>::resetClock()
  {
    timeElapsed = 0;
  }

  template<size_t actionsCount>
  size_t DelayedActions<actionsCount>::findNextValidAction(size_t currentIndex) const
  {
    for (size_t i = 1; i < actionsCount; ++i)
    {
      size_t nextIndex = (currentIndex + i) % actionsCount;

      if (actions[nextIndex])
        return nextIndex;
    }

    return currentIndex;
  }

  template<size_t actionsCount>
  bool DelayedActions<actionsCount>::isOnLastTurn() const
  {
    if (findNextValidAction(nextAction) < nextAction)
      return true;
  }

  template<size_t actionsCount>
  bool DelayedActions<actionsCount>::isEmpty() const
  {
    for (Callback* callback : actions)
    {
      if (callback)
        return false;
    }
    
    return true;
  }

  template<size_t actionsCount>
  size_t DelayedActions<actionsCount>::getTurn() const
  {
    return nextAction;
  }

  template<size_t actionsCount>
  void DelayedActions<actionsCount>::setTurn(size_t index)
  {
    // Check if callback at index is valid, if it isn't search for
    // its next nearest valid action, if there isn't any, just leave
    // next action to `index` meaning that next action is pointing
    // to a nullptr callback pointer.
    if (getAction(index))
    {
      nextAction = index;
    }
    else
    {
      nextAction = findNextValidAction(index);
    }
  }

  template<size_t actionsCount>
  void DelayedActions<actionsCount>::setDelay(uint32_t delayms, size_t index)
  {
    assert(index < actionsCount && F("Out-of-bound in DelayedActions<>::setDelay"));

    actionsDelay[index] = delayms;
  }

  template<size_t actionsCount>
  uint32_t DelayedActions<actionsCount>::getDelay(size_t index) const
  {
    assert(index < actionsCount && F("Out-of-bound in DelayedActions<>::getDelay"));

    return actionsDelay[index];
  }
}