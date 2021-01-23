#ifndef NATIVE_UNIT_TEST

#include "AlarmAction.hpp"

namespace tda
{
    AlarmAction::AlarmAction()
        : active(false)
    {

    }

    AlarmAction::AlarmAction(Callback* callback, const DateTime& invocationDate)
        : active(true)
    {
        setAction(callback);
        setInvocationDate(invocationDate);
    }

    void AlarmAction::update(const DateTime& currentDateTime)
    {
        if (!active) return;

        if (currentDateTime > invocationTime)
        {
            if (action)
                action->invoke();
            
            active = false;
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
}

#endif