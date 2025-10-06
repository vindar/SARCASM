#pragma once
#include "PulseGenerator.h"
#include "TeensyTimerTool.h"

class LaserController
{
 public:
    void begin(unsigned preTriggerPin, unsigned triggerPin, unsigned camPin);
    void shoot();

 protected:
    PulseGenerator preTrigger, trigger, camera;
};

void LaserController::begin(unsigned preTriggerPin, unsigned triggerPin, unsigned camPin)
{
    preTrigger.begin(preTriggerPin);
    trigger.begin(triggerPin);
    camera.begin(camPin);
}

void LaserController::shoot()
{
    constexpr float t_warmup = 140 - 5.5;
    constexpr float t_p = 10 - 3;
    constexpr float t_camDelay = 130 - 7.5;
    constexpr float t_int = 30 - 3;

    preTrigger.schedulePulse(0, t_p);        // immediately generate the pretrigger pulse
    trigger.schedulePulse(t_warmup, t_p);    // schedule the trigger pulse to start after the warmup time
    camera.schedulePulse(t_camDelay, t_int); // schedule the cam pulse after the camDelay time
}
