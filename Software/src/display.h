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


#include "config.h"

#include <ILI9341_T4.h>
#include <Arduino.h>
#include <tgx.h>


namespace Display
    {

    extern ILI9341_T4::ILI9341Driver _tft; // the screen driver object

    extern const tgx::Image<tgx::RGB565> image_main;              // the main framebuffer image
    extern const tgx::Image<tgx::RGB565> image_alt;               // the alternate framebuffer image
    extern const tgx::Image<tgx::RGB565> image_zbuffer;           // the z-buffer image   
    extern const tgx::Image<tgx::RGB565> image_extmem;            // the alternate framebuffer image in extmem
    extern const tgx::Image<tgx::RGB565> image_extmem_2;          // the second alternate framebuffer image in extmem

    extern const tgx::Image<tgx::RGB565> image_stickers_texture;  // in extmem
    extern const tgx::Image<tgx::RGB565> image_stickers_tmp;      // in DMAMEM



    /**
    * Initialize the display.
    **/
	SLOWFUN void init();


    /**
    * Update the display with the content of framebuffer image()
    **/
    void update(bool force_full_redraw = false);
    void update(const tgx::Image<tgx::RGB565>& im, bool force_full_redraw = false);


    /**
    * Wait for the DMA transfer to complete
    **/
    void waitUpdateComplete();


    /**
    * Update the display when in panic mode.
    **/
    void update_panic();


    /**
    * Update a specifed region of the display.
    **/
    void updateRegion(tgx::iBox2 region, bool redraw_now);


    /**
    * Turn on or off the screen backlight
    **/
    SLOWFUN void backlight(bool enable);


    /**
    * Move all display memory to extmem to free up DTCM and DMA for kociemba
    **/
    SLOWFUN void displayToEXTMEM(bool enable);


    /**
    * Put the display in panic mode: direct SPI transfer. 
    **/
    SLOWFUN void SetPanicMode();


    /**
    * Return the last time it was touched, in milliseconds from the start. 
    **/
    SLOWFUN uint32_t touchedTime();

    }




/** end of file */
