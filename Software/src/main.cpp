//**************************************************************************
//
// S.A.R.C.A.S.M : Slightly Annoying Rubik's Cube Automatic Solving Machine.
//
//**************************************************************************
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
#include <kociemba.h>
#include <TeensyThreads.h>
#include <tgx.h>

#include "Config.h"
#include "Hardware.h"
#include "debug.h"
#include "InitAll.h"
#include "FBSensors.h"
#include "Memory.h"
#include "PowerCtrl.h"
#include "DRV8834.h"
#include "LedStrip.h"
#include "ESPCAM.h"
#include "Servos.h"
#include "panic.h"
#include "display.h"
#include "audioMAX98357A.h"
#include "AudioLed.h"
#include "craddlemove.h"
#include "BasicMoves.h"
#include "CubeRead.h"
#include "CubeOrientation.h"
#include "CubeState.h"
#include "SCommandResponder.h"
#include "Console.h"
#include "RubikCube3D.h"
#include "UI.h"
#include "graphics3D.h"
#include "teensythread.h"
#include "xorshift32.h"
#include "sentences.h"
#include "transition_tags.h"
#include "songs.h"
#include "solveCube.h"
#include "shutDownSequence.h"
#include "faceAction.h"
#include "idleSolveAnimation.h"
#include "BatteryAnimation.h"
#include "DreamingAnimation.h"
#include "RepeatSolvingAnimation.h"
#include "mainLoop.h"



/**
* Called when a crash was detected during the previous run.
* Save log report on SD card, wait a bit, and then powerOff.
**/
static void crash()
    {
    if (SD.begin(BUILTIN_SDCARD))
        { // append the crash report to the crashlog.txt file
        File logFile = SD.open("crashlog.txt", FILE_WRITE);
        if (logFile)
            {
            logFile.print(CrashReport);
            logFile.close();
            }
        }
    MAX98357A::audioLed.setDriver(MAX98357A::AudioLed::DRIVER_VOICE); // set the audio LED driver to voice mode
    Console::console.setCurrentColor(TFT_CONSOLE_MESSAGE_COLOR_ABORT);
    MAX98357A::speak("CrashReport log created\non SD card.\n\nShutdown in 1 minutes.");
    elapsedMillis em = 0;
    while (em < 60 * 1000)
        {
        graphics3D::updateAll(0); // redraw the screen
        threads.yield();
        }
    PowerCtrl::powerOff(); // turn off the machine
    }


/**
* Called when reset is requested. 
* Reset all the pickers (sentences, transition tags, songs).
**/
static void reset(bool deterministic_at_startup)
    {
    sentences::resetSentences(deterministic_at_startup);
    transition_tags::resetTags(deterministic_at_startup);
    songs::resetSongs(deterministic_at_startup);
    rng::resetRNGnextTime(); 

    Console::console.setCurrentColor(TFT_CONSOLE_MESSAGE_COLOR_ABORT);
    MAX98357A::audioLed.setDriver(MAX98357A::AudioLed::DRIVER_VOICE); // set the audio LED driver to voice mode
    MAX98357A::speak("Factory reset.\n\n");
    while (MAX98357A::isSpeaking())
        {
        graphics3D::updateAll(0); // redraw the screen
        threads.yield(); // 
        }
    Console::console.setCurrentColor(TFT_CONSOLE_MESSAGE_COLOR);
    }



/**
* Main function that runs when powering on the machine;
**/
SLOWFUN int main()
    {
    // intialize the hardware and play the introduction
    const bool touched_at_startup = initAll(false);

    // 1. Uncomment below to run cradle calibration. 
    //DRV8834::cradleCalibration();

    // 2. uncomment below to run servo calibraton (calibration data and info on Serial/Serial2 output)    
    //Servos::Grip::fullCalibration(3);
    //Servos::Tumble::fullCalibration(3);

    // 3. Uncomment below to run SCommandResponder to use with
    // the SARCASM2_PC program to perform camera calibration. 
    //SCommandResponder::start();


    if (CrashReport)
        {  // crash during the previous run: log report it to SD, wait a bit, and then powerOff.
        crash();
        }

    if (touched_at_startup)
        { // touchscreen was pressed during startup: do a reset of the pickers
        reset(true); // reset in deterministic mode
        }

    // enter the main loop, never to return...
    mainLoop();
     
    return 0;
    }




/** end of file */