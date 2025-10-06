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
#include "Hardware.h"
#include "Config.h"

#include <Arduino.h>




/**
* Macros to print debug information to the serial ports.
* 
* - COUT()  : print to the serial port (SerialUSB, always available).
*
* - CIN()   : read from the serial port (SerialUSB, always available).
*
* - INFO()  : print to the 'debug' serial port (SerialUSB1, if available, otherwise do nothing).
*
* - DEBUG() : print an info line with file, line and function to the 'debug' serial port (SerialUSB1 if available, otherwise do nothing).
*
**/


namespace debug
    {







    template<typename X> struct Hex
        {
        Hex(const X& x) : _x(x) {}
        X _x;
        };



    class CoutStr : public Print
        {
        public:
            
            CoutStr() : _buffer{ 0 } {}
            
            template<typename T> CoutStr & operator<<(T v)
                {
                print(v); 
                return (*this);
                }


            void clear() 
                { 
                memset(_buffer, 0, sizeof(_buffer));
                }
        

            virtual size_t write(uint8_t c) override
                {
                if (strlen(_buffer) >= sizeof(_buffer) - 2) return 0; // no space left in the buffer
                _buffer[strlen(_buffer)] = c;
                _buffer[strlen(_buffer) + 1] = 0; // null terminate the string
                return 1;
                }


            const char * c_str() const
                {
                return _buffer;
                }

        private: 

            char _buffer[1024]; 
        };




    template<typename U> struct Cout
        {
            Cout() : _serial(nullptr) {}

            template<typename T>  SLOWFUN Cout& operator<<(const T& v)
                {
                if (_serial) _serial->print(v);
                return (*this);
                }


            template<typename X>  SLOWFUN Cout& operator<<(const Hex<X>& x)
                {
                if (_serial) _serial->print((uint32_t)x._x, HEX);
                return (*this);
                }

            void flush()
                {
                if (_serial) _serial->flush();
                }

            U* _serial;
        };


    /**
    * Initialize the serial port.
    * If true, wait for connection before returning
    **/
    SLOWFUN void init(bool wait_serial_ready = ENABLE_DEBUG_VIA_SERIAL);



    extern Cout<decltype(Serial)> _cout;

    #ifdef _DEBUG_SERIAL
    extern Cout<decltype(_DEBUG_SERIAL)> _debug_cout;
    #endif

    }



#define COUT(VV)  {debug::_cout << VV;}



#ifdef _DEBUG_SERIAL

    #define INFO(VV)  {debug::_debug_cout << VV;}

    #if ENABLE_DEBUG_VIA_SERIAL
        #define DEBUG(VV) {debug::_debug_cout << "[File:" << __FILE__ << " line:" << __LINE__ << "] " << VV << "\n"; debug::_debug_cout.flush(); }
    #else
        #define DEBUG(VV) {}
    #endif

#else

    #define INFO(VV)  {}

    #define DEBUG(VV) {}

#endif



/** end of file */


