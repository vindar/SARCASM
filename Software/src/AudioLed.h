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
#pragma once

#include <Arduino.h>
#include <Audio.h>
#include <tgx.h>


namespace MAX98357A
    {

#define DEFAULT_VOICE_TINT_R  0.6f
#define DEFAULT_VOICE_TINT_G  0.8f
#define DEFAULT_VOICE_TINT_B  1.0f


    /**
    * Return the current activity level
    **/
    tgx::iVec2 activityLevel();


    /*******************************************************
    *
    * Class for driving the LED strip (either with voice or music)
    *
    ********************************************************/
    class AudioLed : public AudioStream
        {
        public:

            static const int DRIVER_NONE = 0;
            static const int DRIVER_VOICE = 1;
            static const int DRIVER_MUSIC = 2;


            AudioLed() : AudioStream(1, inputQueueArray), _drivertype(DRIVER_NONE), _lastFaceAction(1) {}

            /**
            * Select the driver type.
            **/
            void setDriver(int driver);

            /**
            * Set the tint for led when playing voice
            **/
            void setVoiceLedTint(float R= DEFAULT_VOICE_TINT_R, float G = DEFAULT_VOICE_TINT_G, float B = DEFAULT_VOICE_TINT_B);


            /**
            * Set the tinit to change automatically when playing voice
            * with a given period (in ms).
            **/
            void setVoiceLedTintAuto(uint32_t period); // color 


            /**
            * return the current led tint. 
            **/
            tgx::RGBf getCurrentTint();



            virtual void update();


        private:

            audio_block_t* inputQueueArray[1];
            volatile int _drivertype;



            volatile int _lastFaceAction; // 0 = disappear 1 - appear


        };



    extern AudioLed audioLed; // global instance for audio LED effect

    }


/* end of file **/