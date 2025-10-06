//-----------------------
// REQUIRES T4.x
//-----------------------

#include "TeensyTimerTool.h"

using namespace TeensyTimerTool;

PeriodicTimer t1(TCK);  // Tick-Timer does not use any hardware timer (20 32bit channels)
PeriodicTimer t2(TMR1); // First channel on TMR1 aka QUAD timer module. (TMR1 - TMR4, four 16bit channels each)
PeriodicTimer t3(GPT1); // GPT1 module (one 32bit channel per module)
OneShotTimer t4(TMR1);  // Second channel on TMR1

// Callbacks ===================================================================================

void pulse200ns()
{
    digitalWriteFast(1, HIGH);
    delayNanoseconds(200);
    digitalWriteFast(1, LOW);
}

void pulse400ns()
{
    digitalWriteFast(2, HIGH);
    delayNanoseconds(400);
    digitalWriteFast(2, LOW);
}

void LED_ON()
{
    digitalWriteFast(LED_BUILTIN, HIGH); // LED On
    t4.trigger(10'000);                  // trigger t4 to switch of after 10ms
}

void LED_OFF()
{
    digitalWriteFast(LED_BUILTIN, LOW);
}

//================================================================================================

void setup()
{
    for (unsigned pin = 0; pin <= 13; pin++) pinMode(pin, OUTPUT);

    t1.begin(pulse200ns, 50'000); // 200ns pulse every 500 ms
    t2.begin(pulse400ns, 100);    // 400ns pulse every 100 µs
    t3.begin(LED_ON, 1'000'000);  // Switch LED on every second
    t4.begin(LED_OFF);            // One shot timer to switch LED Off
}

void loop()
{
}
