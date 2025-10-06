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

namespace nvs
    {

    /**
    * Read a byte from the non-volatile storage (NVS).
    **/
    uint8_t read8(const int index);

    /**
    * Write a byte to the non-volatile storage (NVS).
    **/
    void write8(const int index, const uint8_t value);


    /**
    * Read a 32-bit integer from the non-volatile storage (NVS).
    **/
    uint32_t read32(const int index);

    /**
    * Write a 32-bit integer to the non-volatile storage (NVS).
    **/
    void write32(const int index, const uint32_t value);


    /**
    * Read a buffer from the non-volatile storage (NVS).
    **/
    void readbuf(const int index, uint8_t* buf, const int size);

    /**
    * Write a buffer to the non-volatile storage (NVS).
    **/
    void writebuf(const int index, const uint8_t* buf, const int size);

    }


/** end of file */

