#pragma once

#include "Arduino.h"
#include "FTM_ChannelInfo.h"
#include "FTM_Info.h"
#include "ITimerChannel.h"

namespace TeensyTimerTool
{
    class FTM_Channel : public ITimerChannel
    {
     public:
        inline FTM_Channel(FTM_r_t *regs, FTM_ChannelInfo *ci);
        inline virtual ~FTM_Channel();

        inline float getMaxPeriod() const override;

        inline errorCode begin(callback_t cb, float period, bool periodic) { return doBegin(cb, period, periodic); } ///hacky improve in later version
        inline errorCode begin(callback_t cb, uint32_t period, bool periodic) { return doBegin(cb, period, periodic); };

        inline errorCode trigger(float tcnt) override FASTRUN;
        inline errorCode triggerDirect(uint32_t reload) override FASTRUN;
        inline errorCode getTriggerReload(float delay, uint32_t *reload) override;

        inline errorCode start() override;
        inline errorCode stop() override;

        inline uint16_t ticksFromMicros(float micros);

        // inline void setPeriod(uint32_t) {}

     protected:
        FTM_ChannelInfo *ci;
        FTM_r_t *regs;
        callback_t *pCallback = nullptr;

        template <typename period_t>
        inline errorCode doBegin(callback_t cb, period_t period, bool periodic);
    };

    // IMPLEMENTATION ==============================================

    FLASHMEM FTM_Channel::FTM_Channel(FTM_r_t *regs, FTM_ChannelInfo *channelInfo)
        : ITimerChannel(nullptr)
    {
        this->regs = regs;
        this->ci   = channelInfo;
    }

    template <typename period_t>
    FLASHMEM errorCode FTM_Channel::doBegin(callback_t callback, period_t period, bool periodic)
    {
        ci->isPeriodic = periodic;
        ci->reload     = ticksFromMicros(period);
        ci->callback   = callback;

        return errorCode::OK;
    }

    FLASHMEM errorCode FTM_Channel::start()
    {
        ci->chRegs->CV = regs->CNT + ci->reload;     // compare value (current counter + pReload)
        ci->chRegs->SC &= ~FTM_CSC_CHF;              // reset timer flag
        ci->chRegs->SC = FTM_CSC_MSA | FTM_CSC_CHIE; // enable interrupts
        return errorCode::OK;
    }

    FLASHMEM errorCode FTM_Channel::stop()
    {
        ci->chRegs->SC &= ~FTM_CSC_CHIE; // enable interrupts
        ci->chRegs->SC &= ~FTM_CSC_CHF;  // reset timer flag

        return errorCode::OK;
    }

    FLASHMEM errorCode FTM_Channel::getTriggerReload(float delay, uint32_t *reload)
    {
        *reload = ticksFromMicros(delay);
        return errorCode::OK;
    }

    FLASHMEM errorCode FTM_Channel::trigger(float delay)
    {
        return triggerDirect(ticksFromMicros(delay));
    }

    FLASHMEM errorCode FTM_Channel::triggerDirect(uint32_t reload)
    {
        uint32_t cv = regs->CNT + reload + 1;        // calc early to minimize error
        ci->chRegs->SC &= ~FTM_CSC_CHF;              // Reset timer flag
                                                     //
        regs->SC &= ~FTM_SC_CLKS_MASK;               // need to switch off clock to immediately set new CV
        ci->chRegs->CV = cv;                         // compare value (current counter + pReload)
        regs->SC |= FTM_SC_CLKS(0b01);               // restart clock
                                                     //
        ci->chRegs->SC = FTM_CSC_MSA | FTM_CSC_CHIE; // enable interrupts

        return errorCode::OK;
    }

    FLASHMEM float FTM_Channel::getMaxPeriod() const
    {
        return (0xFFFF * 1E-6f) / ci->ticksPerMicrosecond; // max period in seconds
    }

    FLASHMEM uint16_t FTM_Channel::ticksFromMicros(float micros)
    {
        uint32_t rl = ci->ticksPerMicrosecond * micros;
        if (rl > 0xFFFF)
        {
            postError(errorCode::periodOverflow); // warning only, continues with clipped value
            rl = 0xFFFF;
        }
        return rl;
    }

    FTM_Channel::~FTM_Channel()
    {
    }

} // namespace TeensyTimerTool
