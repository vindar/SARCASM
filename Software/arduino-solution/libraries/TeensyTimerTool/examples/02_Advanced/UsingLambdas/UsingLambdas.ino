
#include "TeensyTimerTool.h"
using namespace TeensyTimerTool;

OneShotTimer timer;

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);

    timer.begin([] { digitalWriteFast(LED_BUILTIN, LOW); });
}

void loop()
{
    digitalWriteFast(LED_BUILTIN, HIGH); // switch on LED
    timer.trigger(25'000);               // switch off after 25ms
    delay(1'000);                        // repeat every second
}
