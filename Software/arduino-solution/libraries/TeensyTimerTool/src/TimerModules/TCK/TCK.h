#pragma once

//#include "Arduino.h"
#include "TckChannel.h"
#include "core_pins.h"

namespace TeensyTimerTool
{
    extern const unsigned NR_OF_TCK_TIMERS;

    class TCK_t
    {
     public:
        template <typename counterType>
        static inline ITimerChannel *getTimer();
        static inline void removeTimer(TckChannelBase *);
        static inline void tick();

     protected:
        static bool isInitialized;
        static TckChannelBase *channels[NR_OF_TCK_TIMERS];
    };

    // IMPLEMENTATION ==================================================================

    template <typename counterType>
    FLASHMEM ITimerChannel *TCK_t::getTimer()
    {
        if (!isInitialized)
        {
            for (unsigned chNr = 0; chNr < NR_OF_TCK_TIMERS; chNr++)
            {
                channels[chNr] = nullptr;
            }
            isInitialized = true;

            // start the cycle counter if not already running
            if (ARM_DWT_CYCCNT == ARM_DWT_CYCCNT)
            {
                ARM_DEMCR |= ARM_DEMCR_TRCENA;
                ARM_DWT_CTRL |= ARM_DWT_CTRL_CYCCNTENA;
            }

// initialize the yield hook
#if defined(TEENSYDUINO) && YIELD_TYPE == YIELD_STANDARD
            extern void initYieldHook();
            initYieldHook();
#endif
        }
        for (unsigned chNr = 0; chNr < NR_OF_TCK_TIMERS; chNr++)
        {
            if (channels[chNr] == nullptr)
            {
                channels[chNr] = new TckChannel<counterType>();
                return channels[chNr];
            }
        }
        return nullptr;
    }

    FLASHMEM void TCK_t::removeTimer(TckChannelBase *channel)
    {
        for (unsigned chNr = 0; chNr < NR_OF_TCK_TIMERS; chNr++)
        {
            if (channels[chNr] == channel)
            {
                channels[chNr] = nullptr;
                break;
            }
        }
    }

    FLASHMEM void TCK_t::tick()
    {
        for (unsigned i = 0; i < NR_OF_TCK_TIMERS; i++)
        {
            if (channels[i] != nullptr)
            {
                channels[i]->tick();
            }
        }
    }
} // namespace TeensyTimerTool
