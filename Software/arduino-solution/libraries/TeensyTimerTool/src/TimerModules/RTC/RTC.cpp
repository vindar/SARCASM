#include "../../boardDef.h"
#if defined(TTT_TEENSY4X)
#include "RTC.h"
#include "RtcChannel.h"

namespace TeensyTimerTool::RTC_t
{
    namespace // private
    {
        callback_t callback = nullptr;
        RtcChannel channel(&callback);
        bool initialized = false;

        void isr()
        {
            SNVS_HPSR |= 0b11; // reset interrupt flag
            if (callback != nullptr) callback();
            asm volatile("DSB");
        }
    } // namespace

    ITimerChannel *getTimer()
    {
        if (!initialized)
        {
            attachInterruptVector(IRQ_SNVS_IRQ, isr);
            NVIC_ENABLE_IRQ(IRQ_SNVS_IRQ);
            initialized = true;
        }
        return &channel;
    }
} // namespace TeensyTimerTool::RTC_t
#endif