#include <TeensyTimerTool.h>

void callback()
    {
    digitalWriteFast(LED_BUILTIN, !digitalReadFast(LED_BUILTIN));
    }

TeensyTimerTool::PeriodicTimer t1(TeensyTimerTool::TMR4);

void setup()
    {
    Serial.begin(9600);
    pinMode(LED_BUILTIN,OUTPUT);

    t1.begin(callback, 10000, false); // 250ms    
    t1.setPrescaler(5);
    t1.setPeriod(10000);
    t1.start();
    Serial.println(t1.getMaxPeriod()*1000000);
    }

void loop()
  {
  }