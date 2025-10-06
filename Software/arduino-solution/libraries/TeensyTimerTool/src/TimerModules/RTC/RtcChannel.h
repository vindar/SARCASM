#pragma once

#include "../../ITimerChannel.h"
#include "Arduino.h"
#include "ErrorHandling/error_codes.h"

namespace TeensyTimerTool
{
    class RtcChannel : public ITimerChannel
    {
     public:
        inline RtcChannel(callback_t *callbackSlot);
        inline ~RtcChannel();

        inline errorCode begin(callback_t cb, float period, bool periodic) override;
        inline errorCode start() override;
        inline errorCode stop() override;
        inline errorCode setPeriod(float us) override;
        float getMaxPeriod() const override { return 1.0; } // seconds

     protected:
        float period;
    };

    // IMPLEMENTATION ==============================================

    RtcChannel::RtcChannel(callback_t *callbackSlot)
    {
        pCallback = callbackSlot;
    }
    
    RtcChannel::~RtcChannel()
    {
        stop();
        pCallback = nullptr;
    }

    FLASHMEM errorCode RtcChannel::begin(callback_t cb, float period, bool periodic)
    {
        if (!periodic) return errorCode::notImplemented;

        stop();
        *pCallback   = cb;
        this->period = constrain(period, 1E6f / 32768.0f, 1E6f);
        return errorCode::OK;
    }

    FLASHMEM errorCode RtcChannel::start()
    {
        unsigned val = 32 - __builtin_clz(period * (32768 / 1E6) - 1); // next smallest power of 2;
        SNVS_HPCR = SNVS_HPCR_PI_FREQ(val);

        SNVS_HPCR |= SNVS_HPCR_PI_EN; // enable periodic interrupt
        while (!(SNVS_HPCR & SNVS_HPCR_PI_EN)) {}
        return errorCode::OK;
    }

    FLASHMEM errorCode RtcChannel::stop()
    {
        SNVS_HPCR &= ~SNVS_HPCR_PI_EN;
        while ((SNVS_HPCR & SNVS_HPCR_PI_EN)) {}
        return errorCode::OK;
    }

    FLASHMEM errorCode RtcChannel::setPeriod(float _us)
    {
        stop();
        period  = constrain(_us, 1E6f / 32788.0f, 1E6f);
        start();
        return errorCode::OK;
    }

} // namespace TeensyTimerTool
