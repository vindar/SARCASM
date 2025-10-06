#pragma once

#include "ErrorHandling/error_codes.h"
#include "baseTimer.h"
#include "type_traits"

namespace TeensyTimerTool
{
    class OneShotTimer : public BaseTimer
    {
     public:
        inline OneShotTimer(TimerGenerator *generator = nullptr);

        inline errorCode begin(callback_t cb);
        template <typename T> errorCode trigger(T delay);
        template <typename T> errorCode triggerDirect(T reload);
        template <typename T> errorCode getTriggerReload(float delay, T *reload);
    };

    // Implementation ================================================

    OneShotTimer::OneShotTimer(TimerGenerator *generator)
        : BaseTimer(generator, false)
    {}

    FLASHMEM errorCode OneShotTimer::begin(callback_t callback)
    {
        return BaseTimer::begin(callback, 0, false);
    }

    template <typename period_t>
    FLASHMEM errorCode OneShotTimer::trigger(period_t delay)
    {
        if (timerChannel)
            return postError(timerChannel->trigger(period2us(delay)));

        return postError(errorCode::notInitialized);
    }

    template <typename T>
    FLASHMEM errorCode OneShotTimer::triggerDirect(T reload)
    {
        if (timerChannel)
            return postError(timerChannel->triggerDirect(reload));

        return postError(errorCode::notInitialized);
    }

    template <typename T>
    FLASHMEM errorCode OneShotTimer::getTriggerReload(float delay, T *reload)
    {
        if (timerChannel)
            return postError(timerChannel->getTriggerReload(delay, reload));

        return postError(errorCode::notInitialized);
    }
} // namespace TeensyTimerTool