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

#include "teensythread.h"
#include "Hardware.h"
#include "config.h"
#include "Memory.h"
#include "debug.h"
#include "panic.h"

#include <Arduino.h>
#include <TeensyThreads.h>




namespace teensythread
    {


    SLOWFUN void speedUp(int th, float speedup_factor)
        {
        if (th < 0) { ABORT("threadSpeedUp() called with invalid thread id !"); }
        if (speedup_factor <= 0.0f) { ABORT("threadSpeedUp() called with invalid speedup factor !"); }
        int sl = TEENSYTHREADS_DEFAULT_TICKS * speedup_factor;
        if (sl < 1) { sl = 1; }
        if (sl > 100) { sl = 1000; }
        threads.setTimeSlice(th, sl); // set the time slice for this thread
        //DEBUG("Setting thread " << th << " time slice to " << sl << " ticks (speedup factor = " << speedup_factor << ")");
        return;
        }

    SLOWFUN void fillStack(void* stack, int len)
        {
        memset((uint8_t*)stack, 0x97, len); // fill the stack with zeros
        }


    SLOWFUN int  stackUsage(const void* stack, const int len)
        {
        uint8_t* volatile p = (uint8_t* volatile)stack;
        int i = 4;
        while ((i < len) && (p[i] == 0x97)) { i++; }
        return len - i; // return the number of bytes used in the stack
        }



    }


/** end of file */

