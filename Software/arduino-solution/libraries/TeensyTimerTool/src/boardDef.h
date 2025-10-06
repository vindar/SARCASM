#pragma once
#include <cstdint>

namespace TeensyTimerTool
{
    class ITimerChannel;
    using TimerGenerator = ITimerChannel*(); //returns a pointer to a free timer channel or nullptr

    // TEENSYDUINO  ==========================================================================
    #if defined(TEENSYDUINO)
        #if defined(ARDUINO_TEENSY40) || defined(ARDUINO_TEENSY41) || defined(ARDUINO_TEENSY_MICROMOD)
            #define TTT_TEENSY4X
        #elif defined(ARDUINO_TEENSY30) || defined(ARDUINO_TEENSY31) || defined(ARDUINO_TEENSY32) || defined(ARDUINO_TEENSY35) || defined(ARDUINO_TEENSY36)
            #define TTT_TEENSY3X
        #endif

        #if defined(ARDUINO_TEENSYLC)
            extern TimerGenerator *const TCK;

        #elif defined(ARDUINO_TEENSY30)
            extern TimerGenerator *const FTM0, * const FTM1;
            extern TimerGenerator *const TCK, * const TCK32, * const TCK64;

        #elif defined(ARDUINO_TEENSY31) || defined(ARDUINO_TEENSY32)
            extern TimerGenerator *const FTM0, * const FTM1, * const FTM2;
            extern TimerGenerator *const TCK, * const TCK32, * const TCK64;


        #elif defined(ARDUINO_TEENSY35)
            extern TimerGenerator *const FTM0, * const FTM1, * const FTM2, * const FTM3, * const FTM4;
            extern TimerGenerator *const TCK, * const TCK32, * const TCK64;

        #elif defined(ARDUINO_TEENSY36)
            extern TimerGenerator *const FTM0, *const FTM1, *const FTM2, *const FTM3, *const FTM4;
            extern TimerGenerator *const TCK, * const TCK32, * const TCK64;

        #elif defined(TTT_TEENSY4X)
            extern TimerGenerator *const TMR1, *const TMR2, *const TMR3, *const TMR4;
            extern TimerGenerator *const GPT1, *const GPT2;
            extern TimerGenerator *const PIT;
            extern TimerGenerator *const TCK, * const TCK32, * const TCK64, *const TCK_RTC;
            extern TimerGenerator *const RTC;

        #else
            #error BOARD NOT SUPPORTED
        #endif

        #define YIELD_NONE      0
        #define YIELD_STANDARD  1
        #define YIELD_OPTIMIZED 2

        constexpr int PSC_AUTO = -1;
        constexpr int PSC_1 = 0;
        constexpr int PSC_2 = 1;
        constexpr int PSC_4 = 2;
        constexpr int PSC_8 = 3;
        constexpr int PSC_16 = 4;
        constexpr int PSC_32 = 5;
        constexpr int PSC_64 = 6;
        constexpr int PSC_128 = 7;

        extern void(* const tick)();


    #else
    #    error "Board not supported"
    #endif
}