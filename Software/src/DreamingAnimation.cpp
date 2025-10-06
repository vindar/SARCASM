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
#include "DreamingAnimation.h"
#include "graphics3D.h"
#include "Sarcasm3DAnimations.h"
#include "config.h"
#include <tgx.h>
#include <Arduino.h>
#include <TeensyThreads.h>
#include "debug.h"
#include "FBSensors.h"
#include "faceAction.h"
#include "display.h"
#include "Console.h"
#include "UI.h"
#include "xorshift32.h"
#include "audioMAX98357A.h"
#include "audioLed.h"
#include "craddlemove.h"
#include "panic.h"
#include "transition_tags.h"
#include "songs.h"



static const char FILENAME_SOUND_START_DREAMING[] PROGMEM = "act_sounds/tag_start_triolet.wav";
static const char FILENAME_SOUND_END_DREAMING[] PROGMEM = "act_sounds/endsolve_idle.wav";
static const char FILENAME_SOUND_ABORT_DREAMING[] PROGMEM = "act_sounds/tag_end_abort.wav";

#define LED_TINT_PERIOD_DREAMING 8000



SLOWFUN int update_display_dreaming(uint32_t em, bool draw3D, bool drawWheel, float ract)
    {    
    const tgx::RGB565 COLOR_WHEEL = tgx::RGB565(18, 0, 0);
    tgx::Image<tgx::RGB565> im_alt = Display::image_alt;
    tgx::Image<tgx::RGB565> im_main = Display::image_main;

    int g = 0;
    if (draw3D)
        {
        im_alt.clear(tgx::RGB565_Black);
        graphics3D::renderer.clearZbuffer();
        g = Sarcasm3DAnimations::DreamingAnimation(); // draw the next frame of the dreaming animation
        }

    UI::drawMainUI(im_main); // copy im_alt to im_main with the deformation. 
    UI::drawUIvoltage(im_main, FBSensors::getVoltage(), FBSensors::getChargingCurrent()); // draw the voltage/charge status    
    UI::drawUIactivity(im_main, { 0, -1 });

    if (ract <= 0)
        {
        UI::drawUIactivity(im_main, { 0, -1 });
        } 
    else
        {
        UI::drawUIactivity(im_main, { 0, (int)(8 * ract) });
        }

    // draw the console text
    Console::console.draw(im_main);


    // draw the spinning wheel
    if (drawWheel)
        {
        const tgx::fVec2 cc = Console::getCharBox({ 24, 0 }).center() - tgx::iVec2(5, 0);
        const float t = fmodf((em / 800.0f), 2.0f);
        const float R = 8.0f;
        if (t < 1.0f)
            {
            const float thick = R * t;
            im_main.drawThickCircleArcAA(cc, R, 0, 360 * t, thick, COLOR_WHEEL, 1.0f); // draw a blue circle arc
            } else
            {
            const float thick = R * (2.0f - t);
            im_main.drawThickCircleArcAA(cc, R, (t - 1.0f) * 360, 360, thick, COLOR_WHEEL, 1.0f); // draw a blue circle arc
            }
        }

    // display
    Display::update(im_main);
    threads.yield();

    return g; // done
    }



SLOWFUN static bool getReturnCode(int& rcode, uint32_t lt)
    {
    if (CraddleMove::status() == 2) { rcode = CODE_SPINNED; return true; }
    if (Display::touchedTime() > lt + 200) { rcode = CODE_TOUCHED_SCREEN; return true; }
    const int cs = FBSensors::chargeStatus();
    if (cs == CODE_CHARGING_START) { rcode = CODE_CHARGING_START; return true; }
    if (cs == CODE_CHARGING_STOP) { rcode = CODE_CHARGING_STOP; return true; }
    return false;
    }



SLOWFUN int dreamingAnimation()
    {

    uint32_t lt = Display::touchedTime();
    int rcode; // return code
    // wait until the face is not visible anymore
    elapsedMillis em = 0; // timer for the animation
    while (faceAction::currentFaceVisible())
        {
        if (getReturnCode(rcode, lt)) { return rcode; }
        if (em > 10000)
            {
            ABORT("INFINITE LOCK: SHOULD NOT HAPPEN");
            }
        graphics3D::updateAll(0);
        threads.yield();
        }

    MAX98357A::setAudioLevelMusic(0.5f * START_SOLVE_SOUND_LEVEL);
    MAX98357A::audioLed.setDriver(MAX98357A::AudioLed::DRIVER_MUSIC);
    MAX98357A::audioLed.setVoiceLedTintAuto(LED_TINT_PERIOD_DREAMING); // set the audio LED to change color automatically
    MAX98357A::playMusic(FILENAME_SOUND_START_DREAMING);

    // now print the message
    Console::console.clear();
    Console::console.setCurrentColor(TFT_CONSOLE_MESSAGE_COLOR);
    int i = 0;

    const int tag_index = transition_tags::picker_tag_dreaming.next();
    const char* msg = transition_tags::tag_dreaming[tag_index];

    while (msg[i])
        {
        if (em > 60) { Console::console.print(msg[i]); em = 0; i++; };
        if (getReturnCode(rcode, lt))
            {
            MAX98357A::audioLed.setDriver(MAX98357A::AudioLed::DRIVER_MUSIC);
            MAX98357A::playMusic(FILENAME_SOUND_ABORT_DREAMING);
            MAX98357A::audioLed.setVoiceLedTint();
            Console::console.clear();
            Console::console.print(msg);
            Console::console.setCurrentColor(TFT_CONSOLE_MESSAGE_COLOR_ABORT);
            Console::console.setText({ 21, 0 }, "STOP");
            Console::console.setCurrentColor();
            Console::console.setCursor(0, 1);
            Console::console.showCursor(true); // show the cursor again   
            Console::console.setCurrentColor(TFT_CONSOLE_COLOR);
            return rcode;
            }

        graphics3D::updateAll(0);
        }

    Console::console.showCursor(false);

    MAX98357A::audioLed.setDriver(MAX98357A::AudioLed::DRIVER_VOICE); // set the audio LED driver to voice mode
    
   
    MAX98357A::setAudioLevelMusic(ANIMATION_DREAMING_SOUND_LEVEL);
    MAX98357A::playMusic(songs::picker_song_dreaming.next_str());

    tgx::Image<tgx::RGB565> im_alt = Display::image_alt;

    faceAction::audioReacEnable(false);
    faceAction::dreamingAppear(im_alt, 3000);

    Sarcasm3DAnimations::InitDreamingAnimation(im_alt);

    em = 0; 
    bool lightup = false;
    while (1)
        {
        if (getReturnCode(rcode, lt))
            {
            MAX98357A::audioLed.setDriver(MAX98357A::AudioLed::DRIVER_MUSIC);
            MAX98357A::playMusic(FILENAME_SOUND_ABORT_DREAMING);
            MAX98357A::setAudioLevelMusic(0.5f * START_SOLVE_SOUND_LEVEL);
            MAX98357A::audioLed.setVoiceLedTint();
            faceAction::audioReacEnable(true);
            faceAction::dreamingDisappear(400);            
            Console::console.setCurrentColor(TFT_CONSOLE_MESSAGE_COLOR_ABORT);
            Console::console.setText({ 21, 0 }, "STOP");
            Console::console.setCurrentColor();
            Console::console.setCursor(0, 1);
            Console::console.showCursor(true); // show the cursor again   
            Console::console.setCurrentColor(TFT_CONSOLE_COLOR);
            return rcode;
            }

        int g = update_display_dreaming(em, true, true, Sarcasm3DAnimations::dreamingRatioDone());
        if (g == 2) { break; } 
        else if ((g == 1) && (lightup == false))
            { 
            lightup = true;
            faceAction::dreamingEnd(im_alt, 4000); 
            }
        }


    const char* endmsg = transition_tags::tag_dreaming_end[tag_index];
    Console::console.setCursor(25 - strlen(endmsg), 0);
    Console::console.setCurrentColor(TFT_CONSOLE_MESSAGE_COLOR_DONE);
    Console::console.print(endmsg);

    faceAction::dreamingDisappear(900);
    MAX98357A::setAudioLevelMusic(0.5f * START_SOLVE_SOUND_LEVEL);
    MAX98357A::audioLed.setDriver(MAX98357A::AudioLed::DRIVER_MUSIC); // set the audio LED driver to voice mode
    MAX98357A::playMusic(FILENAME_SOUND_END_DREAMING, false);
    MAX98357A::audioLed.setVoiceLedTint();

    uint32_t time_end = em;
    while (em < 1000 + time_end)
        { // turn slightly    
        if (getReturnCode(rcode, lt))
            {
            faceAction::audioReacEnable(true);
            Console::console.setCursor(0, 1);
            Console::console.showCursor(true);
            Console::console.setCurrentColor(TFT_CONSOLE_COLOR);           
            return rcode;
            }
        graphics3D::updateAll(0);        
        }

    faceAction::audioReacEnable(true);
    Console::console.setCursor(0, 1);
    Console::console.showCursor(true);
    Console::console.setCurrentColor(TFT_CONSOLE_COLOR);
    return 0;  // animation completed without interruption
    }


/** end of file */

