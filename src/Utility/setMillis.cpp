#include "Utility/setMillis.hpp"

#include <util/atomic.h>

void setMillis(uint32_t ms)
{
    extern unsigned long timer0_millis;
    ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
        timer0_millis = ms;
    }
}