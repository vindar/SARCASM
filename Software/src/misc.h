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

#include <Arduino.h>
#include "config.h"

namespace misc
    {


    /** convert a char to uppercase */
    inline char uppercase(char c)
        {
        if ((c >= 'a') && (c <= 'z')) { c -= 32; }
        return c;
        }

    /** convert a char to lowercase */
    inline char lowercase(char c)
        {
        if ((c >= 'A') && (c <= 'Z')) { c += 32; }
        return c;
        }


    /**
    * Format a text:
    * -> "test [] {} <>" is replaced by "test v1 v2 v3"
    **/
    void formatText(char* dst, const char* src, int v1, int v2 = 0, int v3 = 0);


    }

/** end of file */