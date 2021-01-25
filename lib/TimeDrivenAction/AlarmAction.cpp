#ifndef NATIVE_UNIT_TEST

#include "AlarmAction.hpp"

namespace tda
{
    AlarmAction::AlarmAction()
        : active(false)
    {

    }

    AlarmAction::AlarmAction(Callback* callback, const DateTime& invocationDate, RepeatMode repeatMode)
        : active(true)
    {
        setAction(callback);
        setInvocationDate(invocationDate);
        setRepeatMode(repeatMode);
    }

    void AlarmAction::update(const DateTime& currentDateTime)
    {
        if (!active) return;

        if (currentDateTime >= invocationTime)
        {
            if (action)
                action->invoke();
            
            if (repeatMode == RepeatMode::Once)
            {
                active = false;
            }
            else
            {
                invocationTime = getNextInvocationTime();
            }
        }
    }

    void AlarmAction::setActive(bool active)
    {
        this->active = active;
    }

    bool AlarmAction::isActive() const
    {
        return active;
    }

    void AlarmAction::setRepeatMode(RepeatMode repeatMode)
    {
        this->repeatMode = repeatMode;
    }

    RepeatMode AlarmAction::getRepeatMode() const
    {
        return repeatMode;
    }

    void AlarmAction::setAction(Callback* callback)
    {
        action = callback;
    }

    Callback* AlarmAction::getAction() const
    {
        return action;
    }

    void AlarmAction::setInvocationDate(const DateTime& invocationDate)
    {
        invocationTime = invocationDate;
    }

    const DateTime& AlarmAction::getInvocationDate() const
    {
        return invocationTime;
    }

    DateTime AlarmAction::getNextInvocationTime() const
    {
        DateTime nextInvocationTime = invocationTime;
        
        switch (repeatMode)
        {
        case RepeatMode::EveryMinute:
            return nextInvocationTime + TimeSpan(0, 0, 0, 60);
            break;

        case RepeatMode::EveryHour:
            return nextInvocationTime + TimeSpan(0, 0, 60, 0);
            break;

        case RepeatMode::EveryDay:
            return nextInvocationTime + TimeSpan(0, 24, 0, 0);
            break;
        
        default:
            return DateTime(0, 0, 0, 0, 0, 0);
            break;
        }
    }

    void setAtSecond(uint8_t second, const DateTime& now, AlarmAction& alarmAction)
    {
        DateTime invocationDate = 
            DateTime(now.year(), now.month(), now.day(), now.hour(), now.minute(), second);

        if (now.second() > second)
        {
            invocationDate = invocationDate + TimeSpan(0, 0, 1, 0);
        }

        alarmAction.setRepeatMode(tda::RepeatMode::EveryMinute);
        alarmAction.setInvocationDate(invocationDate);
    }

    void setAtMinute(uint8_t minute, const DateTime& now, AlarmAction& alarmAction)
    {
        DateTime invocationDate =
            DateTime(now.year(), now.month(), now.day(), now.hour(), minute, 0);

        if (now.minute() > minute)
        {
            invocationDate = invocationDate + TimeSpan(0, 1, 0, 0);
        }

        alarmAction.setRepeatMode(tda::RepeatMode::EveryHour);
        alarmAction.setInvocationDate(invocationDate);
    }

    void setAtHour(uint8_t hour, const DateTime& now, AlarmAction& alarmAction)
    {
        DateTime invocationDate =
            DateTime(now.year(), now.month(), now.day(), hour, 0, 0);

        if (now.hour() > hour)
        {
            invocationDate = invocationDate + TimeSpan(1, 0, 0, 0);
        }

        alarmAction.setRepeatMode(tda::RepeatMode::EveryDay);
        alarmAction.setInvocationDate(invocationDate);
    }
}

#endif