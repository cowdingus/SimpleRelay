#ifndef NATIVE_UNIT_TEST

#pragma once

#include "RTClib.h"
#include "Callback.hpp"

#if !defined(DEBUG) && !defined(UNIT_TEST)
  #define __ASSERT_USE_STDERR
#endif

#ifdef NATIVE_UNIT_TEST
  #define F(x) x
#else
  #include <WString.h>
#endif

#include <assert.h>

#define RAS_MINIMUM_INTERVAL 1

namespace tda
{
    class RepeatingAction_Second
    {
    public:
        RepeatingAction_Second();
        RepeatingAction_Second(Callback* callback, TimeSpan interval, bool overtimeCompensation = false);

        void update(TimeSpan elapsedTime);
        void resetClock();

        void setInterval(TimeSpan interval);
        TimeSpan getInterval() const;

        void setActive(bool active);
        bool isActive() const;

        void setAction(Callback* callback);
        Callback* getAction() const;

        void setOvertimeCompensation(bool compensation);
        bool getOvertimeCompensation() const;

    private:
        TimeSpan executionInterval = 1000;
        TimeSpan timeElapsed = 0;

        bool active = true;
        bool overtimeCompensation = true;

        Callback* actionCallback = nullptr;  
    };
}
#endif