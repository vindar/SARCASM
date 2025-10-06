#pragma once
#include "Arduino.h"
#include "GPTmap.h"
#include "ITimerChannel.h"
#include "core_pins.h"

namespace TeensyTimerTool
{
    class GptChannel : public ITimerChannel
    {
     public:
        inline GptChannel(IMXRT_GPT_t *, callback_t *);
        virtual ~GptChannel();

        inline errorCode begin(callback_t cb, float tcnt, bool periodic) override;
        inline errorCode start() override;
        inline errorCode stop() override;

        inline errorCode trigger(float delay) override;
        inline errorCode triggerDirect(uint32_t delay) override;
        inline errorCode getTriggerReload(float delay, uint32_t *reload) override;

        inline errorCode setNextPeriod(float us) override;
        inline errorCode setPeriod(float us) override;

        inline float getMaxPeriod() const override { return getMaxMicros() / 1E6; }

        bool isPeriodic;

     protected:
        inline uint32_t us2ticks(float micros) const;
        inline float getMaxMicros() const;

        IMXRT_GPT_t *regs;        
        float clock;
    };

    // IMPLEMENTATION ==============================================

    GptChannel::GptChannel(IMXRT_GPT_t *registers, callback_t *cbStorage)
        : ITimerChannel(cbStorage), regs(registers)
    {
    }

    FLASHMEM errorCode GptChannel::begin(callback_t cb, float period, bool periodic)
    {
        clock      = (CCM_CSCMR1 & CCM_CSCMR1_PERCLK_CLK_SEL) ? 24 : (F_BUS_ACTUAL / 1000000);
        isPeriodic = periodic;
        if (isPeriodic)
        {
            setPeriod(period);
        }
        setCallback(cb);

        return errorCode::OK;
    }

    FLASHMEM errorCode GptChannel::start()
    {
        regs->SR = 0x3F;         // clear all interrupt flags
        regs->IR = GPT_IR_OF1IE; // enable OF1 interrupt
        regs->CR |= GPT_CR_EN;   // enable timer
        return errorCode::OK;
    }

    FLASHMEM errorCode GptChannel::stop()
    {
        regs->CR &= ~GPT_CR_EN; // disable timer
        regs->IR = 0;
        return errorCode::OK;
    }

    FLASHMEM errorCode GptChannel::trigger(float delay) // should be optimized somehow
    {
        return triggerDirect(us2ticks(delay));
    }

    FLASHMEM errorCode GptChannel::triggerDirect(uint32_t reload)
    {
        regs->SR   = 0x3F;         // clear all interrupt flags
        regs->IR   = GPT_IR_OF1IE; // enable OF1 interrupt
        regs->OCR1 = reload;       // set overflow value
        regs->CR |= GPT_CR_EN;     // enable timer

        return errorCode::OK;
    }

    FLASHMEM errorCode GptChannel::getTriggerReload(float delay, uint32_t *reload)
    {
        *reload = us2ticks(delay);
        return errorCode::OK;
    }

    FLASHMEM uint32_t GptChannel::us2ticks(float micros) const
    {
        if (micros > getMaxMicros())
        {
            micros = getMaxPeriod();
            postError(errorCode::periodOverflow);
        }
        //Serial.printf("gpt p:%f %d\n", clock, (uint32_t)(clock * micros) - 1);
        return (uint32_t)(clock * micros) - 1;
    }

    FLASHMEM float GptChannel::getMaxMicros() const
    {
        return (float)0xFFFF'FFFE / clock;
    }

    FLASHMEM errorCode GptChannel::setNextPeriod(float us)
    {
        return errorCode::notImplemented;
    }

    FLASHMEM errorCode GptChannel::setPeriod(float us) // not good, will generate one too long period if called before cnt == oldPeriod
    {                                         // need to redo the timing using free running timer to get setPeriod and setNewPeriod working correctly
        regs->OCR1 = us2ticks(us);        
        return errorCode::OK;
    }

} // namespace TeensyTimerTool