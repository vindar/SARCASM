#pragma once

#include "Arduino.h"
#include "ITimerChannel.h"

namespace TeensyTimerTool
{
    class PIT_t;

    class PITChannel : public ITimerChannel
    {
     public:
        inline PITChannel(unsigned nr);
        inline virtual ~PITChannel();

        inline errorCode begin(callback_t cb, float tcnt, bool periodic) override;
        inline errorCode start() override;
        inline errorCode stop() override;

        inline errorCode trigger(float) override;
        inline errorCode setNextPeriod(float us) override;
        inline errorCode setPeriod(float us) override;

        inline float getMaxPeriod() const override;

        bool isPeriodic;

     protected:
        inline void isr();

        inline uint32_t us2ticks(float micros) const;
        float clock;

        const unsigned chNr;
        callback_t callback = nullptr;

        friend PIT_t;
        PITChannel()                   = delete;
        PITChannel(const PITChannel &) = delete;
    };

    // IMPLEMENTATION ==============================================

    PITChannel::PITChannel(unsigned nr)
        : ITimerChannel(nullptr), chNr(nr)
    {
        callback = nullptr;        
    }

    FLASHMEM errorCode PITChannel::begin(callback_t cb, float micros, bool periodic)
    {
        clock = (CCM_CSCMR1 & CCM_CSCMR1_PERCLK_CLK_SEL) ? 24 : (F_BUS_ACTUAL / 1000000);        
        isPeriodic = periodic;
        callback   = cb;

        if (isPeriodic)
        {
            IMXRT_PIT_CHANNELS[chNr].TCTRL = 0;
            IMXRT_PIT_CHANNELS[chNr].TFLG  = 1;
            setNextPeriod(micros);
        }
        return errorCode::OK;
    }

    FLASHMEM uint32_t PITChannel::us2ticks(float micros) const
    {
        if (micros > getMaxPeriod())
        {
            micros = getMaxPeriod();
            postError(errorCode::periodOverflow);
        }

        //Serial.printf("pit p:%f %d\n", clock, (uint32_t)(clock * micros) - 1);
        return (uint32_t)(clock * micros) - 1;
    }

    FLASHMEM errorCode PITChannel::start()
    {
        IMXRT_PIT_CHANNELS[chNr].TCTRL = PIT_TCTRL_TEN | PIT_TCTRL_TIE;
        return errorCode::OK;
    }

    FLASHMEM errorCode PITChannel::stop()
    {
        IMXRT_PIT_CHANNELS[chNr].TCTRL = 0;
        return errorCode::OK;
    }

    FLASHMEM errorCode PITChannel::setNextPeriod(float us)
    {
        IMXRT_PIT_CHANNELS[chNr].LDVAL = us2ticks(us);
        return errorCode::OK;
    }

    FLASHMEM errorCode PITChannel::setPeriod(float us)
    {
        stop(); // need to stop/start timer to change current period (see ch 53.9.5.4)
        setNextPeriod(us);
        start();
        return errorCode::OK;
    }

    FLASHMEM void PITChannel::isr()
    {
        if (callback != nullptr)
        {
            callback();
            if (!isPeriodic) IMXRT_PIT_CHANNELS[chNr].TCTRL = 0; // switch off timer
        }
    }

    PITChannel::~PITChannel()
    {
        callback = nullptr;
    }

    FLASHMEM errorCode PITChannel::trigger(float delay) // should be optimized somehow
    {
        IMXRT_PIT_CHANNELS[chNr].TCTRL = 0;
        IMXRT_PIT_CHANNELS[chNr].TFLG  = 1;

        setNextPeriod(delay);
        start();

        return errorCode::OK;
    }

    FLASHMEM float PITChannel::getMaxPeriod() const
    {
        return (float)0xFFFF'FFFE / clock / 1'000'000;
    }

} // namespace TeensyTimerTool