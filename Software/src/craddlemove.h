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



namespace CraddleMove
    {


    /** method called from within the FBSensor interrupt to monitor the craddle movement */
    FASTRUN void craddlemove_int(float newpos, bool monitor);


    /** Return the current craddle rotation speed in degree per seconds.
     * Positive for clockwise and negative for anticlokwise turns */
    float rotationSpeed();


    /**
    * Return:
    * - 0 if the craddle is not moving or is being moved by the DRV8834 motor driver  
    * - 1 if the craddle is moving from the intervention ofthe user  
    * - 2 if the craddle has stopped moving recently (return only once then goes back to state 0)
    *
    **/
    int status();



    }



/** end of file */