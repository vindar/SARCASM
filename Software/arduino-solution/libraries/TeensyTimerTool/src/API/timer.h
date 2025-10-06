#pragma once
#include "ErrorHandling/error_codes.h"
#include "ITimerChannel.h"
#include "baseTimer.h"
#include "types.h"

namespace TeensyTimerTool
{
    class [[deprecated("Consider using PeriodicTimer or OneShotTimer instead")]] Timer : public BaseTimer
    {
     public:
        Timer(TimerGenerator *gen = nullptr);

        inline errorCode beginPeriodic(callback_t cb, uint32_t period)
        {
            isPeriodic = true;
            return BaseTimer::begin(cb, period, true);
        }
        inline errorCode beginOneShot(callback_t cb)
        {
            isPeriodic = false;
            return BaseTimer::begin(cb, 0, false);
        }
        inline void trigger(uint32_t delay);
    };

    // IMPLEMENTATION =======================================================

    void Timer::trigger(const uint32_t delay)
    {
        timerChannel->trigger(delay);
    }
} // namespace TeensyTimerTool
