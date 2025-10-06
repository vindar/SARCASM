// Uses the built in Real Time Clock (RTC) as timebase.
// The sketch calculates the relative drift of the main 24MHz crystal relative to the 32.768kHz RTC crystal.
// Requires a T4.x board

#include "Arduino.h"
#include "TeensyTimerTool.h"
using namespace TeensyTimerTool;

PeriodicTimer t1(TCK_RTC);

constexpr uint32_t period = 500'000; //µs

void callback()
{
    static uint32_t start = micros();
    static uint32_t idx   = 0;

    uint32_t now          = micros() - start;
    uint32_t expected     = idx++ * period;
    int32_t delta         = now - expected;
    float drift           = 1E6 * delta / expected;  // ppm

    Serial.printf("t: %d µs,  rel. drift: %.2f ppm\n", now, drift);
}

void setup()
{
    t1.begin(callback, period);
}

void loop()
{
}
