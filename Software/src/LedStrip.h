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
// along with this code. If not, see  <http://www.gnu.org/licenses/>.

#pragma once
#include "config.h"



namespace LedStrip
{

    /**
    * Initialize the led strips
    **/
    SLOWFUN void init();


    /**
    * Set the color of a particular led (call show() to update)
    **/
    SLOWFUN void setLed(int index, int color);

    /**
    * Set the color of all led (call show() to update)
    **/
    SLOWFUN void setAllLed(int color);


    /**
    * Set the color of a particular led (call show() to update)
    **/
    SLOWFUN void setLed(int index, uint8_t R, uint8_t G, uint8_t B, uint8_t W);


    /**
    * Set the color of all led (call show() to update)
    **/
    SLOWFUN void setAllLed(uint8_t R, uint8_t G, uint8_t B, uint8_t W);






    /**
    * Set the color of a particular led (call show() to update)
    * This is the method call be the AudioLed class to set the color of the leds
    * according to the audio level.
    * Can be enabled/disabled with 'enableLedAudio()'
    **/
    SLOWFUN void setLedAudio(int index, uint8_t R, uint8_t G, uint8_t B, uint8_t W);


    /**
    * Enable or disable update of the led by the AudioLed class.
    **/
    SLOWFUN void enableLedAudio(bool enable); 



    /**
    * Update the colors of the leds with the new values. 
    **/
    SLOWFUN void show();


    /**
    * Same as show() but called by the AudioLed class 
    * and can be disabled with 'enableLedAudio(false)'.
    **/
    SLOWFUN void showAudio();


}


/** end of file */

 