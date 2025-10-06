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
#include "FBSensors.h"
#include "PowerCtrl.h"
#include "LedStrip.h"
#include "panic.h"

#include <Arduino.h>





extern "C" uint32_t set_arm_clock(uint32_t frequency);	



namespace PowerCtrl
{

	SLOWFUN void init()
		{	
		static int8_t _init_done = 0; 
		if (_init_done) return;
		_init_done = 1;

		debug::init(); // just in case
		DEBUG("PowerCtrl::init() started")
		FBSensors::init(); // just in case, this take care of the voltage feedback initialization.

		pinMode(PIN_MAIN_PWR_CTRL, OUTPUT);

		DEBUG("PowerCtrl::init() completed.\n")
		}


	SLOWFUN void powerOff()
		{
		digitalWrite(PIN_MAIN_PWR_CTRL, HIGH);
		}


	SLOWFUN void overclock(int mode)
		{
		switch (mode)
			{
			case 0: 
				{
				set_arm_clock(24000000); // 24Mhz,power saving
				return;
				}
			case 2:
				{
				set_arm_clock(816000000); // 816Mhz, mpax perf. 
				return;
				}
			}
		set_arm_clock(600000000); // 600Mhz, default cpu clock
		}



}

/** end of file */

