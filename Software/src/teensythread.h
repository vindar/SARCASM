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


namespace teensythread
    {
    /**
     * Speed up a given thread by a given factor compared to normal speed.
     * Call this without any arguments to reset the speed to normal.
     **/
    void speedUp(int th, float speedup_factor = 1.0f);

    
    /**
    * Mark the stack before running the thread.
    **/
    void fillStack(void* stack, int len);
        

    /**
    * Estimate the maxium stack usage of the thread.
    **/
    int stackUsage(const void* stack, const int len);

    }

/* end of files */