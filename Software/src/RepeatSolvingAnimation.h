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
#pragma once
#include "config.h"




/**
* Perform a 'repeat previous solve' Animation.
*
* Return:
*    0                    : animation completed correctly.
*    CODE_SPINNED         : the cradle was spinned
*    CODE_TOUCHED_SCREEN  : the screen was touched
*    CODE_CHARGING_START  : charging just started
*    CODE_CHARGING_STOP   : charging just stopped
*
* cursor in a the begining of line 1, below the tag [training] on line 0.
**/
SLOWFUN int repeatSolvingAnimation();


/**
* Return true if there is a previoous move sequence to replay. 
**/
SLOWFUN bool hasPreviousMovesSequence();

/** end of file */

