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

#include "display.h"

#include "config.h"
#include "memory.h"
#include "debug.h"
#include "panic.h"

#include <TeensyThreads.h>
#include <ILI9341_T4.h>
#include <tgx.h>


namespace Display
	{

    // the screen driver object
    ILI9341_T4::ILI9341Driver _tft(PIN_ILI9341_CS, PIN_ILI9341_DS, PIN_ILI9341_SCK, PIN_ILI9341_MOSI, PIN_ILI9341_MISO, PIN_ILI9341_RESET, 255, 255); // <- disable touch. We deal with touch irq separately


    // 2 diff buffers with about 6K memory each
    ILI9341_T4::DiffBuff _diff1(Memory::buffer_diff1, ILI9341_DIFFBUFFER_SIZE);
    ILI9341_T4::DiffBuff _diff2(Memory::buffer_diff2, ILI9341_DIFFBUFFER_SIZE);

    volatile int _display_panic_mode = 0; 

    const tgx::Image<tgx::RGB565> image_main(Memory::framebuffer_main, ILI9341_LX, ILI9341_LY);
    const tgx::Image<tgx::RGB565> image_alt(Memory::dma_shared.A.framebuffer_alt, ILI9341_LX, ILI9341_LY);
    const tgx::Image<tgx::RGB565> image_zbuffer(Memory::dma_shared.A.zbuffer, ILI9341_LX, ILI9341_LY);
    const tgx::Image<tgx::RGB565> image_extmem(Memory::framebuffer_extmem, ILI9341_LX, ILI9341_LY);
    const tgx::Image<tgx::RGB565> image_extmem_2(Memory::framebuffer_extmem_2, ILI9341_LX, ILI9341_LY);

    const tgx::Image<tgx::RGB565> image_stickers_texture(Memory::cube_stickers_texture, CUBE_TEXTURE_LX, CUBE_TEXTURE_LX*CUBE_NB_STICKERS);
    const tgx::Image<tgx::RGB565> image_stickers_tmp(Memory::dma_shared.A.cube_stickers_tmp, CUBE_TEXTURE_LX, CUBE_TEXTURE_LX);




    volatile uint32_t _last_touched_time = 0;

    /** The touchscreen interrupt.**/    
    FASTRUN void touch_irq_cb()
        { // called when the touchscreen is touched
        _last_touched_time = millis();
        return;
        }


    uint32_t touchedTime()
        {
        return _last_touched_time;        
        }


	SLOWFUN void init()
		{
		static int8_t _init_done = 0;
		if (_init_done) return;
		_init_done = 1;
        _display_panic_mode = 0;
        DEBUG("Display::init() started")


        // disable touch via SPI.
        pinMode(PIN_ILI9341_TOUCH_IRQ, OUTPUT);
        pinMode(PIN_ILI9341_TOUCH_CS, OUTPUT);
        digitalWrite(PIN_ILI9341_TOUCH_CS, HIGH);

        // but intercept the touch interrupt
        attachInterrupt(PIN_ILI9341_TOUCH_IRQ, touch_irq_cb, FALLING);

        // now we can initialize the display
        int nbt = 0; 
        while (!_tft.begin(ILI9341_SPI_SPEED))
            {
            DEBUG("ILI9341_T4 Initialization error...");
            threads.delay(100);
            if (nbt++ == 5)
                {
                ABORT("ILI9341_T4 initialization failed!");
                }
            }

        #if defined(_DEBUG_SERIAL)
        _tft.output(&_DEBUG_SERIAL); // bugged ?
        #endif  

        _tft.setIRQPriority(ILI9341_IRQ_PRIORITY);    // set the IRQ priority for the display.
        _tft.setRotation(ILI9341_ORIENTATION);        // portrait mode 240 x320        
        _tft.setFramebuffers(Memory::dma_shared.A.framebuffer_internal);   // set internal framebuffer -> activate double buffering.
        _tft.setDiffBuffers(&_diff1, &_diff2);        // set the 2 diff buffers => activate differential updates. 
        _tft.setDiffGap(ILI8341_DIFFGAP);             // use a small gap for the diff buffers
        _tft.setRefreshRate(ILI9341_REFRESHRATE);     // around 120hz for the display refresh rate. 
        _tft.setVSyncSpacing(ILI9341_VSYNC_SPACING);  // set framerate = refreshrate/2 (and enable vsync at the same time).        
        _tft.setIRQPriority(ILI9341_IRQ_PRIORITY);    // set the IRQ priority for the display.
        //const int touch_calibration[4] = ILI9341_TOUCH_CALIBRATION; // touch calibration values
        //_tft.setTouchCalibration(touch_calibration); // set the touch calibration values

        digitalWrite(PIN_ILI9341_LED, LOW);
        pinMode(PIN_ILI9341_LED, OUTPUT);
        digitalWrite(PIN_ILI9341_LED, LOW);

        DEBUG("Display::init() completed.\n")

		}


    void update(bool force_full_redraw)
        {    
        if (_display_panic_mode) return; // in panic mode, only update_panic() can be used to update the display
        _tft.update((const uint16_t*)image_main.data(), force_full_redraw);
        }


    void update(const tgx::Image<tgx::RGB565> & im, bool force_full_redraw)
        {
        if (_display_panic_mode) return; // in panic mode, only update_panic() can be used to update the display
        _tft.update((const uint16_t*)im.data(), force_full_redraw);
        }




    void waitUpdateComplete()
        {
        while (_tft.asyncUpdateActive()) { threads.delay(1); }
        }


    void update_panic()
        {
        _tft.update((const uint16_t*)image_main.data(), true);
        }


    void updateRegion(tgx::iBox2 region, bool redraw_now)
        {
        if (_display_panic_mode) return; // in panic mode, only update_panic() can be used to update the display
        _tft.updateRegion(redraw_now, (const uint16_t*)&(image_main(region.minX, region.minY)), region.minX, region.maxX, region.minY, region.maxY, image_main.stride());
        _tft.waitUpdateAsyncComplete();
        }


    void backlight(bool enable)
        {
        digitalWrite(PIN_ILI9341_LED, enable ? HIGH : LOW);
        }


    SLOWFUN void displayToEXTMEM(bool enable)
        {
        if (enable)
            { // disable the internal framebuffer in DMAMEM
            _tft.waitUpdateAsyncComplete();
            _tft.setFramebuffer(Memory::framebuffer_main);

            }
        else
            { // move back to real double buffering with internal fb in DMAMEM
            _tft.waitUpdateAsyncComplete();
            _tft.setFramebuffers(Memory::dma_shared.A.framebuffer_internal);
            }
        }


    SLOWFUN void SetPanicMode()
        {
        _display_panic_mode = 1; 
        }


	}




/** end of file */