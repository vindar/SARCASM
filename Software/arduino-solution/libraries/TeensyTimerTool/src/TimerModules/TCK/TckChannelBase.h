#pragma once

#include "../../ITimerChannel.h"

namespace TeensyTimerTool
{
    class TckChannelBase : public ITimerChannel
    {
     public:
        virtual bool tick() = 0;
        virtual ~TckChannelBase();
    };
}