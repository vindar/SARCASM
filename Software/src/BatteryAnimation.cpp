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
#include <tgx.h>
#include <TeensyThreads.h>

#include "BatteryAnimation.h"
#include "config.h"
#include "debug.h"
#include "Console.h"
#include "Display.h"
#include "AudioMAX98357A.h"
#include "audioLed.h"
#include "UI.h"
#include "FBSensors.h"
#include "CraddleMove.h"
#include "transition_tags.h"
#include "indexpicker.h"
#include "sentences.h"
#include "graphics3D.h"


static const char FILENAME_SOUND_CHARGING_ON[] PROGMEM = "act_sounds/tag_charge_on.wav";
static const char FILENAME_SOUND_CHARGING_OFF[] PROGMEM = "act_sounds/tag_charge_off.wav";
static const char FILENAME_SOUND_BATTERY_STATUS[] PROGMEM = "act_sounds/tag_start_blip.wav";
static const char FILENAME_SOUND_ABORT_CHARGING[] PROGMEM = "act_sounds/tag_end_abort.wav";

static const char TAG_CHARGE_ON[] PROGMEM   = "[Current injection]";
static const char TAG_CHARGE_OFF[] PROGMEM  = "[Battery mode on]";
static const char TAG_BATTERY[] PROGMEM     = "[Battery status]";
                                           

SLOWFUN static inline tgx::RGB565 batteryColorRGB565(int p) 
    {                                                  
    if (p < 0) { p = 0; } else if (p > 100) { p = 100; }
    const int R_red   = 18, G_red   =  0, B_red   = 0;  // 0%
    const int R_yel   = 18, G_yel   = 35, B_yel   = 0;  // 50%
    const int R_grn   =  0, G_grn   = 35, B_grn   = 5;  // 100%
    int r, g, b;
    if (p <= 50) 
        {
        const int q = 50 - p;
        r = (R_red * q + R_yel * p + 25) / 50;
        g = (G_red * q + G_yel * p + 25) / 50;
        b = (B_red * q + B_yel * p + 25) / 50;
        } 
    else 
        {
        p = p - 50;
        const int q = 50 - p;
        r = (R_yel * q + R_grn * p + 25) / 50;
        g = (G_yel * q + G_grn * p + 25) / 50;
        b = (B_yel * q + B_grn * p + 25) / 50;
        }
    return tgx::RGB565(r, g, b);
    }


SLOWFUN int batteryAnimation(int code, bool clearConsole)
    {
    const uint32_t lt = Display::touchedTime();

    if (MAX98357A::isSpeaking()) 
        { // force clear when interrupting speech
        MAX98357A::stopSpeak(); 
        clearConsole = true; 
        }  
    int y;
    if (clearConsole)
        { // clear
        Console::console.clear(); 
        y = 0;
        } 
    else 
        { // skip 2 lines. 
        y = Console::console.getCursor().y;
        } 
        
    // beep sound
    MAX98357A::setAudioLevelMusic(0.5f * TRANSITION_TAG_SOUND_LEVEL);
    MAX98357A::audioLed.setDriver(MAX98357A::AudioLed::DRIVER_MUSIC);
    MAX98357A::playMusic((code == CODE_CHARGING_START) ? FILENAME_SOUND_CHARGING_ON : ((code == CODE_CHARGING_STOP) ? FILENAME_SOUND_CHARGING_OFF : FILENAME_SOUND_BATTERY_STATUS));

    // create the full tag
    char msg[26];
    memset(msg, ' ', 25); 
    msg[25] = 0;
    strcpy(msg, (code == CODE_CHARGING_START) ? TAG_CHARGE_ON : ((code == CODE_CHARGING_STOP) ? TAG_CHARGE_OFF : TAG_BATTERY));
    msg[strlen(msg)] = ' ';
    int iC; // index where color changes
    tgx::RGB565 C;  // color to use
    const int st = FBSensors::peekChargeStatus();
    const int pct = FBSensors::getBatteryPercent();    
    if (code == CODE_CHARGING_START)
        { // display the current in format "234mA" or "1.23A"
        const int mA = (int)(FBSensors::getChargingCurrent() * 1000); // in mA
        if (mA > 1000)
            {
            msg[20] = '0' + ((mA / 1000) % 10);
            msg[21] = '.';
            msg[22] = '0' + ((mA / 100) % 10);
            msg[23] = '0' + ((mA / 10) % 10);
            msg[24] = 'A';
            }
        else
            {
            if (mA >= 100) { msg[20] = '0' + ((mA / 100) % 10); }
            if (mA >= 10) { msg[21] = '0' + ((mA / 10) % 10); }
            msg[22] = '0' + (mA % 10);
            msg[23] = 'm';
            msg[24] = 'A';
            }
        C = TFT_CONSOLE_MESSAGE_COLOR_DONE;
        iC = 20; 
        }
    else
        { 
        if ((st == CODE_CHARGING) || (st == CODE_CHARGING_START))
            { // display "CHARGING"
            iC = 17;
            strcpy(msg + iC, "CHARGING");
            C = TFT_CONSOLE_MESSAGE_COLOR_DONE;
            }
        else
            { // display the battery level in format "87%"
            int pct = FBSensors::getBatteryPercent();
            iC = 21;
            if (pct >= 100) { msg[21] = '0' + ((pct / 100) % 10); }
            if (pct >= 10) { msg[22] = '0' + ((pct / 10) % 10); }
            msg[23] = '0' + (pct % 10);
            msg[24] = '%';
            C = batteryColorRGB565(pct);
            }
        }

    // print the tag progressively, char by char
    Console::console.setCurrentColor(TFT_CONSOLE_MESSAGE_COLOR);
    Console::console.setCursor(0, y);
    int i = 0;
    elapsedMillis em = 0;
    while (msg[i])
        {
        if (i == iC) { Console::console.setCurrentColor(C); } // change color
        if (em > 60) { Console::console.print(msg[i]); em = 0; i++; };
        if (CraddleMove::status() == CODE_SPINNED)
            {
            MAX98357A::audioLed.setDriver(MAX98357A::AudioLed::DRIVER_MUSIC);
            MAX98357A::playMusic(FILENAME_SOUND_ABORT_CHARGING);
            Console::console.eraseLine(y);
            msg[iC - 1] = 0; 
            Console::console.setCurrentColor(TFT_CONSOLE_MESSAGE_COLOR);
            Console::console.setText({ 0, y }, msg);
            Console::console.setCurrentColor(C);
            Console::console.setText({ iC, y }, msg + iC);
            Console::console.setCursor( 0, y);
            Console::console.print("\n");
            Console::console.showCursor(true); // show the cursor again   
            Console::console.setCurrentColor(TFT_CONSOLE_COLOR);
            return CODE_SPINNED;
            }
        graphics3D::updateAll(0);
        threads.yield();
        }

    // speak the text
    Console::console.setCursor(0, y);
    Console::console.print("\n\n");
    Console::console.showCursor(true);
    Console::console.setCurrentColor(TFT_CONSOLE_COLOR);
    const char* text;
    if (code == CODE_CHARGING_START)
        {
        text = sentences::picker_power_on.next_str(); 
        }
    else if (code == CODE_CHARGING_STOP)
        {
        text = sentences::picker_power_off.next_str();
        }
    else if ((st == CODE_CHARGING) || (st == CODE_CHARGING_START))
        {
        text = sentences::picker_battery_charging.next_str();
        }
    else
        {
        if (pct <= VOLTAGE_THRESHOLD_LOW)
            {
            text = sentences::picker_battery_critical.next_str();
            }
        else
            {
            text = sentences::picker_battery_ok.next_str();
            }
        }
    MAX98357A::audioLed.setDriver(MAX98357A::AudioLed::DRIVER_VOICE);
    MAX98357A::speak(text);

    while (MAX98357A::isSpeaking())
        {
        if (CraddleMove::status() == CODE_SPINNED) 
            { // the craddle was spinned 
            FBSensors::chargeStatus(); 
            return CODE_SPINNED; 
            }
        graphics3D::updateAll(0); // redraw the screen
        threads.yield(); // 
        }
    Console::console.print("\n");
    FBSensors::chargeStatus();// discard change in battery status
    return ((CraddleMove::status() == CODE_SPINNED) ? CODE_SPINNED : ((Display::touchedTime()  > lt + 200) ? CODE_TOUCHED_SCREEN : 0));
    }




/** end of file */

