#include "error_handler.h"
#include "core_pins.h"
#include "types.h"

namespace TeensyTimerTool
{
    FLASHMEM ErrorHandler::ErrorHandler(Stream &s) : stream(s)
    {
        pinMode(LED_BUILTIN, OUTPUT);
    }

    FLASHMEM void ErrorHandler::operator()(errorCode code) const
    {
        const char *txt;

        switch (code)
        {
            case errorCode::OK:
                txt = "OK";
                break;

            // warnings
            case errorCode::periodOverflow:
                txt = "Period overflow. Period was set to maximum value";
                break;
            case errorCode::wrongType:
                txt = "Wrong parameter type";
                break;

            // general errors
            case errorCode::reload:
                txt = "Period must not be zero";
                break;
            case errorCode::noFreeChannel:
                txt = "Timer module has no free channel";
                break;
            case errorCode::noFreeModule:
                txt = "Timer pool contains no free timer";
                break;
            case errorCode::notImplemented:
                txt = "Function not implemented for this timer";
                break;
            case errorCode::notInitialized:
                txt = "Timer not initialized or available. Did you call begin?";
                break;

            default:
                txt = "Unknown error";
                break;
        }

        if ((int)code < 0) // in case of warnings we return after printing
        {
            stream.printf("W-%i: %s\n", -(int)code, txt);
            return;
        }

        stream.printf("E-%i: %s\n", (int)code, txt); // in case of errors we don't return
        while (true)
        {
            digitalWriteFast(LED_BUILTIN, !digitalReadFast(LED_BUILTIN));
            delay(50);
        }
    }

    errorFunc_t errFunc;

    FLASHMEM errorCode postError(errorCode e)
    {
        if (errFunc != nullptr && e != errorCode::OK) errFunc(e);
        return e;
    }

    FLASHMEM void attachErrFunc(errorFunc_t _errFunc)
    {
        errFunc = _errFunc;
    }

} // namespace TeensyTimerTool