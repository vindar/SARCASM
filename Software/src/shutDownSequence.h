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
//

#pragma once




/**
* Start the shutdown sequence by speaking then playing a song and then shutting down.
* if there is no interaction from the user in between. 
* 
* If the method returns, the return value is:
*   CODE_TOUCHED_SCREEN if the user has touched the screen (a speach is ongoing, make a transition afterward).
*   CODE_SPINNED if the user has spun the craddle (no speach, launch cube resolution)
*   
* Change of charge status during the shutdown are ignored. (so the method never returns CODE_CHARGING_START or CODE_CHARGING_STOP).
*
* Return with cursor one line below the speech (CODE_TOUCHED or 0) or still speaking (CODE_SPINNED)*
**/
int shutDownSequence();

/** end of file */

