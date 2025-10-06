#pragma once

//#include "Arduino.h"
#include "ErrorHandling/error_codes.h"
#include "TckChannelBase.h"

namespace TeensyTimerTool
{
    template <typename tckCounter> // tckCounter is the underlying counter, e.g. 32bit cycle counter, RTC counter, micros counter etc
    class TckChannel : public TckChannelBase
    {
        using counter_t = typename tckCounter::counter_t; // use the counter type (uint32_t, unit64_t...) of the used tckCounter class

     public:
        TckChannel();

        inline errorCode begin(callback_t cb, float period, bool periodic) override;
        inline errorCode start() override;
        inline errorCode stop() override;

        inline errorCode trigger(float delay_us) override;
        inline errorCode triggerDirect(counter_t reload) override;
        inline errorCode getTriggerReload(float delay, counter_t *reload) override;

        inline errorCode setNextPeriod(float us) override;
        inline errorCode setPeriod(float us) override;

        float getMaxPeriod() const override { return tckCounter::getMaxMicros() / 1E6f; } // seconds
        float getRemainingTime() const override { return (currentPeriod - (tckCounter::getCount() - startCnt)) / (float)TTT_F_CPU; }

     protected:
        inline bool tick();
        callback_t callback;
        counter_t startCnt, currentPeriod, nextPeriod;

        bool periodic;
        bool triggered;
    };

    // IMPLEMENTATION ==============================================

    template <typename tckCounter>
    TckChannel<tckCounter>::TckChannel()
    {
        triggered = false;
    }

    template <typename tckCounter>
    FLASHMEM errorCode TckChannel<tckCounter>::begin(callback_t cb, float period, bool periodic)
    {
        this->triggered = false;

        this->periodic = periodic;
        if (periodic)
        {
            this->currentPeriod = tckCounter::us2ticks(period);
            this->nextPeriod    = this->currentPeriod;
        }
        this->callback = cb;

        return errorCode::OK;
    }

    template <typename tckCounter>
    FLASHMEM errorCode TckChannel<tckCounter>::start()
    {
        this->startCnt  = tckCounter::getCount();
        this->triggered = true;
        return errorCode::OK;
    }

    template <typename tckCounter>
    FLASHMEM errorCode TckChannel<tckCounter>::stop()
    {
        this->triggered = false;
        return errorCode::OK;
    }

    template <typename tckCounter>
    FLASHMEM errorCode TckChannel<tckCounter>::triggerDirect(counter_t reload)
    {
        this->startCnt      = tckCounter::getCount();
        this->nextPeriod    = reload;
        this->currentPeriod = this->nextPeriod;
        this->triggered     = true;
        return errorCode::OK;
    }

    template <typename tckCounter>
    FLASHMEM errorCode TckChannel<tckCounter>::trigger(float delay) // µs
    {
        return triggerDirect(tckCounter::us2ticks(delay));
    }

    template <typename tckCounter>
    FLASHMEM errorCode TckChannel<tckCounter>::getTriggerReload(float delay, counter_t *reload)
    {
        *reload = tckCounter::us2ticks(delay);
        return errorCode::OK;
    }

    template <typename tckCounter>
    FLASHMEM errorCode TckChannel<tckCounter>::setNextPeriod(float us)
    {
        nextPeriod = tckCounter::us2ticks(us);
        return errorCode::OK;
    }

    template <typename tckCounter>
    FLASHMEM errorCode TckChannel<tckCounter>::setPeriod(float us)
    {
        counter_t newPeriod = tckCounter::us2ticks(us);
        counter_t now       = tckCounter::getCount();

        if (now - startCnt >= newPeriod) // new period already expired
        {                                //
            startCnt = now;              // -> restart cycle but,
            callback();                  // since expired, invoke callback now
        }                                //
        currentPeriod = newPeriod;       // in any case next callback will
        nextPeriod    = newPeriod;       // be invoked after newly set period
        return errorCode::OK;
    }

    template <typename tckCounter>
    FLASHMEM bool TckChannel<tckCounter>::tick()
    {
        static bool lock = false;
        counter_t now    = tckCounter::getCount();
        if (!lock && this->currentPeriod != 0 && this->triggered && (now - this->startCnt) >= this->currentPeriod)
        {
            lock = true;
            this->startCnt += currentPeriod;
            this->currentPeriod = nextPeriod;     // update period if it was changed during current cycle.
            this->triggered     = this->periodic; // i.e., stays triggerd if periodic, stops if oneShot
            callback();
            lock = false;
            return true;
        } else
        {
            return false;
        }
    }

} // namespace TeensyTimerTool
