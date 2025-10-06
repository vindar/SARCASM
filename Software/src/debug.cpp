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

#include "config.h"
#include "debug.h"

#include <TeensyThreads.h>


namespace debug 
    { 
        
        Cout<decltype(Serial)> _cout; 

#ifdef _DEBUG_SERIAL
        Cout<decltype(_DEBUG_SERIAL)> _debug_cout;
#endif



        SLOWFUN void init(bool wait_serial_ready)
            {
            static int8_t _init_done = 0;
            if (_init_done) return;
            _init_done = 1;

            if (wait_serial_ready)
                { // we must wait for all the serial to be open. 
             // while (!Serial) { threads.delay(1); }
#ifdef _DEBUG_SERIAL
                while (!_DEBUG_SERIAL) { threads.delay(1); }
#endif      
                }
            Serial.begin(2000000); // speed does not matter and is ignored. 
            _cout._serial = &Serial;
#ifdef _DEBUG_SERIAL
            _DEBUG_SERIAL.begin(2000000); // speed does not matter and is ignored.
            _debug_cout._serial = &_DEBUG_SERIAL;
#endif      
            }

    }



/** end of file */

