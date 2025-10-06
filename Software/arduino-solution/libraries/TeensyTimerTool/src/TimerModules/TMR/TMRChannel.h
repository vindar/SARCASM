#pragma once
#include "../../ITimerChannel.h"
//#include "Arduino.h"
#include "ErrorHandling/error_codes.h"
#include "config.h"
#include "imxrt.h"
#include <cmath>

namespace TeensyTimerTool
{
    class TMRChannel : public ITimerChannel
    {
     public:
        inline TMRChannel(IMXRT_TMR_CH_t *regs, callback_t *cbStorage);
        inline virtual ~TMRChannel();

        inline errorCode begin(callback_t cb, float tcnt, bool periodic) override;
        inline errorCode start() override;
        inline errorCode stop() override;

        inline errorCode trigger(float tcnt) override;
        inline float getMaxPeriod() const override;

        inline errorCode setPeriod(float us) override;
        inline errorCode setNextPeriod(float us) override;
        inline void setPrescaler(uint32_t psc); // psc 0..7 -> prescaler: 1..128

     protected:
        IMXRT_TMR_CH_t *regs;
        callback_t **pCallback = nullptr;
        float pscValue;
        uint32_t pscBits;

        inline errorCode us2Ticks(const float us, uint16_t *ticks) const;
        inline float_t counterToMicrosecond(const float_t cnt) const;
    };

    // IMPLEMENTATION ==============================================

    TMRChannel::TMRChannel(IMXRT_TMR_CH_t *regs, callback_t *cbStorage)
        : ITimerChannel(cbStorage)
    {
        this->regs = regs;
        setPrescaler(TMR_DEFAULT_PSC);
    }

    TMRChannel::~TMRChannel()
    {
        regs->CTRL = 0x0000; // stop timer and mark it free
    }

    FLASHMEM errorCode TMRChannel::start()
    {
        regs->CNTR = 0x0000;

        regs->CSCTRL &= ~TMR_CSCTRL_TCF1;
        regs->CSCTRL |= TMR_CSCTRL_TCF1EN;
        return errorCode::OK;
    }

    FLASHMEM errorCode TMRChannel::stop()
    {
        regs->CSCTRL &= ~TMR_CSCTRL_TCF1EN;
        return errorCode::OK;
    }

    FLASHMEM errorCode TMRChannel::begin(callback_t cb, float period, bool periodic)
    {
        uint16_t reload;
        errorCode status = us2Ticks(period, &reload);

        regs->CTRL   = 0x0000;
        regs->LOAD   = 0x0000;
        regs->COMP1  = reload;
        regs->CMPLD1 = reload;
        regs->CNTR   = 0x0000;
        regs->CSCTRL = TMR_CSCTRL_CL1(1);
        setCallback(cb);

        if (!periodic)
            regs->CTRL = TMR_CTRL_CM(1) | TMR_CTRL_PCS(pscBits) | TMR_CTRL_ONCE | TMR_CTRL_LENGTH;

        else
            regs->CTRL = TMR_CTRL_CM(1) | TMR_CTRL_PCS(pscBits) | TMR_CTRL_LENGTH;

        start();
        return status;
    }

    FLASHMEM errorCode TMRChannel::trigger(float us) // quick and dirty, should be optimized
    {
        // const float_t t = us2Ticks(tcnt);
        // uint16_t reload = t > 0xFFFF ? 0xFFFF : (uint16_t)t;

        uint16_t reload;
        errorCode status = us2Ticks(us, &reload);

        regs->CTRL   = 0x0000;
        regs->LOAD   = 0x0000;
        regs->COMP1  = reload;
        regs->CMPLD1 = reload;
        regs->CNTR   = 0x0000;

        regs->CSCTRL &= ~TMR_CSCTRL_TCF1;
        regs->CSCTRL |= TMR_CSCTRL_TCF1EN;

        regs->CTRL = TMR_CTRL_CM(1) | TMR_CTRL_PCS(pscBits) | TMR_CTRL_ONCE | TMR_CTRL_LENGTH;

        return status;
    }

    FLASHMEM void TMRChannel::setPrescaler(uint32_t psc) // psc 0..7 -> prescaler: 1..128
    {
        pscValue = 1 << (psc & 0b0111);
        pscBits  = 0b1000 | (psc & 0b0111);
    }

    FLASHMEM float TMRChannel::getMaxPeriod() const
    {
        return pscValue / 150'000'000.0f * 0xFFFE;
    }

    FLASHMEM errorCode TMRChannel::setNextPeriod(float us)
    {
        uint16_t reload;
        errorCode status = us2Ticks(us, &reload);
        regs->CMPLD1     = reload;
        return status;
    }

    // errorCode TMRChannel::_setCurrentPeriod(const uint16_t cnt)
    // {

    //     regs->COMP1 = cnt;

    //     //Do we need to wait some cycle for IP bus to update here / cache flush?
    //     //asm volatile("dsb");

    //     if (regs->CNTR > cnt)
    //     {
    //         //if counter alrready went over setted value force a triggering
    //         regs->CNTR = cnt;
    //         return errorCode::triggeredLate;
    //     }

    //     else
    //     {
    //         return errorCode::OK;
    //     }
    // }

    FLASHMEM errorCode TMRChannel::setPeriod(float us)
    {
        uint16_t newReload;
        errorCode status = us2Ticks(us, &newReload);

        regs->CMPLD1 = newReload;     // counter will load this value to COMP1 at next trigger
                                      //
        noInterrupts();               // interrupting this code could lead to wrong cntr settings
        if (regs->CNTR > newReload)   // already too late for new period
            regs->CNTR = regs->COMP1; //   -> force trigger;  will also load COMP1 with value from CMPLD1
        else                          // not too late
            regs->COMP1 = newReload;  //   -> change current compare value to new one (counter _might_ be > newReload in between.. watch and fix if necessary)
        interrupts();

        return status;
    }

    FLASHMEM errorCode TMRChannel::us2Ticks(const float us, uint16_t *ticks) const
    {
        constexpr uint16_t maxTicks = 0xFFFE;

        float tmpTicks = us * 150.0f / pscValue;
        if (tmpTicks > maxTicks)
        {
            *ticks = maxTicks;
            return errorCode::periodOverflow;
        }
        *ticks = (uint16_t)tmpTicks;
        return errorCode::OK;
    }

    FLASHMEM float_t TMRChannel::counterToMicrosecond(const float_t cnt) const
    {
        return cnt * pscValue / 150.0f;
    }

} // namespace TeensyTimerTool
