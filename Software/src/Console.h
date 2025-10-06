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
#include "debug.h"
#include <tgx.h>
#include <Arduino.h>


// using FONT 20
#define TFT_CONSOLE_COLOR (tgx::RGB565_Yellow)
#define TFT_CONSOLE_MESSAGE_COLOR (tgx::RGB565(8, 34, 14))
//(tgx::RGB565(22, 54, 29))
#define TFT_CONSOLE_MESSAGE_COLOR_DONE (tgx::RGB565(0,35,5))
#define TFT_CONSOLE_MESSAGE_COLOR_ABORT (tgx::RGB565(18,0,0))
#define TFT_CONSOLE_MESSAGE_COUNT (tgx::RGB565(8, 34, 14)) // (tgx::RGB565_Blue)


//#define TFT_CONSOLE_COLOR_CURSOR (tgx::RGB565_Yellow)
#define TFT_CONSOLE_FONT (font_SourceCodePro_AA4_20)
#define TFT_CONSOLE_FONT_BOLD (font_SourceCodePro_Bold_AA4_20)
#define TFT_CONSOLE_FONT_HEIGHT (20)
#define TFT_CONSOLE_FONT_WIDTH (12)
#define TFT_CONSOLE_OFF_X (11)
#define TFT_CONSOLE_OFF_Y (30)
#define TFT_CONSOLE_BASELINE_OFF (15)

#define TFT_CONSOLE_CHAR_LX (25)
#define TFT_CONSOLE_CHAR_LY (10)

#define TFT_CONSOLE_CURSOR_ON_MS  (500)
#define TFT_CONSOLE_CURSOR_OFF_MS (400)

#define TFT_CONSOLE_CURSOR_OFF_X (12)
#define TFT_CONSOLE_CURSOR_OFF_Y (43)
#define TFT_CONSOLE_CURSOR_LX (8)
#define TFT_CONSOLE_CURSOR_LY (3)


/*
// Using FONT 18
#define TFT_CONSOLE_COLOR (tgx::RGB565_Yellow)
#define TFT_CONSOLE_FONT (font_SourceCodePro_AA4_18)
#define TFT_CONSOLE_FONT_HEIGHT (18)
#define TFT_CONSOLE_FONT_WIDTH (11)
#define TFT_CONSOLE_OFF_X (11)
#define TFT_CONSOLE_OFF_Y (32)
#define TFT_CONSOLE_BASELINE_OFF (13)

#define TFT_CONSOLE_CHAR_LX (27)
#define TFT_CONSOLE_CHAR_LY (11)

#define TFT_CONSOLE_CURSOR_ON_MS  (500)
#define TFT_CONSOLE_CURSOR_OFF_MS (400)

#define TFT_CONSOLE_CURSOR_OFF_X (12)
#define TFT_CONSOLE_CURSOR_OFF_Y (43)
#define TFT_CONSOLE_CURSOR_LX (7)
#define TFT_CONSOLE_CURSOR_LY (3)
*/

/*
// Using  FONT 16
#define TFT_CONSOLE_COLOR (tgx::RGB565_Yellow)
#define TFT_CONSOLE_FONT (font_SourceCodePro_AA4_16)
#define TFT_CONSOLE_FONT_HEIGHT (16)
#define TFT_CONSOLE_FONT_WIDTH (10)
#define TFT_CONSOLE_OFF_X (11)
#define TFT_CONSOLE_OFF_Y (26)
#define TFT_CONSOLE_BASELINE_OFF (12)

#define TFT_CONSOLE_CHAR_LX (30)
#define TFT_CONSOLE_CHAR_LY (13)

#define TFT_CONSOLE_CURSOR_ON_MS  (500)
#define TFT_CONSOLE_CURSOR_OFF_MS (400)

#define TFT_CONSOLE_CURSOR_OFF_X (12)
#define TFT_CONSOLE_CURSOR_OFF_Y (36)
#define TFT_CONSOLE_CURSOR_LX (7)
#define TFT_CONSOLE_CURSOR_LY (3)
*/


/**
  FONT 20 : baseline = 15 lx = 12 ly = 20
  FONT 18 : baseline = 13 lx = 11 ly = 18
  FONT 16 : baseline = 12 lx = 10 ly = 16
*/

namespace Console
    {



    /**
    * Draw, the UI, activity, voltage and console text on the main image. 
    * Use im_ui_blank as the backgorund image (so it should be the sarcasm background
    * with the SARCASM CMD and round rect drawn on it).
    **/
    void drawFromBlank(const tgx::Image<tgx::RGB565>& im_ui_blank);
    void drawFromBlank(const tgx::Image<tgx::RGB565>& im_ui_blank, tgx::Image<tgx::RGB565> im_dest);
    void drawFromBlankActivity(tgx::iVec2 act, const tgx::Image<tgx::RGB565>& im_ui_blank);
    void drawFromBlankActivity(tgx::iVec2 act, const tgx::Image<tgx::RGB565>& im_ui_blank, tgx::Image<tgx::RGB565> im_dest);



    /**
    * Return the bounding box of a given character
    **/
    tgx::iBox2 getCharBox(const tgx::iVec2& pos);



    /**
    * SARCASM text console. Lazy redraw (only when draw is called)
    **/
    class TFTConsole : public Print
        {


        public:

            /**
            * ctor
            **/
            TFTConsole();


            /**
            * Clear the console, put the cursor at (0,0)
            **/
            void clear();


            /**
            * Draw the console onto the image.
            * (incremental, only draws the changed characters)
            **/
            void draw(tgx::Image<tgx::RGB565>& im, bool force_cursor = false);


            /**
            * Show or not the cursor on the screen. 
            **/
            void showCursor(bool show);


            /**
            * Set the cursor position.
            **/
            void setCursor(int x, int y);


            /**
            * Erase a given line
            * (does not affect the cursor)
            **/
            void eraseLine(int y);


            /**
            * Set a character at a given position in the console.
            * (does not affect the cursor)
            **/
            void setChar(tgx::iVec2 pos, char c);


            /**
            * Set a text at a given position in the console.
            * Truncate at end of line
            * (does not affect the cursor)
            **/
            void setText(tgx::iVec2 pos, const char* txt);


            /**
            * Get the cursor position.
            **/
            tgx::iVec2 getCursor();


            /**
            * Set the text color from now on.
            **/
            void setCurrentColor(tgx::RGB565 color = TFT_CONSOLE_COLOR);


            /**
            * Print overload, for adding text to the console.
            **/
            virtual size_t write(uint8_t c) override;




        private:

            void _drawChar(tgx::Image<tgx::RGB565>& im, int i, int j, float opacity);
            volatile int _cx, _cy; // cursor position
            char _buffer[TFT_CONSOLE_CHAR_LX * TFT_CONSOLE_CHAR_LY]; // content of the console
            uint16_t _color[TFT_CONSOLE_CHAR_LX * TFT_CONSOLE_CHAR_LY]; // color of each character
            uint16_t _currentcolor;
            bool _cursor_on; // cursor visibility
            elapsedMillis _em;
        };




    extern TFTConsole console; // global console instance

    }

/** end of file */

