#ifndef NATIVE_UNIT_TEST

#pragma once

#include "Callback.hpp"

#include <RTClib.h>

namespace tda
{
    enum class RepeatMode
    {
        Once,
        EveryMinute,
        EveryHour,
        EveryDay
    };

    class AlarmAction
    {
    public:
        AlarmAction();
        AlarmAction(Callback* callback, const DateTime& invocationDate, RepeatMode repeatMode = RepeatMode::Once);

        void update(const DateTime& currentDateTime);

        void setActive(bool active);
        bool isActive() const;

        void setRepeatMode(RepeatMode repeatMode);
        RepeatMode getRepeatMode() const;

        void setAction(Callback* callback);
        Callback* getAction() const;

        void setInvocationDate(const DateTime& invocationDate);
        const DateTime& getInvocationDate() const;
    
    private:
        Callback* action = nullptr;
        DateTime invocationTime = DateTime(0, 0, 0, 0, 0, 0);
        bool active = false;

        RepeatMode repeatMode = RepeatMode::Once;

        DateTime getNextInvocationTime() const;
    };

    void setAtMinute(uint8_t minute, const DateTime& now, AlarmAction& alarmAction);
    void setAtHour(uint8_t hour, const DateTime& now, AlarmAction& alarmAction);
}

#endif