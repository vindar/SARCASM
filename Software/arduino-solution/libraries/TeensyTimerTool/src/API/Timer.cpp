#include "timer.h"
#include "config.h"

namespace TeensyTimerTool
{
    Timer::Timer(TimerGenerator *generator)
        : BaseTimer(generator, true)
    {
    }
} // namespace TeensyTimerTool
