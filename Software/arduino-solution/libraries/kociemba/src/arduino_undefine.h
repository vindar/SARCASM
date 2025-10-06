#pragma once

// On arduino, we include the standard Arduino header
#if defined(TEENSYDUINO) || defined(ESP32)
#include "Arduino.h"
#else
#include <chrono>
#include <string.h>
#include <stdlib.h>
#endif

#ifndef PROGMEM
#define PROGMEM
#endif

#ifndef FLASHMEM
#define FLASHMEM
#endif

/** make sure these below are not defined */

#undef B1
#undef B2
#undef B3
#undef B4
#undef B5
#undef B6
#undef B7
#undef B8
#undef B9
#undef BR
