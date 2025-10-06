#pragma once

#include <cstdint>

namespace TeensyTimerTool
{
    class CycleCounterBase
    {
     protected:
        CycleCounterBase();
    };

    //---------------------------------------------------------------------
    // Software counter based on the 32bit ARM cycle counter
    // Resolution 1/FCPU_ACTUAL (1.66ns @600MHz)
    //
    struct CycleCounter32 : CycleCounterBase
    {
        using counter_t = uint32_t;

        static counter_t getCount();
        static uint32_t us2ticks(float us);
        static float getMaxMicros();
    };

    //---------------------------------------------------------------------
    // Software counter based on a 64bit extension of
    // the ARM cycle counter
    // Resolution 1/FCPU_ACTUAL (6.66ns @600MHz)
    //
    struct CycleCounter64 : CycleCounterBase
    {
        using counter_t = uint64_t;

        static uint64_t getCount();
        static uint64_t us2ticks(float us);
        static float getMaxMicros();
    };

    //------------------------------------------------------------------
    // Software counter based on the 64bit period counter
    // of the built in real time clock (RTC).
    // Resolution: 1/32768s (30.5 µs)
    //
    struct RtcCounter
    {
        using counter_t = uint64_t;

        static counter_t getCount();
        static counter_t us2ticks(float us);
        static float getMaxMicros();
    };

    //------------------------------------------------------------------
    // Fallback counter for boards without cycle counter
    // E.g. Teensy LC
    // Resolution: 1 µs
    //
    struct MicrosCounter
    {
        using counter_t = uint32_t;

        static counter_t getCount();
        static counter_t us2ticks(float us);
        static float getMaxMicros();
    };

} // namespace TeensyTimerTool