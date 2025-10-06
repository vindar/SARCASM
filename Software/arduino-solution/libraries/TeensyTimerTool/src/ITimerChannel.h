#pragma once

#include "types.h"
#include <cmath>

namespace TeensyTimerTool
{
    class ITimerChannel
    {
     public:
        virtual errorCode begin(callback_t callback, float period, bool oneShot) = 0;
        virtual errorCode trigger(float delay)    {return errorCode::notImplemented;} 
        virtual errorCode triggerDirect(uint32_t reload) { return postError(errorCode::notImplemented); };
        virtual errorCode triggerDirect(uint64_t reload) { return postError(errorCode::notImplemented); };
        virtual errorCode getTriggerReload(float delay, uint32_t *reload) { return postError(errorCode::notImplemented); };
        virtual errorCode getTriggerReload(float delay, uint64_t *reload) { return postError(errorCode::notImplemented); };

        virtual errorCode start() = 0;
        virtual errorCode stop()  = 0;

        virtual errorCode setPrescaler(int psc) { return postError(errorCode::notImplemented); }

        virtual float getMaxPeriod() const = 0;
        virtual float getRemainingTime() const { postError(errorCode::notImplemented); return NAN; }
        virtual errorCode setPeriod(float microSeconds) { return postError(errorCode::notImplemented); };
        virtual errorCode setNextPeriod(float microSeconds) { return postError(errorCode::notImplemented); };
        virtual uint32_t getPeriod() { return 0; }

        inline void setCallback(callback_t);

        virtual ~ITimerChannel(){};

     protected:
        inline ITimerChannel(callback_t *cbStorage = nullptr);
        callback_t *pCallback;
    };

    // IMPLEMENTATION ====================================================

    ITimerChannel::ITimerChannel(callback_t *cbStorage)
    {
        this->pCallback = cbStorage;
    }

    void ITimerChannel::setCallback(callback_t cb)
    {
        *pCallback = cb;
    }

} // namespace TeensyTimerTool