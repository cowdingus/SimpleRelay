#ifndef NATIVE_UNIT_TEST

#pragma once

#include "Callback.hpp"

#include <RTClib.h>

namespace tda
{
    class AlarmAction
    {
    public:
        AlarmAction();
        AlarmAction(Callback* callback, const DateTime& invocationDate);

        void update(const DateTime& currentDateTime);

        void setActive(bool active);
        bool isActive() const;

        void setAction(Callback* callback);
        Callback* getAction() const;

        void setInvocationDate(const DateTime& invocationDate);
        const DateTime& getInvocationDate() const;
    
    private:
        Callback* action = nullptr;
        DateTime invocationTime = DateTime(0, 0, 0, 0, 0, 0);
        bool active = false;
    };
}

#endif