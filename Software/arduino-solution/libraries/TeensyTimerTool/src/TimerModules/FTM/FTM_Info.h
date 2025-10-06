#pragma once

#include "boardDef.h"
#include <algorithm>

// All information in this header is calculated at compile time.
// FTM_Info will not generate any code

namespace TeensyTimerTool
{
    typedef struct // FTM & TPM Channels
    {
        volatile uint32_t SC;
        volatile uint32_t CV;
    } FTM_CH_t;

    typedef struct // FTM register block (this layout is compatible to a TPM register block)
    {
        volatile uint32_t SC;
        volatile uint32_t CNT;
        volatile uint32_t MOD;
        FTM_CH_t CH[8];
        volatile uint32_t CNTIN;
        volatile uint32_t STATUS;
        volatile uint32_t MODE;
        volatile uint32_t SYNC;
        volatile uint32_t OUTINIT;
        volatile uint32_t OUTMASK;
        volatile uint32_t COMBINE;
        volatile uint32_t DEADTIME;
        volatile uint32_t EXTTRIG;
        volatile uint32_t POL;
        volatile uint32_t FMS;
        volatile uint32_t FILTER;
        volatile uint32_t FLTCTRL;
        volatile uint32_t QDCTRL;
        volatile uint32_t CONF;
        volatile uint32_t FLTPOL;
        volatile uint32_t SYNCONF;
        volatile uint32_t INVCTRL;
        volatile uint32_t SWOCTRL;
        volatile uint32_t PWMLOAD;
    } FTM_r_t;

    //=======================================================================
    // using a static class instead of namespace here to reduce namespace pollution
    // (anonymous namespace doesn't make sense for header only class)

    template <unsigned module>
    class FTM_Info
    {
     private:
#if defined(ARDUINO_TEENSYLC)
        static constexpr unsigned boardNr = 0;
#elif defined(ARDUINO_TEENSY30)
        static constexpr unsigned boardNr = 1;
#elif defined(ARDUINO_TEENSY31) || defined(ARDUINO_TEENSY32)
        static constexpr unsigned boardNr = 2;
#elif defined(ARDUINO_TEENSY35)
        static constexpr unsigned boardNr = 3;
#elif defined(ARDUINO_TEENSY36)
        static constexpr unsigned boardNr = 4;
#else
#error Board not valid
#endif

        static constexpr int IRQ_Numbers[][4]{
            //FTM0  FTM1  FTM2  FTM3
            {0, 0, 0, 0},     // Teensy LC
            {25, 26, 0, 0},   // Teensy 3.0
            {62, 63, 64, 0},  // Teensy 3.1/3.2
            {42, 43, 44, 71}, // Teensy 3.5
            {42, 43, 44, 71}, // Teensy 3.6
        };

        static constexpr int FTM_NrOfChannels[]{
            8, // FTM0
            2, // FTM1
            2, // FTM2
            8, // FTM3
        };

        static constexpr uintptr_t FTM_BaseAdr[] // can't use defines from kinetis.h since, depending on board, not all are always defined.
            {
                0x4003'8000, // FTM0
                0x4003'9000, // FTM1
                0x400B'8000, // FTM2
                0x400B'9000, // FTM3
            };

        static constexpr unsigned FTM_Prescale =
            FTM_DEFAULT_PSC[module] < 0 || FTM_DEFAULT_PSC[module] > 7 ? // prescale value to roughly get 2 ticks per µs
                (
                    F_BUS > 120'000'000 ? 0b111 : F_BUS > 60'000'000 ? 0b110
                                              : F_BUS > 30'000'000   ? 0b101
                                              : F_BUS > 15'000'000   ? 0b100
                                              : F_BUS > 8'000'000    ? 0b011
                                              : F_BUS > 4'000'000    ? 0b010
                                              : F_BUS > 2'000'000    ? 0b001
                                                                     : 0b000)
                                                                       : FTM_DEFAULT_PSC[module];

     public:
        static constexpr uintptr_t baseAdr      = FTM_BaseAdr[module];
        static constexpr IRQ_NUMBER_t irqNumber = (IRQ_NUMBER_t)IRQ_Numbers[boardNr][module];
        static constexpr unsigned nrOfChannels  = FTM_NrOfChannels[module];
        static constexpr unsigned prescale      = FTM_Prescale;
    };
} // namespace TeensyTimerTool