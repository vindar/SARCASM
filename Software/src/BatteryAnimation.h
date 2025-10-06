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



/**
* Perform an 'battery' animation.
* 
* code: CODE_CHARGING_START or CODE_CHARGING_STOP or 0 (to display current Battery status)
*
* Return:
*    0                   : animation completed correctly.
*    CODE_SPINNED        : the cradle was spinned. may be speaking.
*    CODE_TOUCHED_SCREEN : the screen was touched. may be speaking.
* 
* Change of charge status during the animation are ignored. (so the method never returns CODE_CHARGING_START or CODE_CHARGING_STOP).
* 
* Return with cursor one line below the speech (CODE_TOUCHED or 0) or still speaking (CODE_SPINNED)
**/
int batteryAnimation(int code, bool clearConsole);




/** end of file */