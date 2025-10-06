#pragma once

#include "FTM_Info.h"
#include "types.h"

namespace TeensyTimerTool
{
    struct FTM_ChannelInfo
    {
        bool isReserved;
        bool isPeriodic;
        callback_t callback;
        uint32_t reload;
        FTM_CH_t *chRegs;
        float ticksPerMicrosecond;
    };
} // namespace TeensyTimerTool