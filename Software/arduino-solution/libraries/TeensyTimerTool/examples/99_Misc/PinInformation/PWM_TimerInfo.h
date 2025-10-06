#pragma once

#include "core_pins.h"
#include <cstdio>

struct PWM_TimerInfo_t
{
    inline PWM_TimerInfo_t(unsigned _pin = 0);
    unsigned type;
    unsigned module;
    char name[20];
};

//===========================================================================================================
#if defined(ARDUINO_TEENSY40) || defined(ARDUINO_TEENSY41) || defined(ARDUINO_TEENSY_MICROMOD)

// holds core info about used pwm timers. Struct is defined in pwm.c.
// There is no header declaring it. So, we need to do this here:
struct pwm_pin_info_struct
{
    uint8_t type;    // 0=no pwm, 1=flexpwm, 2=quad
    uint8_t module;  // 0-3, 0-3
    uint8_t channel; // 0=X, 1=A, 2=B
    uint8_t muxval;  //
};
extern pwm_pin_info_struct pwm_pin_info[]; // This array holds the pwm timer info

PWM_TimerInfo_t::PWM_TimerInfo_t(unsigned pin)
{
    constexpr char timerNames[][9] = {"FLEX_PWM", "QUAD"};

    type = pwm_pin_info[pin].type;
    module = (pwm_pin_info[pin].module >> 4) + 1;

    if (type != 0)
        snprintf(name, 20, "%s%d", timerNames[type - 1], module);
    else
        snprintf(name, 20, "no pwm");
}

//===========================================================================================================
#elif defined(ARDUINO_TEENSYLC) ||                            \
    defined(ARDUINO_TEENSY30) || defined(ARDUINO_TEENSY32) || \
    defined(ARDUINO_TEENSY35) || defined(ARDUINO_TEENSY36)

extern "C" const int PwmTimerModule[CORE_NUM_DIGITAL];
extern "C" const int PwmTimerType[CORE_NUM_DIGITAL];

PWM_TimerInfo_t::PWM_TimerInfo_t(unsigned pin)
{
    constexpr char timerNames[][9] = {"FTM", "TPM"};

    type = PwmTimerType[pin];
    module = PwmTimerModule[pin];

    if (type != 0)
        snprintf(name, 20, "%s%d", timerNames[type - 1], module);
    else
        snprintf(name, 20, "none");
}

#endif