#if defined(TEENSYDUINO)

#include "TckChannelBase.h"
#include "Arduino.h"
#include "TCK.h"

namespace TeensyTimerTool
{
    FLASHMEM TckChannelBase::~TckChannelBase()
    {
        TCK_t::removeTimer(this);
    }
} // namespace TeensyTimerTool

#endif