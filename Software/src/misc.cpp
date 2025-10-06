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

#include <Arduino.h>
#include <TeensyThreads.h>
#include "config.h"
#include "misc.h"

namespace misc
    {


    SLOWFUN static void _convint(char*& dst, int val)
        {
        if (val < 0) { dst[0] = '-'; dst++; val = -val; }
        if (val == 0) { dst[0] = '0'; dst++; return; }
        int n = 1;
        while (val >= n) { n *= 10; }
        n /= 10;
        while (n > 0)
            {
            const int d = val / n;
            dst[0] = '0' + d;
            dst++;
            val = val - (d * n);
            n /= 10;
            }
        }


    SLOWFUN void formatText(char* dst, const char* src, int v1, int v2, int v3)
        {
        while (src[0])
            {
            if ((src[0] == '[') && (src[1] == ']'))
                {
                _convint(dst, v1);
                src += 2;
                } else if ((src[0] == '{') && (src[1] == '}'))
                    {
                    _convint(dst, v2);
                    src += 2;
                    } else if ((src[0] == '<') && (src[1] == '>'))
                        {
                        _convint(dst, v3);
                        src += 2;
                        } else
                        {
                        dst[0] = src[0];
                        dst++;
                        src++;
                        }
            }
        dst[0] = 0; // null terminate
        }


    }



/** end of file */