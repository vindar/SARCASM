#include "TeensyTimerTool.h"
using namespace TeensyTimerTool;



//Timer t[]{FTM0, FTM0, FTM2};             // two channels from FTM0, one from FTM2
//Timer t[3];                              // use 3 timers from the pool
OneShotTimer t[]{TCK, TCK, TCK, TCK, TCK}; // 5 software timers

constexpr unsigned nrOfTimers = sizeof(t) / sizeof(t[0]);

void setup()
{
    // setup timers, use lambdas as callbacks
    for (unsigned i = 0; i < nrOfTimers; i++)
    {
        pinMode(i, OUTPUT);
        t[i].begin([i] { digitalWriteFast(i, LOW); }); // callback resets pin to LOW
    }
}


void loop()
{
    for (unsigned i = 0; i < nrOfTimers; i++)
    {
        digitalWriteFast(i, HIGH);
        t[i].trigger(50 * (i + 1)); // 50, 100, 150 ... µs
    }
    delay(1);
}
