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
#include <Arduino.h>


#include "Hardware.h"
#include "config.h"
#include "debug.h"
#include "InitAll.h"

#include "FBSensors.h"
#include "Memory.h"
#include "LedStrip.h"
#include "ESPCAM.h"
#include "SerialPacket.h"
#include "SCommandResponder.h"
#include "Servos.h"
#include "PowerCtrl.h"
#include "DRV8834.h"
#include "display.h"
#include "audioMAX98357A.h"
#include "intro.h"
#include "panic.h"
#include "graphics3D.h"
#include "xorshift32.h"

#include "teensythread.h"
#include <TeensyThreads.h>


SLOWFUN bool initAll(bool wait_for_serial)
    {
    [[maybe_unused]] elapsedMillis em = 0;

    // minimum initialization before we can play the intro scene
    
    rng::initRNG();         // initialize the random number generator (cannot panic, nvs always available)
    init_panic();           // start the panic handler
    Memory::init();         // initialize memory. never panic
    LedStrip::init();       // initialize the ledstrip. never panic
    Display::init();        // initialize the display (may panic but this means real harware failure...)
    MAX98357A::init();      // initialize the audio output (may panic but this means hardware failure (SD card not present/detected).

    // ok, initialise teensythread
    //threads.setSliceMicros(TEENSYTHREADS_SLICE_US);             // duration of each tick 
    //threads.setTimeSlice(0, TEENSYTHREADS_DEFAULT_TICKS);       // the main threads ...
    //threads.setDefaultTimeSlice(TEENSYTHREADS_DEFAULT_TICKS);   // ... and other threads have same duration by default. 

    const uint32_t lt = Display::touchedTime(); // last touch time of the screen

    // play the intro while doing initialization of the other peripherals
    int th = threads.addThread(playIntro, 0, ESPEAK_STACK_SIZE, &(Memory::espeak_stack)); 
    if (th < 0) { ABORT("initAll() failed to start intro thread !"); }    
    //teensythread::speedUp(th, 2.0);

    debug::init(wait_for_serial);
    PowerCtrl::init();
    FBSensors::init();
    ESPCAM::init();
    DRV8834::init();
    Servos::init();
    Servos::Grip::calibrateOffset();

    Servos::Tumble::calibrateOffset();
    graphics3D::init3D();

    threads.wait(th); // wait for intro to end playing 
    FBSensors::chargeStatus(); // discard charge status changes during intro

    DEBUG("Intro stack usage : " <<  teensythread::stackUsage(Memory::espeak_stack, ESPEAK_STACK_SIZE)); // print the sta²²ck usage of the intro thread

    return (Display::touchedTime() != lt); // return true if the screen was touched during startup
    }


/** end of file */
