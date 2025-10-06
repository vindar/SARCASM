#pragma once

#include "core_pins.h"
#include <cstdint>

struct GPIO_Info_t
{
    inline GPIO_Info_t(unsigned pin);
    const uintptr_t pinCfg;
    const unsigned gpioPortNr;
    const unsigned gpioBitNr;

    char name[20];
};

//======================================================================================
#if defined(KINETISK) || defined(KINETISL)

GPIO_Info_t::GPIO_Info_t(unsigned pin)
    : pinCfg((uintptr_t)digital_pin_to_info_PGM[pin].config), // address of pin config register
      gpioPortNr((pinCfg - (uintptr_t)&PORTA_PCR0) / 0x1000), // cfg base addresses are 4kB aligned staring with PORTA_PCR0
      gpioBitNr((pinCfg & 0xFFF) / 4)                         // each bit has to 4 consecutive 32bit cfg registers
{
  snprintf(name, 20, "GPIO%d_%02d", gpioPortNr, gpioBitNr);
}

//======================================================================================
#elif defined(ARDUINO_TEENSY40) || defined(ARDUINO_TEENSY41) || defined(ARDUINO_TEENSY_MICROMOD)

 
GPIO_Info_t::GPIO_Info_t(unsigned pin)
    : pinCfg((uintptr_t)digital_pin_to_info_PGM[pin].reg),            // address of pin config register
      gpioPortNr((pinCfg - (uintptr_t)&IMXRT_GPIO6) / 0x4000 + 6),    // cfg base addresses are 4kB aligned staring with PORTA_PCR0
      gpioBitNr(__builtin_ffs(digital_pin_to_info_PGM[pin].mask) - 1) // each bit has to 4 consecutive 32bit cfg registers
{
        snprintf(name, 20, "GPIO%d_%02d", gpioPortNr, gpioBitNr);
}

#endif