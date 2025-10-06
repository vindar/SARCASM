#pragma once
#include "Arduino.h"
#include "ErrorHandling/error_codes.h"
#include "config.h"


#if not defined(PLAIN_VANILLA_CALLBACKS)
        #define USE_MODERN_CALLBACKS
        #include "inplace_function.h"
#endif
namespace TeensyTimerTool
{
    #if defined(USE_MODERN_CALLBACKS)
        using callback_t  = stdext::inplace_function<void(void)>;
        using errorFunc_t = stdext::inplace_function<void(errorCode)>;

        extern void attachErrFunc(errorFunc_t);
        extern errorCode postError(errorCode);
    #else
        using callback_t  = void (*)();
        using errorFunc_t = void (*)(errorCode);

        extern void attachErrFunc(errorFunc_t);
        extern errorCode postError(errorCode);
    #endif

    #if defined(TTT_TEENSY4X)
    #define TTT_F_CPU F_CPU_ACTUAL
    #else
    #define TTT_F_CPU F_CPU
    #endif
} // namespace TeensyTimerTool