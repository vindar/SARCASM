#if defined(ARDUINO_TEENSY40) || defined(ARDUINO_TEENSY41) || defined(ARDUINO_TEENSY_MICROMOD)

#include "GPTChannel.h"
#include "GPT.h"
//#include "Arduino.h"

namespace TeensyTimerTool
{
    GptChannel::~GptChannel()
    {
        stop();
        setCallback(nullptr);

        if (regs == (IMXRT_GPT_t *)&IMXRT_GPT1)
            GPT_t<0>::end();

        else if (regs == (IMXRT_GPT_t *)&IMXRT_GPT2)
            GPT_t<1>::end();
        else
            postError(errorCode::GTP_err);
    }
} // namespace TeensyTimerTool

#endif