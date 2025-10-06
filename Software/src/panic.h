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

#include "Memory.h"



/**
* Start the panic interupt.
**/
void init_panic(); 


/**
* This method never returns
*/
void hang();


/**
* trigger panic mode
**/
void set_panic();

/**
* Query if panic mode is on.
**/
bool is_panic();


/**
* Set panic mode. This macro returns.
**/
#define PANIC(VV) { if (!is_panic()) { DEBUG(VV) Memory::panic_cout_extmem << VV; set_panic(); }}


/**
* Set panic mode. This macro never returns.
**/
#define ABORT(VV) { if (!is_panic()) { DEBUG(VV) Memory::panic_cout_extmem << VV; set_panic(); hang(); }}



/**
* Sert the panic flag  (but do nothing else, do not trigger panic mode)
* 0 = OK.
* 1 = Cradle panic  
* 2 = Servo Tumble panic  
* 3 = Servo Grip panic
**/
void setPanicFlag(int f);


/**
* Return the panic flag.
**/
int getPanicFlag();

