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

#include "Hardware.h"
#include "config.h"



/**
* All the methods below are threadsafe and return immediatly so they can be called from anywhere. 
* 
* They return the last value read from the sensor (which is polled by the periodic interrupt) 
**/

namespace FBSensors
    {


    /**
    * Initialize the feedback sensors.
    * Starts the periodical interrupts that polls the sensors.
    *
    * @param   polling_period_microseconds period of the polling timer.
    *
    **/
    SLOWFUN void init(int polling_period_microseconds = FBSENSORS_PERIOD_US);


    /**
    * Return the value of the counter which tracks how many time polling occured.
    **/
    int count();


    /**
    * Wait until a new measurmeent of the Grip/Tumble/Craddle sensor is completed. 
    * !!! Do not call this method in an interrupt with higher priority than the polling timer !!!
    **/
    SLOWFUN void waitForMeasurement();


    /**
    * Return the battery voltage, in V.
    * Can be queried from anywhere, returns immediatly, read from ADC.
    **/
    float getVoltage();

    /**
    * Return the battery charge percentage, in % in [0,100].
    * ! beware, this value is computed from the voltage
    *   so it only works when the battery is NOT charging !
    **/
    int getBatteryPercent();

    /**
    * Return the charging voltage (at the USB port), in V, read from the INA219 sensor.
    * Can be queried from anywhere, returns immediatly.
    **/
    float getChargingVoltage();


    /**
    * Return the current charging status:
    * CODE_NO_CHARGING : not charging
    * CODE_CHARGING : charging
    * CODE_CHARGING_START : recently stopped charging.
    * CODE_CHARGING_STOP : recently began charging.
    **/
    int chargeStatus();


    /**
    * As chargeStatus() but does not affect the next call. 
    **/
    int peekChargeStatus();

    /**
    * Return the charging current (at the USB) , in A, read from the INA219 sensor.
    * Can be queried from anywhere, returns immediatly.
    **/
    float getChargingCurrent();


    /**
    * Return the raw value of the grip servo feedback sensor, read from ADC.
    **/
    int getServoGripRawPos();


    /**
    * Return the raw value of the tumble servo feedback sensor, read from ADC
    **/
    int getServoTumbleRawPos();


    /**
    * Return the raw value for the position of the craddle read from the AS5600 sensor.
    **/
    int getCraddleRawPos();


}


/** end of file */


