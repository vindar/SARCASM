#pragma once

#include "Arduino.h"
#include "ErrorHandling/error_codes.h"
#include "ITimerChannel.h"
#include "helpers.h"
#include <type_traits>

#if defined(USE_TIME_LITERALS)
//#include "frequency.h"
#include <chrono>
#include <cmath>
using namespace std::chrono_literals;
using namespace std::chrono;
#endif

namespace TeensyTimerTool
{
    class BaseTimer
    {
     public:
        template <typename period_t>
        inline errorCode begin(callback_t callback, period_t period, bool start = true);
        inline errorCode setPrescaler(int psc);
        inline errorCode end();
        inline errorCode start();
        inline errorCode stop();

        inline float getMaxPeriod() const;
        inline float getRemainingTime() const;

     protected:
        BaseTimer(TimerGenerator *generator, bool periodic);
        virtual ~BaseTimer();

        TimerGenerator *timerGenerator;
        ITimerChannel *timerChannel;
        bool isPeriodic;
        uint32_t prescaler = 0;
    };

    // INLINE IMPLEMENTATION ================================================

    template <typename period_t>
    FLASHMEM errorCode BaseTimer::begin(callback_t callback, period_t p, bool start)
    {
        float period = period2us(p); // transform from any period type to microseconds (float)

        if (callback == nullptr) return postError(errorCode::callback);
        if (isPeriodic && period == 0) return postError(errorCode::reload);

        if (timerChannel == nullptr)
        {
            if (timerGenerator != nullptr) // use timer passed in during construction
            {
                timerChannel = timerGenerator();
                if (timerChannel == nullptr) return postError(errorCode::noFreeChannel);
            } else //find the next free timer
            {
                for (unsigned i = 0; timerChannel == nullptr && i < timerCnt; i++)
                {
                    timerChannel = timerPool[i]();
                }
            }
            if (timerChannel == nullptr) return postError(errorCode::noFreeModule);
        }

        errorCode result = timerChannel->begin(callback, period, isPeriodic);

        if (result == errorCode::OK)
        {
            if (isPeriodic && start) timerChannel->start();
        }
        return postError(result);
    }

    FLASHMEM errorCode BaseTimer::end()
    {
        return postError(errorCode::notImplemented);
    }

    FLASHMEM errorCode BaseTimer::start()
    {
        if (timerChannel)
            return postError(timerChannel->start());

        return postError(errorCode::notInitialized);
    }

    FLASHMEM errorCode BaseTimer::stop()
    {
        if (timerChannel)
            return postError(timerChannel->stop());

        return postError(errorCode::notInitialized);
    }

    FLASHMEM float BaseTimer::getMaxPeriod() const
    {
        if (timerChannel != nullptr)
            return timerChannel->getMaxPeriod();

        postError(errorCode::notInitialized);
        return NAN;
    }

    FLASHMEM float BaseTimer::getRemainingTime() const
    {
         if (timerChannel != nullptr)
             return timerChannel->getRemainingTime();

        postError(errorCode::notInitialized);
        return NAN;
    }
} // namespace TeensyTimerTool