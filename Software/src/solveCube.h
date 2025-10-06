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
* Solve the cube (main routine).
* Returns: 
*   - CODE_SPINNED : the cradle was spinned again during the end speech.   
*   - 0 : ok, resolution complete. 
* 
* Sets in cube difficulty:
* - CODE_CUBE_MISSING: no cube in cradle
* - CODE_CUBE_INVALID: invalid cube
* - CODE_CUBE_ALREADY_SOLVED: cube already solved  
* - CODE_CUBE_TRIVIAL: trivial solve  
* - CODE_CUBE_MODERATE: easy solve
* - CODE_CUBE_FULL: full solve  
*  
* Set the number of moves performed in nb_moves (so puts 0 if no cube or invalid cube or missing cube).
**/
int solveCube(int& cube_difficulty, int& nb_moves);


/** end of file */