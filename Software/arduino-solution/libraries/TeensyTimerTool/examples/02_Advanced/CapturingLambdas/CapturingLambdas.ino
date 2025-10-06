
#include "TeensyTimerTool.h"

using namespace TeensyTimerTool;

// Array of 10 tick timers
PeriodicTimer timer[]{
    TCK, TCK, TCK, TCK, TCK,
    TCK, TCK, TCK, TCK, TCK};

void onTimer(int nr){
    Serial.printf("Timer #%d at %d\n", nr, millis());
}

void setup(){
    // start all 10 timers with random period. Pass timer number to callback
    for (int i = 0; i < 10; i++)
    {
        timer[i].begin([i] { onTimer(i); }, random(100'000, 900'000));
    }
}

void loop(){
}
