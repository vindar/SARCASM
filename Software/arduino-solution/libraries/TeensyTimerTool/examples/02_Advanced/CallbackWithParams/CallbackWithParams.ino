#include "TeensyTimerTool.h"

using namespace TeensyTimerTool;

void callback(int& someInt, int cnt)  // this callback has context, i.e. parameter
{
    for (int i = 0; i < cnt; i++)     // when called, print out someInt cnt times
    {
        Serial.print(someInt);
        Serial.print(" | ");
    }
    Serial.println();
}

//==============================================================

PeriodicTimer t;
int number = 0;

void setup()
{
    t.begin([] { callback(number, 5); }, 50ms);
}

void loop()
{
    number++;     // change every second
    delay(1000);
}