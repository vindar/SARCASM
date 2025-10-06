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

#include "config.h" 


namespace PowerCtrl
{


    /**
     * Intialization
     **/
    SLOWFUN void init();


    /**
     * Power off everything by shutting down the Pololu switch button.
     **/
    SLOWFUN void powerOff();


    /**
     * Set the CPU clock.
     * - 0 = underclocking (power saving),
     * - 1 = normal (default)
     * - 2 = overclock (max perf.)
     **/
    SLOWFUN void overclock(int mode = 1);


}

/** end of file */

