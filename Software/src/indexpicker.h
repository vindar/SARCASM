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

#include <Arduino.h>
#include "config.h"
#include "nvs.h"
#include "debug.h"
#include "xorshift32.h"



/**
* Class used to pick a random index in a range, avoiding duplicates.
* 
* Pick random indices, avoiding the LEN last picked indices.
* State saved in NVS (non-volatile storage).
* 
* Used for chossing sentences. 
**/
template<int LEN> class IndexPicker
    {

    public:

        /**
        * Ctor (with delayed initialization).
        * 
        * size: the size of the range to pick from (0 to size-1).
        * nvs_index: the index in NVS where the state is saved (must be at least LEN bytes long).
        **/
        SLOWFUN  IndexPicker(int size, int nvs_index, const char* const tabstr[] = nullptr) : _size(size), _nvs_index(nvs_index), _tabstr(tabstr), _initdone(0)
            {
            }


        /**
        * Set the next value
        **/
        SLOWFUN void set(const uint8_t v)
            {
            _init();
            _tab[_pos] = v;
            nvs::write8(_nvs_index + _pos, v); // save the updated table
            _pos = (_pos + 1) % LEN; // wrap around
            nvs::write8(_nvs_index + _pos, 255); // mark the next starting index. 
            }


        /**
        * Return the next sentence
        **/
        SLOWFUN  const char* next_str()
            {
            return next_str(0, _size);
            }

        /**
        * Return the next random sentence in a given range [a,b[.
        **/
        SLOWFUN  const char* next_str(int a, int b)
            {
            if (_tabstr == nullptr)
                {
                DEBUG("IndexPicker::next_str() called with no tabstr set\n");
                delay(10000);
                return nullptr;
                }
            const uint8_t n = next(a, b);
            return _tabstr[n];
            }


        /**
        * Return the next random index in the full range, avoiding duplicates.
        **/
        SLOWFUN  uint8_t next()
            {
            return next(0, _size);
            }

        /**
        * Return the next random index in the range [a,b[, avoiding duplicates.
        **/
        SLOWFUN  uint8_t next(int a, int b)
            {
            _init();
            if ((a < 0) || (a > _size)) a = 0; 
            if ((b < 0) || (b > _size)) b = _size;
            if (a >= b) return a;
            int v = 0;
            if ((_n < a) || (_n>= b))
                {// normal mode
                _n = 255; // deactivate detemrinistic mode if needed.
                int found = 0;
                int nbr = 40; // try up to 40 times to find a new value
                while ((nbr-- > 0) && (found == 0))
                    {
                    v = rng::rand(a, b);
                    found = 1;
                    for (int k = 0; k < LEN; k++)
                        {
                        if (_tab[k] == v) { found = 0;  break; }
                        }
                    }
                if (nbr <= 0)
                    { // we did not find a new value, so just take the last one found
                    DEBUG("IndexPicker::next() WARNING: could not find a new value after 40 tries. Repeating value " << v << "\n");
                    }
                }
            else
                {// deterministic mode
                v = _n;
                _n = ((_n + 1) % _size);
                }
         //   DEBUG("IndexPicker::next() returning " << v << " from index " << _pos << "\n");
            set(v);
            return v;
            }


        /**
        * Reset the object, removing all data from EEPROM. 
        * Optionally set determinisitic mode at next launch. 
        **/
        void reset(bool deterministic_at_reset)
            {
            _init();
            for (int k = 1; k < LEN; k++)
                {
                _tab[k] = 255;
                nvs::write8(_nvs_index + k, 255);
                }
            nvs::write8(_nvs_index, (deterministic_at_reset ? 254 : 255)); // special reset code. 
            _tab[0] = 255;
            _pos = (deterministic_at_reset ? 1 : 0);
            _n = 255; // continue in normal mode
            }


        /**
        * Return the number of elements in the picker
        **/
        int size() const 
            {
            return _size;
            }


    private: 



        SLOWFUN void _init()
            {
            if (_initdone != 0) return;
            _initdone = 1;
            nvs::readbuf(_nvs_index, _tab, LEN);
            _pos = 0; 
            if (_tab[0] == 254)
                { 
                _n = 0; // use deterministic mode
                DEBUG("IndexPicker [" << _nvs_index << "] using deterministic mode\n");
                }
            else
                { 
                _n = 255; // normal mode
                while (_pos < LEN)
                    { // try to find a good index to start
                    if (_tab[_pos] == 255) { break; } // unused value found.
                    _pos++;
                    }
                if (_pos >= LEN) { _pos = 0; } // all values used, start from the beginning
                DEBUG("IndexPicker [" << _nvs_index << "] normal mode, starting at index " << _pos << "\n");
                }

            /*
            Serial.printf("IndexPicker[%d] with LEN=%d, _n=%d, _pos=%d\n->", (int)_nvs_index, (int)LEN, (int)_n, (int)_pos);
            for (int k = 0; k < LEN; k++)
                {
                Serial.printf("%d ", _tab[k]);
                }
            Serial.print("\n\n");
            */

            }

     
        const int _size;
        const int _nvs_index;
        int _pos;

        const char* const* _tabstr;
        uint8_t _tab[LEN];
        uint8_t _n; 
        uint8_t _initdone;
    };


/** end of file */