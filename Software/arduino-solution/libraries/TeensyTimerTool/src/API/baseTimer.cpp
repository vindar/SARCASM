#include "baseTimer.h"
//#include "Arduino.h"
#include "types.h"

namespace TeensyTimerTool
{

    FLASHMEM BaseTimer::BaseTimer(TimerGenerator *generator, bool periodic)
        : timerGenerator(generator)
    {
        this->timerGenerator = generator;
        this->timerChannel   = nullptr;
        this->isPeriodic     = periodic;
    }

    FLASHMEM BaseTimer::~BaseTimer()
    {
        if (timerChannel != nullptr)
        {
            delete timerChannel;
        }
    }

    FLASHMEM errorCode BaseTimer::setPrescaler(int psc)
    {
        this->prescaler = psc;
        return errorCode::OK;
    }

} // namespace TeensyTimerTool