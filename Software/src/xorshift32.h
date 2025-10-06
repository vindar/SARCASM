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

#include <stdint.h>
#include "config.h"
#include "debug.h"
#include <Arduino.h>
#include <TeensyThreads.h>



namespace rng
    {

    /**
    * Init the RNG with a seed based on the current time and a stored value in NVS
    **/
    SLOWFUN void initRNG();


    /**
    * RNG will use seed RESET_RNG_SEED next time initRNG() is called (at next startup).
    **/
    SLOWFUN void resetRNGnextTime();


    /**
    * Reseed the generator.
    **/
    SLOWFUN void reseed(uint32_t seed = 0);


    /**
    * Return a random uint32_t number.
    **/
    SLOWFUN uint32_t rand();


    /**
    * Return uniform number in [0,1[
    **/
    SLOWFUN float unif();


    /**
    * Return a random integer in [0,b[
    **/
    SLOWFUN int rand(int b);


    /**
    * Return a random integer in [a,b[
    **/
    SLOWFUN int rand(int a, int b);

    }


/** end of file */
