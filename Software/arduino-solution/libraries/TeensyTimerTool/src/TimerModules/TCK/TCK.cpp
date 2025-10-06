#include "../../config.h"

#if defined(TEENSYDUINO)

#include "TCK.h"

namespace TeensyTimerTool
{
    bool TCK_t::isInitialized = false;
    TckChannelBase *TCK_t::channels[NR_OF_TCK_TIMERS];
} // namespace TeensyTimerTool

//----------------------------------------------------------------------
#if YIELD_TYPE == YIELD_OPTIMIZED

void yield()
{
    TeensyTimerTool::TCK_t::tick();
}

//----------------------------------------------------------------------
#elif YIELD_TYPE == YIELD_STANDARD

#include "EventResponder.h"

namespace TeensyTimerTool
{
    static EventResponder er;

    FLASHMEM void initYieldHook()
    {
        er.attach([](EventResponderRef r) {
            TeensyTimerTool::TCK_t::tick();
            r.triggerEvent();
        });
        er.triggerEvent();
    }
} // namespace TeensyTimerTool
#endif
#endif
