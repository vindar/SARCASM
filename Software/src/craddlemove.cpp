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
// along with this code. If not, see  <http://www.gnu.org/licenses/>.


#include "config.h"
#include "craddlemove.h"
#include "DRV8834.h"
#include <Arduino.h>


namespace CraddleMove
    {

    volatile float _startpos; 
    volatile float _prevpos; 
    volatile float _prevspeed;
    volatile int _status = -1; // -1 not monitored, 0: not moving, 1: moving, 2: stopped recently
    volatile int _count;
    volatile float _largest_angle;



#define CRADDLE_MOVE_MIN_ANGLE 60.0f

#define CRADDLE_MOVE_COUNT_MOVE 2
#define CRADDLE_MOVE_MIN_SPEED_MOVE 210.0f

#define CRADDLE_MOVE_COUNT_STOP 3
#define CRADDLE_MOVE_MAX_SPEED_STOP 10.0f

#define CRADDLE_MOVE_STATUS_2_DURATION_MS 250 


    void craddlemove_int(float newpos, bool monitor)
        {
        static elapsedMicros _el;
        static elapsedMillis _el2;
        if (!monitor)
            { // stop monotoring
            _status = -1;
            return;
            }
        // we monitor
        if (_status == -1)
            { // first time we monitor
            _status = 0;
            _prevpos = newpos;
            _prevspeed = 0.0f;
            _count = 0; 
            _el = 0;
            return;
            } 
        // we are already monitoring        
        _prevspeed = DRV8834::_angle_sub(_prevpos, newpos) * (1000000.0f / _el); // new speed in degree per second           
        _prevpos = newpos; // new position
        _el = 0;
        if (_status == 0)
            {
            if (abs(_prevspeed) >= CRADDLE_MOVE_MIN_SPEED_MOVE) _count++; else _count = 0;
            if (_count >= CRADDLE_MOVE_COUNT_MOVE)
                {
                _status = 1; // moving
                _count = 0;
                _largest_angle = 0;
                _startpos = newpos;
                }
            return;
            }
        if (_status == 1)
            {
            if (_largest_angle < CRADDLE_MOVE_MIN_ANGLE)
                {
                float d = abs(DRV8834::_angle_sub(_startpos, newpos));
                if (d > _largest_angle) _largest_angle = d;
                }
            if (abs(_prevspeed) <= CRADDLE_MOVE_MAX_SPEED_STOP) _count++; else _count = 0;
            if (_count >= CRADDLE_MOVE_COUNT_STOP)
                {
                if (_largest_angle >= CRADDLE_MOVE_MIN_ANGLE)
                    {
                    _status = 2; // stopped
                    _el2 = 0;
                    } 
                else
                    {
                    _status = -1; // back to not moving
                    }                                                        
                }
            return;
            }
        if (_status == 2)
            {
            if (_el2 >= CRADDLE_MOVE_STATUS_2_DURATION_MS)
                {
                _status = -1; // back to not moving
                }
            }
        }


    float rotationSpeed()
        {
        if (_status == -1) return 0.0f; 
        return _prevspeed;
        }




    int status()
        {
        if (_status == -1) return 0;
        if (_status == 2)
            {
            _status = -1; 
            return 2;
            }
        return _status;
        }




    }

/** end of file */