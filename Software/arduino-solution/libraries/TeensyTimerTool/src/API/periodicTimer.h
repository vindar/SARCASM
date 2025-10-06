#pragma once

#include "baseTimer.h"

namespace TeensyTimerTool
{
    class PeriodicTimer : public BaseTimer
    {
     public:
        PeriodicTimer(TimerGenerator *generator = nullptr) : BaseTimer(generator, true) {}

        template <typename period_t>
        inline errorCode setPeriod(period_t p) { return postError(timerChannel->setPeriod(period2us(p))); };

        template <typename period_t>
        inline errorCode setNextPeriod(period_t p) { return postError(timerChannel->setNextPeriod(period2us(p))); };
    };

    // IMPLEMENTATION =====================================================================

} // namespace TeensyTimerTool