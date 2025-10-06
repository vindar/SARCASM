// Uses the built in Real Time Clock (RTC) to generate periodic interrupts
// The RTC can generate periodic interrupts with periods of: 1s, 0.5s, 0.25s, 0.125s .... 30.51µs.
// The interrupts are syncronized to the RTC. I.e., the 1s interrupt fires exactly at the 1s clock transition
// If other periods are requested, the library uses the the next longer period. I.e. if
// you do a setPeriod(0.4s) you get a period of 0.5s.
//
// Requires a T4.x board

#include "Arduino.h"
#include "TeensyTimerTool.h"

using namespace TeensyTimerTool;

PeriodicTimer myRTCtimer(RTC);

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    while (!Serial) {}

    myRTCtimer.begin([] { digitalToggleFast(LED_BUILTIN); }, 25ms);  // blink with 20Hz for 1.5s
    delay(1500);

    myRTCtimer.stop(); // stop for 2s
    delay(2000);

    myRTCtimer.setPeriod(0.5s); // blink with 1Hz for 5s
    delay(5000);

    myRTCtimer.setPeriod(50ms); // blink with 10Hz
}

void loop()
{
}
