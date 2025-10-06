#pragma once

#include "GPIO_Info.h"
#include "PWM_TimerInfo.h"

struct PinInfo
{
    inline PinInfo(unsigned _pin)
        : pin(_pin),
          gpioInfo(pin),
          pwmTimerInfo(pin)
    {}

    const unsigned pin;
    const GPIO_Info_t gpioInfo;
    const PWM_TimerInfo_t pwmTimerInfo;
};
