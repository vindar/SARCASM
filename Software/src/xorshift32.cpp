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


#include "xorshift32.h"
#include <Arduino.h>
#include <stdint.h>
#include <TeensyThreads.h>
#include "nvs.h"



namespace rng
    {

    static uint32_t _state; // must be non zero


    SLOWFUN void initRNG()
        {
        static int initdone = 0;
        if (initdone) return;
        initdone = 1;
        const uint32_t s1 = nvs::read32(NVS_ADR_PREV_SEED);
        const uint32_t s2 = micros();
        if (s1 == 0)
            { // use deterministic seed
            _state = RESET_RNG_SEED;
            nvs::write32(NVS_ADR_PREV_SEED, s2); // next time use real random seed. 
            DEBUG("RNG Seed : " << _state << " [RESET]");
            }
        else
            {
            _state = s1 + s2; // do not use xor (^) becaue s1 may be equal to s2. 
            if (_state == 0) _state = micros(); // must be non zero
            nvs::write32(NVS_ADR_PREV_SEED, _state); // store the seed for next time
            DEBUG("RNG Seed : " << _state);
            }        
        }


    SLOWFUN void resetRNGnextTime()
        {
        nvs::write32(NVS_ADR_PREV_SEED, 0); // next time use deterministic seed
        }


    SLOWFUN void reseed(uint32_t seed)
        {
        if (seed == 0) seed = (uint32_t)micros();
        _state = seed;
        }


    SLOWFUN uint32_t rand()
        { // Algorithm "xor" from p. 4 of Marsaglia, "Xorshift RNGs"
        _state ^= _state << 13;
        _state ^= _state >> 17;
        _state ^= _state << 5;
        return _state;
        }


    SLOWFUN float unif()
        {
        const float e = 1.0f / float(0x1000000);
        return (rand() >> 8) * e;
        }


    SLOWFUN int rand(int b)
        {
        if (b <= 0) return 0;
        return int((uint64_t(b) * uint64_t(rand())) >> 32);
        }


    SLOWFUN int rand(int a, int b)
        {
        if (b <= a) return a;
        return a + rand(b - a);
        }

    }


/** end of file */
