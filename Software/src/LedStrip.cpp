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


#include "Hardware.h"
#include "Config.h"
#include "debug.h"
#include "memory.h"
#include "LedStrip.h"

#include <Arduino.h>
#include <OctoWS2811.h>

namespace LedStrip
{



	const uint8_t _pinled = PIN_LED;

	OctoWS2811 _leds(LED_COUNT, Memory::led_display_memory, Memory::led_drawing_memory, LED_CONFIG, 1, &_pinled);

	static bool _enable_led_audio = true;


	SLOWFUN void init()
		{		
		static int8_t _init_done = 0;
		if (_init_done) return;
		_init_done = 1;
		DEBUG("LedStrip::init() started")
		_enable_led_audio = true;
		setAllLed(0);
		_leds.begin();
		DEBUG("LedStrip::init() completed.\n")
		}



	SLOWFUN void setLed(int index, int color)
		{
		_leds.setPixel(index, color);
		}


	SLOWFUN void setAllLed(int color)
		{
		for (int i = 0; i < LED_COUNT; i++)
			{
			_leds.setPixel(i, color);
			}
		}

	
	SLOWFUN void setLed(int index, uint8_t R, uint8_t G, uint8_t B, uint8_t W)
		{
		_leds.setPixel(index, R, G, B, W);
		}


	SLOWFUN void setAllLed(uint8_t R, uint8_t G, uint8_t B, uint8_t W)
		{
		for (int i = 0; i < LED_COUNT; i++)
			{
			_leds.setPixel(i, R, G, B, W);
			}
		}


	SLOWFUN void show()
		{
		_leds.show();
		}


	SLOWFUN void setLedAudio(int index, uint8_t R, uint8_t G, uint8_t B, uint8_t W)
		{
        if (!_enable_led_audio) return; // if audio led is disabled, do nothing
        setLed(index, R, G, B, W);
		}


	SLOWFUN void enableLedAudio(bool enable)
		{
        _enable_led_audio = enable;
		}


	SLOWFUN void showAudio()
		{
        if (!_enable_led_audio) return; // if audio led is disabled, do nothing
        _leds.show();
		}


}


/** end of file */

