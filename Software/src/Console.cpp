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


#include "Console.h"
#include "font_SourceCodePro_AA4.h"
#include "font_SourceCodePro_Bold_AA4.h"

#include "UI.h"
#include "AudioLed.h"
#include "Display.h"
#include "FBSensors.h"


namespace Console
    {

    TFTConsole console; // global console object


    SLOWFUN void drawFromBlank(const tgx::Image<tgx::RGB565> & im_ui_blank)
        {
        drawFromBlank(im_ui_blank, Display::image_main);
        }


    SLOWFUN void drawFromBlank(const tgx::Image<tgx::RGB565>& im_ui_blank, tgx::Image<tgx::RGB565> im_dest)
        {        
        drawFromBlankActivity(MAX98357A::activityLevel(), im_ui_blank, im_dest);
        }


    SLOWFUN void drawFromBlankActivity(tgx::iVec2 act, const tgx::Image<tgx::RGB565>& im_ui_blank)
        {
        drawFromBlankActivity(act, im_ui_blank, Display::image_main);
        }


    SLOWFUN void drawFromBlankActivity(tgx::iVec2 act, const tgx::Image<tgx::RGB565>& im_ui_blank, tgx::Image<tgx::RGB565> im_dest)
        {        
        im_dest.blit(im_ui_blank, { 0,0 }); // copy the ui
        UI::drawMainUI(im_dest);
        UI::drawUIactivity(im_dest, act); // draw the current activity bar
        UI::drawUIvoltage(im_dest, FBSensors::getVoltage(), FBSensors::getChargingCurrent()); // draw the voltage/charge status
        Console::console.draw(im_dest); // draw the text of the console on the image
        }




    SLOWFUN tgx::iBox2 getCharBox(const tgx::iVec2& pos)
        {
        const int xx = TFT_CONSOLE_OFF_X + TFT_CONSOLE_FONT_WIDTH * pos.x;
        const int yy = TFT_CONSOLE_OFF_Y + TFT_CONSOLE_FONT_HEIGHT * pos.y;
        return tgx::iBox2(xx, xx + TFT_CONSOLE_FONT_WIDTH, yy, yy + TFT_CONSOLE_FONT_HEIGHT);
        }


    SLOWFUN TFTConsole::TFTConsole() : _cx(0), _cy(0), _currentcolor((uint16_t)TFT_CONSOLE_COLOR), _cursor_on(true), _em(0)
        {
        memset(_buffer, ' ', sizeof(_buffer));
        memset(_color, 0, sizeof(_color)); // initialize the color buffer to 0 (black)
        }


    SLOWFUN void TFTConsole::clear()
        {
        memset(_buffer, ' ', sizeof(_buffer));
        memset(_color, 0, sizeof(_color)); // initialize the color buffer to 0 (black)
        _cx = 0;
        _cy = 0;
        }


    SLOWFUN void TFTConsole::showCursor(bool show)
        {
        _cursor_on = show;
        }


    SLOWFUN void TFTConsole::setCurrentColor(tgx::RGB565 color)
        {
        _currentcolor = (uint16_t)color; // set the current color
        }



    SLOWFUN void TFTConsole::eraseLine(int y)
        {
        if ((y < 0) || (y >= TFT_CONSOLE_CHAR_LY)) return; // out of bounds
        const int o = y * TFT_CONSOLE_CHAR_LX; // offset in the buffer
        memset(_buffer + o, ' ', TFT_CONSOLE_CHAR_LX); // clear the line
        }

    SLOWFUN void TFTConsole::setChar(tgx::iVec2 pos, char c)
        {
        if ((pos.x < 0) || (pos.x >= TFT_CONSOLE_CHAR_LX) || (pos.y < 0) || (pos.y >= TFT_CONSOLE_CHAR_LY)) return; // out of bounds
        if (c < 32) c = 32; // replace control characters with space
        const int o = (pos.y * TFT_CONSOLE_CHAR_LX) + pos.x; // offset in the buffer
        _buffer[o] = c; // set the character
        _color[o] = _currentcolor; // set the color of the character
        }


    SLOWFUN void TFTConsole::setText(tgx::iVec2 pos, const char* txt)
        {
        if ((pos.x < 0) || (pos.x >= TFT_CONSOLE_CHAR_LX) || (pos.y < 0) || (pos.y >= TFT_CONSOLE_CHAR_LY)) return; // out of bounds
        int i = 0;
        while (txt[i] && (i + pos.x < TFT_CONSOLE_CHAR_LX))
            { // copy the text to the buffer
            auto o = (pos.y * TFT_CONSOLE_CHAR_LX) + pos.x + i;
            if (txt[i] < 32) _buffer[o] = ' '; else _buffer[o] = txt[i];
            _color[o] = _currentcolor; // set the color of the character
            i++;
            }
        }


    SLOWFUN size_t TFTConsole::write(uint8_t c)
        {
        if (c != '\n')
            { // regular char
            if (c < 32) c = 32;  // replace control characters with space
            const int o = (_cy * TFT_CONSOLE_CHAR_LX) + _cx; // offset in the buffer
            _buffer[o] = c;
            _color[o] = _currentcolor;
            _cx++; // now update the cursor position            
            if (_cx >= TFT_CONSOLE_CHAR_LX)
                {
                _cx = 0; // reset the cursor to the beginning of the line
                _cy++; // move to the next line
                }
            } 
        else
            { // new line
            _cx = 0;
            _cy++;
            }
            if (_cy >= TFT_CONSOLE_CHAR_LY)
                { // scrolling required
                _cy = TFT_CONSOLE_CHAR_LY - 1; // keep the cursor at the last line
                for (int j = 0; j < TFT_CONSOLE_CHAR_LY - 1; j++)
                    {
                    for (int i = 0; i < TFT_CONSOLE_CHAR_LX; i++)
                        {
                        const int o = (j * TFT_CONSOLE_CHAR_LX) + i; // offset in the buffer
                        _buffer[o] = _buffer[o + TFT_CONSOLE_CHAR_LX]; // move the character from the next line to the current line
                        _color[o] = _color[o + TFT_CONSOLE_CHAR_LX];
                        }
                    }
                memset(_buffer + ((TFT_CONSOLE_CHAR_LY - 1) * TFT_CONSOLE_CHAR_LX), ' ', TFT_CONSOLE_CHAR_LX); // clear the last line
                memset(_color + ((TFT_CONSOLE_CHAR_LY - 1) * TFT_CONSOLE_CHAR_LX), 0, TFT_CONSOLE_CHAR_LX*sizeof(uint16_t)); // clear the last line
                }
            _em = 0; // reset the elapsed millis for the cursor
            return 1;
        }


    SLOWFUN void TFTConsole::setCursor(int x, int y)
        {
        if (x < 0 || x >= TFT_CONSOLE_CHAR_LX || y < 0 || y >= TFT_CONSOLE_CHAR_LY) return; // out of bounds
        _cx = x;
        _cy = y;
        _em = 0; // reset cursor timer
        }


    SLOWFUN tgx::iVec2 TFTConsole::getCursor()
        {
        return { _cx, _cy }; 
        }



    SLOWFUN void TFTConsole::_drawChar(tgx::Image<tgx::RGB565>& im, int i, int j, float opacity)
        {
        const int o = i + (j * TFT_CONSOLE_CHAR_LX);
        const char c = _buffer[o];
        const tgx::RGB565 color = _color[o];
        const int x = TFT_CONSOLE_OFF_X + TFT_CONSOLE_FONT_WIDTH * i;
        const int y = TFT_CONSOLE_OFF_Y + TFT_CONSOLE_FONT_HEIGHT * j;
        if ((color == TFT_CONSOLE_COLOR) && (opacity >= 1.0f))
            { // hack : yellow text is semi transparent. 
            opacity = 0.6f; 
            }
        if ((color == TFT_CONSOLE_MESSAGE_COLOR)||(color == TFT_CONSOLE_MESSAGE_COUNT) ||(color == TFT_CONSOLE_MESSAGE_COUNT) || (color == TFT_CONSOLE_MESSAGE_COLOR_ABORT) || (color == TFT_CONSOLE_MESSAGE_COLOR_DONE))
            { // hack, use bold font
            im.drawChar(c, { x, y + TFT_CONSOLE_BASELINE_OFF - 1}, TFT_CONSOLE_FONT_BOLD, color, opacity); // draw the character
            }
        else
            {
            im.drawChar(c, { x, y + TFT_CONSOLE_BASELINE_OFF }, TFT_CONSOLE_FONT, color, opacity); // draw the character
            }
        
        }


    SLOWFUN void TFTConsole::draw(tgx::Image<tgx::RGB565>& im, bool force_cursor)
        {
        static bool first_draw = true;
        if (first_draw)
            { // first time we draw: restart cursor timer
            first_draw = false; // only do this once
            _em = 0;
            }
        const int cx = _cx;
        const int cy = _cy;
        for (int j = 0; j < TFT_CONSOLE_CHAR_LY; j++)
            {
            for (int i = 0; i < TFT_CONSOLE_CHAR_LX; i++)
                {

                _drawChar(im, i, j, 1.0f); // draw the character at (i,j)
                }
            }
        if (force_cursor) { _em = 0; }
        const bool cursor_visible = ((_em % (TFT_CONSOLE_CURSOR_ON_MS + TFT_CONSOLE_CURSOR_OFF_MS)) < TFT_CONSOLE_CURSOR_ON_MS);
        if ((cursor_visible)&&(_cursor_on))
            {
            im.fillRect({ TFT_CONSOLE_CURSOR_OFF_X + (TFT_CONSOLE_FONT_WIDTH * cx), TFT_CONSOLE_CURSOR_OFF_X + (TFT_CONSOLE_FONT_WIDTH * cx) + TFT_CONSOLE_CURSOR_LX,
                          TFT_CONSOLE_CURSOR_OFF_Y + (TFT_CONSOLE_FONT_HEIGHT * cy), TFT_CONSOLE_CURSOR_OFF_Y + (TFT_CONSOLE_FONT_HEIGHT * cy) + TFT_CONSOLE_CURSOR_LY }, _currentcolor, 1.0f);
            }
        }


    }


/** end of file */
