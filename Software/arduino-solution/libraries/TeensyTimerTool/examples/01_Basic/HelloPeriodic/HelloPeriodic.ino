/********************************************************
 * Basic usage of the timer
 *
 * Generates a timer from the timer pool and
 * starts it with a period of 250ms.
 * The timer callback simply toggles the built in LED
 *
 ********************************************************/

#include "TeensyTimerTool.h"

using namespace TeensyTimerTool;

void callback()
{
    digitalWriteFast(LED_BUILTIN, !digitalReadFast(LED_BUILTIN));
}

PeriodicTimer t1; // generate a timer from the pool (Pool: 2xGPT, 16xTMR(QUAD), 20xTCK)

void setup()
{
    pinMode(LED_BUILTIN,OUTPUT);
    t1.begin(callback, 250'000); // 250ms
}

void loop()
{
}
