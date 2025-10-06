//
// Copyright 2021 Arvind Singh
//
// This file is part of the S.A.R.C.A.S.M. source code.
//
// This is a free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this code. If not, see <http://www.gnu.org/licenses/>.

#include "panic.h"
#include "Hardware.h"
#include "Memory.h"
#include "Config.h"
#include "debug.h"

#include "DRV8834.h"
#include "Servos.h"
#include "LedStrip.h"
#include "PowerCtrl.h"
#include "display.h"
#include "FBSensors.h"
#include "audioMAX98357A.h"


#include "font_Arial_10.h"

#include <Arduino.h>

#include <TeensyTimerTool.h>
#include <TeensyThreads.h>



static TeensyTimerTool::PeriodicTimer _panicTimer(PANIC_TIMER_NAME);

static volatile int _panic_on = 0;


/** the periodic interrupt that check for panic condition */
SLOWFUN static  void _panic_int()
    {
    if (!_panic_on) return;

    DEBUG("ENTERING PANIC MODE: [" << Memory::panic_cout_extmem.c_str() << "]");
    
    // disable stepper motor. 
    DRV8834::panic_off();

    // disable servos
    Servos::disablePower();

    // disable the audio output
    MAX98357A::disable(); 

    // disable led audio playback
    LedStrip::enableLedAudio(false); 

    // set red light 
    LedStrip::setAllLed(255, 0, 0, 0);
    LedStrip::show();

    // display error message while blinking
    Display::SetPanicMode();
    elapsedMillis em = 0;

    tgx::Image<tgx::RGB565> im = Display::image_main;

    while (em < PANIC_POWEROFF_DELAY_MS)
        {
        int intensity = em % 500;
        if (intensity > 250) intensity = 500 - intensity;
        LedStrip::setAllLed(255, 0, 0, intensity / 2);
        LedStrip::show();

        im.clear(tgx::RGB565_Red);
        im.drawTextEx("*** PANIC MODE TRIGGERED ***", { 160,30 }, Arial_10, tgx::Anchor::CENTERTOP, false, false, tgx::RGB565_White);
        im.drawTextEx(Memory::panic_cout_extmem.c_str(), {10, 80}, Arial_10, tgx::Anchor::TOPLEFT, true, false, tgx::RGB565_White);

        Memory::panic_cout_extmem2.clear(); 
        Memory::panic_cout_extmem2 << "- Battery Voltage = " << ((int)(10*FBSensors::getVoltage()))/10.0f << "V\n";
        im.drawTextEx(Memory::panic_cout_extmem2.c_str(), { 10,160 }, Arial_10, tgx::Anchor::BOTTOMLEFT, true, false, tgx::RGB565_Black);
        
        Memory::panic_cout_extmem2.clear(); 
        Memory::panic_cout_extmem2 << "- Charging Voltage = " << ((int)(10*FBSensors::getChargingVoltage()))/10.0f << "V\n";
        im.drawTextEx(Memory::panic_cout_extmem2.c_str(), { 10,175 }, Arial_10, tgx::Anchor::BOTTOMLEFT, true, false, tgx::RGB565_Black);

        Memory::panic_cout_extmem2.clear(); 
        Memory::panic_cout_extmem2 << "- Charging Current = " << ((int)(10 * FBSensors::getChargingCurrent()))/10.0f << "A\n";
        im.drawTextEx(Memory::panic_cout_extmem2.c_str(), { 10,190 }, Arial_10, tgx::Anchor::BOTTOMLEFT, true, false, tgx::RGB565_Black);

        Memory::panic_cout_extmem2.clear(); 
        Memory::panic_cout_extmem2 << "SARCASM will turn off in " << (int)((PANIC_POWEROFF_DELAY_MS - em) / 1000) << " seconds...\n";
        im.drawTextEx(Memory::panic_cout_extmem2.c_str(), {160,225}, Arial_10, tgx::Anchor::BOTTOM, true, false, tgx::RGB565_Green);
        
        Display::update_panic();
        }
    // turn off     
    DEBUG("TURNING OFF");
    PowerCtrl::powerOff();
    while (1) { threads.delay(1); }
    }



SLOWFUN void init_panic()
    {
    static int _init_done = 0;
    if (_init_done) return;
    setPanicFlag(0);
    Memory::panic_cout_extmem.clear();
    Memory::panic_cout_extmem2.clear();
    _panic_on = 0;
    _init_done = 1;
    // start the timer
    NVIC_SET_PRIORITY(PANIC_IRQ_NAME, PANIC_IRQ_PRIORITY);
    _panicTimer.begin(_panic_int, PANIC_PERIOD_US);
    NVIC_SET_PRIORITY(PANIC_IRQ_NAME, PANIC_IRQ_PRIORITY);
    }


SLOWFUN void hang()
    {
    while (1)
        {
        threads.yield();
        threads.delay(100);
        }
    }


void set_panic()
    {
    _panic_on = 1;
    }


bool is_panic()
    {
    return(_panic_on != 0);
    }


static volatile int _panic_flag = 0;

void setPanicFlag(int f)
    {
    DRV8834::panic_off(); // power down the cradle
    Servos::disablePower();
    _panic_flag = f;
    }


int getPanicFlag()
    {
    return _panic_flag;
    }



/** end of file */