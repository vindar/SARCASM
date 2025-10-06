#pragma once

#include "Arduino.h"
#include "TeensyTimerTool.h"

class PulseGenerator
{
 public:
    PulseGenerator();                             // constructor, initializes non hardware related stuff
    void begin(unsigned pin);                     // initializes hardware related stuff
    void schedulePulse(float delay, float width); // schedules a 'width µs' wide pulse after a waiting time of 'delay µs'. Non blocking.

 protected:
    TeensyTimerTool::OneShotTimer pulseTimer;     // used for generating the pulses
    void callback();

    uint32_t pin;
    float width;
};



void PulseGenerator::begin(unsigned pin)
{
    this->pin = pin;
    pinMode(pin, OUTPUT);
    digitalWriteFast(pin, LOW);

    pulseTimer.begin([this] { this->callback(); });
}

void PulseGenerator::schedulePulse(float delay, float _width)
{
    width = _width;                             // the timer callback needs to know the pulse width to generate the pulse

    if (delay != 0)
    {
        pulseTimer.trigger(delay);              // this triggers the oneShot timer to fire after 'delay µs'
    }
    else                                        //delay == 0, directly generate the pulse
    {
        digitalWriteFast(pin, HIGH);
        pulseTimer.trigger(width);              // this triggers the oneShotTimer to fire after 'width µs'
    }
}

void PulseGenerator::callback()
{
    if (digitalReadFast(pin) == LOW)
    {
        digitalWriteFast(pin, HIGH);
        pulseTimer.trigger(width);              // re-trigger
    } else
    {
        digitalWriteFast(pin, LOW);
    }
}

PulseGenerator::PulseGenerator()
    : pulseTimer(TeensyTimerTool::FTM0)         // use one of the 8 FTM0 channels
{}