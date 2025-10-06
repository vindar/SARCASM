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

#include <stdint.h>



    /**
    * Very simple class that compute the CRC CCITT (16bit) 
    * of a data buffer.
    **/
    class CRC16
    {

    public:           

        CRC16()
            {
            reset();
            }


        /**
        * Reset the crc. 
        **/
        void reset()
            {
            _crc = 0xFFFF;
            }

        /**
        * Compute the crc for a given buffer.
        **/
        uint16_t ccitt(const uint8_t* data, size_t data_len)
            {
            reset();
            return ccitt_upd(data, data_len);
            }



        /**
        * Add additional data.
        **/
        uint16_t ccitt_upd(const uint8_t* data, size_t data_len)
            {
            for (size_t i = 0; i < data_len; ++i)
                {
                uint16_t dbyte = data[i];
                _crc ^= dbyte << 8;
                for (unsigned char j = 0; j < 8; ++j)
                    {
                    uint16_t mix = _crc & 0x8000;
                    _crc = (_crc << 1);
                    if (mix) _crc = _crc ^ 0x1021;
                    }
                }
            return _crc;
            }


        /**
        * Return the current value of the CRC. 
        **/
        operator uint16_t() const
            {
            return _crc;
            }



    private:

        uint16_t _crc;    
    };




/** end of file */

