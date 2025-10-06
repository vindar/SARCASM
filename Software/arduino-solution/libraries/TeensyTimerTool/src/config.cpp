#include "config.h"
#include "TimerModules/TCK/TCK.h"
#include "TimerModules/TCK/tickCounters.h"
#include "boardDef.h"

using tick_t = void (*)();

#if defined(ARDUINO_TEENSY40) || defined(ARDUINO_TEENSY41) || defined(ARDUINO_TEENSY_MICROMOD)
#include "TimerModules/GPT/GPT.h"
#include "TimerModules/PIT4/PIT.h"
#include "TimerModules/TMR/TMR.h"
#include "TimerModules/RTC/RTC.h"

namespace TeensyTimerTool
{
    TimerGenerator *const TMR1 = TMR_t<0>::getTimer;
    TimerGenerator *const TMR2 = TMR_t<1>::getTimer;
    TimerGenerator *const TMR3 = TMR_t<2>::getTimer;
    TimerGenerator *const TMR4 = TMR_t<3>::getTimer;

    TimerGenerator *const GPT1 = GPT_t<0>::getTimer;
    TimerGenerator *const GPT2 = GPT_t<1>::getTimer;

    TimerGenerator *const PIT = PIT_t::getTimer;

    TimerGenerator *const TCK     = TCK_t::getTimer<CycleCounter32>;
    TimerGenerator *const TCK32   = TCK_t::getTimer<CycleCounter32>; // same as TCK
    TimerGenerator *const TCK64   = TCK_t::getTimer<CycleCounter64>;
    TimerGenerator *const TCK_RTC = TCK_t::getTimer<RtcCounter>;

    TimerGenerator *const RTC = RTC_t::getTimer;

    constexpr tick_t tick = &TCK_t::tick;
} // namespace TeensyTimerTool

#elif defined(ARDUINO_TEENSY35) || defined(ARDUINO_TEENSY36)
#include "TimerModules/FTM/FTM.h"

namespace TeensyTimerTool
{
    TimerGenerator *const TCK   = TCK_t::getTimer<CycleCounter32>;
    TimerGenerator *const TCK32 = TCK_t::getTimer<CycleCounter32>; // same as TCK
    TimerGenerator *const TCK64 = TCK_t::getTimer<CycleCounter64>;

    TimerGenerator *const FTM0 = FTM_t<0>::getTimer;
    TimerGenerator *const FTM1 = FTM_t<1>::getTimer;
    TimerGenerator *const FTM2 = FTM_t<2>::getTimer;
    TimerGenerator *const FTM3 = FTM_t<3>::getTimer;
    TimerGenerator *const FTM4 = FTM_t<3>::getTimer;

    constexpr tick_t tick = &TCK_t::tick;
} // namespace TeensyTimerTool

#elif defined(ARDUINO_TEENSY31) || defined(ARDUINO_TEENSY32)
#include "TimerModules/FTM/FTM.h"

namespace TeensyTimerTool
{
    TimerGenerator *const TCK   = TCK_t::getTimer<CycleCounter32>;
    TimerGenerator *const TCK32 = TCK_t::getTimer<CycleCounter32>; // same as TCK
    TimerGenerator *const TCK64 = TCK_t::getTimer<CycleCounter64>;

    TimerGenerator *const FTM0 = FTM_t<0>::getTimer;
    TimerGenerator *const FTM1 = FTM_t<1>::getTimer;
    TimerGenerator *const FTM2 = FTM_t<2>::getTimer;
    constexpr tick_t tick      = &TCK_t::tick;
} // namespace TeensyTimerTool

#elif defined(ARDUINO_TEENSY30)
#include "TimerModules/FTM/FTM.h"

namespace TeensyTimerTool
{
    TimerGenerator *const TCK   = TCK_t::getTimer<CycleCounter32>;
    TimerGenerator *const TCK32 = TCK_t::getTimer<CycleCounter32>; // same as TCK
    TimerGenerator *const TCK64 = TCK_t::getTimer<CycleCounter64>;

    TimerGenerator *const FTM0 = FTM_t<0>::getTimer;
    TimerGenerator *const FTM1 = FTM_t<1>::getTimer;
    constexpr tick_t tick      = &TCK_t::tick;
} // namespace TeensyTimerTool

#elif defined(ARDUINO_TEENSYLC)

namespace TeensyTimerTool
{
    TimerGenerator *const TCK = TCK_t::getTimer<MicrosCounter>;
    constexpr tick_t tick     = &TCK_t::tick;
} // namespace TeensyTimerTool

#endif
