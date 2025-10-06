#if defined(ARDUINO_TEENSY40) || defined(ARDUINO_TEENSY41) || defined(ARDUINO_TEENSY_MICROMOD)

#include "PIT.h"

namespace TeensyTimerTool
{
    bool PIT_t::isInitialized    = false;
    PITChannel PIT_t::channel[4] = {{0}, {1}, {2}, {3}};
} // namespace TeensyTimerTool

#endif