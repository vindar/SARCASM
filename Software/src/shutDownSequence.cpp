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
//

#include <Arduino.h>
#include <TeensyThreads.h>
#include <tgx.h>

#include "config.h"
#include "debug.h"
#include "PowerCtrl.h"
#include "UI.h"
#include "Console.h"
#include "AudioMAX98357A.h"
#include "AudioLed.h"
#include "sentences.h"
#include "songs.h"
#include "craddlemove.h"
#include "Display.h"
#include "graphics3D.h"
#include "shutDownSequence.h"
#include "faceAction.h"

const char SHUTDOWN_SEQUENCE_STR[] PROGMEM = "[Shutdown sequence]";
const char FILENAME_SOUND_ABORT_SHUTDOWN[] PROGMEM = "act_sounds/tag_end_high.wav";

SLOWFUN int shutDownSequence()
    {
    //MAX98357A::stopMusic();
    //MAX98357A::stopSpeak();
     
    faceAction::audioReacEnable(false);
    faceAction::appear(2000);
    // select the index of the ending song
    const uint8_t index = songs::picker_song_ending.next();

    // speak the goodbye message
    Console::console.clear();
    MAX98357A::audioLed.setDriver(MAX98357A::AudioLed::DRIVER_VOICE); // set the audio LED driver to voice mode
    MAX98357A::speak(songs::song_ending_intro_sentence(index));
    while (MAX98357A::isSpeaking())
        {
        if (CraddleMove::status() == CODE_SPINNED)
            { // user spun the craddle
            MAX98357A::stopSpeak();
            faceAction::audioReacEnable(true);
            return CODE_SPINNED;
            }
        graphics3D::updateAll(0);
        threads.yield();
        }

    // start playing the ending song
    MAX98357A::setAudioLevelMusic(DEFAULT_SOUND_LEVEL_ENDING_MUSIC);
    MAX98357A::fadeInMusic(FADE_IN_ENDING_TIME_MS);
    MAX98357A::playMusic(songs::song_ending_filename(index));


    faceAction::shuteyes(10*60*1000); // 10 minute is longer than any song.

    bool touched = false;
    bool spinned = false;
    elapsedMillis em = 0;

    Console::console.showCursor(false); // hide the cursor
    for(int i=0; i< 4; i++)
        { 
        graphics3D::updateAll(0);
        threads.yield();
        Console::console.write('\n');
        em = 0;
        while (em < 70) 
            { 
            graphics3D::updateAll(0); 
            threads.yield(); 
            if (CraddleMove::status() == CODE_SPINNED) { spinned = true; }
            } 
        }
    const int cy = TFT_CONSOLE_CHAR_LY - 3;
    Console::console.setCursor(0, cy);
    Console::console.showCursor(true); // hide the cursor
    Console::console.setCurrentColor(TFT_CONSOLE_MESSAGE_COLOR);

    int i = 0;
    while (SHUTDOWN_SEQUENCE_STR[i])
        {
        if (em > 60) { Console::console.print(SHUTDOWN_SEQUENCE_STR[i]); em = 0; i++; };
        if (CraddleMove::status() == CODE_SPINNED) 
            { 
            spinned = true; 
            }
        graphics3D::updateAll(0);
        threads.yield(); // yield to other threads
        }
    Console::console.showCursor(false);

    // display the progress bar and remaining time before shutdown    
    tgx::Image<tgx::RGB565> im_alt = Display::image_alt; // get the main image
    tgx::Image<tgx::RGB565> im_main = Display::image_main; // get the main image

    const uint32_t lt = Display::touchedTime();

    const float music_len = (float)MAX98357A::musicLenghtMs();    
    faceAction::disapear_shutdown(music_len);// disappear slowly

    while (MAX98357A::isPlayingMusic())
        {
        if (CraddleMove::status() == CODE_SPINNED) { spinned = true; }
        if (Display::touchedTime() > lt + 200) { touched = true; }
        if ((spinned) || (touched))
            {
            MAX98357A::stopMusic();
            faceAction::shuteyes(0); // open eyes
            // make beep sound
            MAX98357A::setAudioLevelMusic(SHUTDOWN_ABORT_SOUND_LEVEL);
            MAX98357A::playMusic(FILENAME_SOUND_ABORT_SHUTDOWN);
            while (!MAX98357A::isPlayingMusic()) { threads.yield(); }
            //Console::console.clear();
            Console::console.setCurrentColor(TFT_CONSOLE_MESSAGE_COLOR);
            Console::console.setText({ 0, cy }, SHUTDOWN_SEQUENCE_STR);
            Console::console.setCurrentColor(TFT_CONSOLE_MESSAGE_COLOR_ABORT);
            Console::console.setText({ 20, cy }, "ABORT");
            Console::console.setCurrentColor();
            Console::console.setCursor(0, cy + 2); 
            Console::console.showCursor(true); // show the cursor again   
            graphics3D::updateAll(0);
            threads.yield();
            faceAction::audioReacEnable(true);
            if (spinned) return CODE_SPINNED; // return immediately is spinned !
            while (MAX98357A::isPlayingMusic())
                { // wait until the beep sound is done
                graphics3D::updateAll(0);
                threads.yield();
                }
            // otherwise start to speak the abort shutdown sentence.
            MAX98357A::audioLed.setDriver(MAX98357A::AudioLed::DRIVER_VOICE);
            MAX98357A::speak(sentences::picker_shutdown_aborted.next_str());
            while (MAX98357A::isSpeaking())
                {
                if (CraddleMove::status() == CODE_SPINNED)
                    {
                    return CODE_SPINNED;
                    }
                graphics3D::updateAll(0);
                threads.yield();
                }
            Console::console.print("\n");
            return CODE_TOUCHED_SCREEN;
            }

        float music_pos = (float)MAX98357A::musicPositionMs();
        int s = (int)round(music_len - music_pos) / 1000;
        char sec[6] = "  0s ";
        i = 2;
        while ((s > 0) && (i >= 0)) { sec[i] = '0' + (s % 10); s /= 10; i--; }
        Console::console.eraseLine(cy);
        Console::console.setCurrentColor(TFT_CONSOLE_MESSAGE_COLOR);
        Console::console.setText({ 0, cy }, SHUTDOWN_SEQUENCE_STR);
        Console::console.setCurrentColor(TFT_CONSOLE_MESSAGE_COLOR_ABORT);
        Console::console.setText({ 20, cy }, sec);
        UI::drawMainUI(im_alt);
        Console::drawFromBlank(im_alt, im_main); // erase everything and redraw the background, console, voltage and activity        

        // draw the progress bar on im. 
        float r = music_pos / music_len;
        if (r < 0.0f) r = 0.0f;
        if (r > 1.0f) r = 1.0f;
        int y = (Console::getCharBox({ 0, cy + 1 }).center().y + Console::getCharBox({ 0, cy + 2 }).center().y)/2;
        const int xmin = 15;
        const int xmax = 304;
        const int xmax2 = xmin + (int)((float)(xmax - xmin) * r);
        im_main.fillThickRoundRectAA(tgx::iBox2(xmin, xmax2, y - 18, y + 19), 5, 2, TFT_CONSOLE_MESSAGE_COLOR_ABORT, TFT_CONSOLE_MESSAGE_COLOR_ABORT, 0.5f);
        im_main.drawThickRoundRectAA(tgx::iBox2(xmin, xmax, y - 18, y + 19), 5, 2, TFT_CONSOLE_MESSAGE_COLOR, 1.0f);

        Display::update(im_main); // update
        threads.yield(); // yield to other threads
        }
    // here we turn off !
    PowerCtrl::powerOff();
    return 0;
    }






/** end of file */