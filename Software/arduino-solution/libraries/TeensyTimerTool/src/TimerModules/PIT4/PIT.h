#pragma once

#include "PITChannel.h"

namespace TeensyTimerTool
{
    class PIT_t
    {
     public:
        inline static ITimerChannel *getTimer();

     protected:
        static bool isInitialized;
        static void isr();
        static PITChannel channel[4];
    };

    // IMPLEMENTATION ===========================================================================

    ITimerChannel *PIT_t::getTimer()
    {
        if (!isInitialized)
        {
            isInitialized = true;

            CCM_CCGR1 |= CCM_CCGR1_PIT(CCM_CCGR_ON);
            PIT_MCR = 1;

            if (USE_GPT_PIT_150MHz)                       // timer clock setting from config.h
                CCM_CSCMR1 &= ~CCM_CSCMR1_PERCLK_CLK_SEL; // FBus (usually 150MHz)
            else
                CCM_CSCMR1 |= CCM_CSCMR1_PERCLK_CLK_SEL; // 24MHz

            attachInterruptVector(IRQ_PIT, isr);
            NVIC_ENABLE_IRQ(IRQ_PIT);
        }

        for (unsigned i = 0; i < 4; i++)
        {
            if (channel[i].callback == nullptr) return &channel[i];
        }
        return nullptr;
    }

    inline void PIT_t::isr()
    {
        if (IMXRT_PIT_CHANNELS[0].TFLG)
        {
            IMXRT_PIT_CHANNELS[0].TFLG = 1;
            channel[0].isr();
        }
        if (IMXRT_PIT_CHANNELS[1].TFLG)
        {
            IMXRT_PIT_CHANNELS[1].TFLG = 1;
            channel[1].isr();
        }
        if (IMXRT_PIT_CHANNELS[2].TFLG)
        {
            IMXRT_PIT_CHANNELS[2].TFLG = 1;
            channel[2].isr();
        }
        if (IMXRT_PIT_CHANNELS[3].TFLG)
        {
            IMXRT_PIT_CHANNELS[3].TFLG = 1;
            channel[3].isr();
        }

        asm volatile("dsb"); //wait until register changes propagated through the cache
    }
} // namespace TeensyTimerTool
