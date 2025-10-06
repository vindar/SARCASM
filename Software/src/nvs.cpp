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

#include "nvs.h"
#include "debug.h"
#include "panic.h"

#include <Arduino.h>
#include <EEPROM.h>


namespace nvs
    {

    #define NVS_SIZE 4000


    uint8_t read8(const int index)
        {
        if (index < 0 || index >= NVS_SIZE) { ABORT("nvs::read8() index out of range"); }
        uint8_t value = EEPROM.read(index);
       // DEBUG("nvs::read8() index " << index << " value " << (int)value << "\n");
        return value;
        }

    void write8(const int index, const uint8_t value)
        {
        if (index < 0 || index >= NVS_SIZE) { ABORT("nvs::write8() index out of range"); }
        EEPROM.write(index, value);
        //DEBUG("nvs::write8() index " << index << " value " << (int)value << " written\n");
        }


    uint32_t read32(const int index)
        {
        if (index < 0 || index + 4 >= NVS_SIZE) { ABORT("nvs::read32() index out of range"); }
        uint32_t value = 0;
        for (int i = 0; i < 4; i++)
            {
            value |= (((uint32_t)(EEPROM.read(index + i))) << (i * 8));
            }
       // DEBUG("nvs::read32() index " << index << " value " << value << "\n");
        return value;
        }


    void write32(const int index, const uint32_t value)
        {
        if (index < 0 || index + 4 >= NVS_SIZE) { ABORT("nvs::write32() index out of range"); }
        for (int i = 0; i < 4; i++)
            {
            EEPROM.write(index + i, (value >> (i * 8)) & 0xFF);
            }
        //DEBUG("nvs::write32() index " << index << " value " << value << " written\n");
        }


    void readbuf(const int index, uint8_t* buf, const int size)
        {
        if (!buf) { ABORT("nvs::readbuf() null buffer"); }
        if (index < 0 || index + size > NVS_SIZE) { ABORT("nvs::readbuf() index out of range"); }
        for (int i = 0; i < size; i++)
            {
            buf[i] = EEPROM.read(index + i);
            }
        //DEBUG("nvs::readbuf() index " << index << " size " << size << " read\n");
        }


    void writebuf(const int index, const uint8_t* buf, const int size)
        {
        if (!buf) { ABORT("nvs::writebuf() null buffer"); }
        if (index < 0 || index + size > NVS_SIZE) { ABORT("nvs::writebuf() index out of range"); }
        for (int i = 0; i < size; i++)
            {
            EEPROM.write(index + i, buf[i]);
            }
       // DEBUG("nvs::writebuf() index " << index << " size " << size << " written\n");
        }




    }

/** end of file */


