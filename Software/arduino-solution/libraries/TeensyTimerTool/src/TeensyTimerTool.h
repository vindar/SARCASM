#pragma once

#include "API/oneShotTimer.h"
#include "API/periodicTimer.h"
#include "API/timer.h"
#include "ErrorHandling/error_handler.h"
#include "config.h"

static_assert(TEENSYDUINO >= 150, "This library requires Teensyduino > 1.5");