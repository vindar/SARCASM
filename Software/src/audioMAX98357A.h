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
#pragma once

#include "config.h"
#include "Console.h"


namespace MAX98357A
    {

    /**
     * Intialiase the audio hardware
     **/
    SLOWFUN void init();


    /**
     * Disable the audio  (power off MAX98357A)
     **/
    SLOWFUN void disable();


    /**
    * Set the audio level for the music.
    **/
    SLOWFUN void setAudioLevelMusic(float level = 1.0f);


    /**
    * Set the audio level for the voice.
    **/
    SLOWFUN void setAudioLevelVoice(float level = 1.0f);


    /**
    * Set the audio level for both music and voice.
    **/
    SLOWFUN void setAudioLevel(float level = 1.0f);


    /**
    * Fade in for the music channel
    **/
    SLOWFUN void fadeInMusic(int ms = FADE_IN_TIME_MS);

    /**
    * Fade out for the music channel
    **/
    SLOWFUN void fadeOutMusic(int ms = FADE_OUT_TIME_MS);


    /*******************************************************************
    * Methods for playing a music file
    *******************************************************************/

    /**
    * Play a music file
    **/
    SLOWFUN void playMusic(const char* filename, bool wait = false);

    /**
    * Stop any misic that is currently playing
    **/
    SLOWFUN void stopMusic();

    /**
    * Query if a file is currently playing.
    **/
    bool isPlayingMusic();

    /**
    * Wait until the file is finished playing.
    **/
    SLOWFUN void waitEndPlayMusic();

    SLOWFUN uint32_t musicLenghtMs();


    SLOWFUN uint32_t musicPositionMs();



    /*******************************************************************
    * Methods for speaking a text
    *******************************************************************/


    /**
    * Start speaking a text using the espeak-ng library
    **/
    SLOWFUN void speak(const char* text, Print * textOutput = &Console::console);


    /**
    * Speak a formatted text
    * -> "test [] {} <>" is replaced by "test v1 v2 v3"
    **/
    SLOWFUN void speakformatted(const char* text, int v1, int v2 = 0, int v3 = 0, Print* textOutput = &Console::console);



    /**
    * Stop speaking the text.
    **/
    SLOWFUN void stopSpeak();

    /**
    * Query is a text is currently being spoken.
    **/
    bool isSpeaking();

    /**
    * While until the text is finished speaking.
    **/
    SLOWFUN void waitEndSpeak();



    }

/** end of file */