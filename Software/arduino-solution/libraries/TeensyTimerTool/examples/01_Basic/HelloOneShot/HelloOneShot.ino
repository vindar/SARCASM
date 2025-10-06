#include "Arduino.h"
#include "TeensyTimerTool.h"

using namespace TeensyTimerTool;

void callback()
{
    digitalWriteFast(LED_BUILTIN, LOW);  // switch off LED
}


OneShotTimer timer1; // generate a timer from the pool (Pool: 2xGPT, 16xTMR(QUAD), 20xTCK)

void setup()
{
    pinMode(LED_BUILTIN,OUTPUT);
    timer1.begin(callback);
}

void loop()
{
    digitalWriteFast(LED_BUILTIN, HIGH);
    timer1.trigger(10'000); // switch of after 10ms

    delay(500);
}
