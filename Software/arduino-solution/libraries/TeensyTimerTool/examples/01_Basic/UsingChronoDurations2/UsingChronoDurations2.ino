#include "TeensyTimerTool.h"
using namespace TeensyTimerTool;

OneShotTimer timer[]{TCK, TCK, TCK, TCK}; // 4 one-shot-timers
PeriodicTimer pt1;                        // 1 periodic timer
unsigned t_0;                             // start time

void isr()
{
    Serial.printf("called @: %u ms\n", millis() - t_0);
}

void setup()
{
    while (!Serial) {} // wait for PC to connect the virtual serial port

    for (OneShotTimer& t : timer) // for the sake of simplicity, attach the same isr to all timers in array
    {
        t.begin(isr);
    }

    timer[0].trigger(10ms);                                  // 10 ms
    timer[1].trigger(0.5s + 10ms);                           // 510 ms
    timer[2].trigger(2.5 * 0.3s + 20'000us / 2);             // 760 ms
    timer[3].trigger(milliseconds(50) + microseconds(5000)); // 55ms
    t_0 = millis();

    pt1.begin([] { digitalToggleFast(LED_BUILTIN); }, 0.5s);
}

void loop()
{
}
