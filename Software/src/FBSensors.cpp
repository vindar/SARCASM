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

#include "Hardware.h"
#include "config.h"
#include "debug.h"
#include "FBSensors.h"
#include "panic.h"
#include "DRV8834.h"
#include "servos.h"
#include "craddlemove.h"
#include "ESPCAM.h"

#include <Arduino.h>
#include <ADC.h>
#include <Wire.h>
#include <INA219.h>
#include <AS5600.h>
#include <TeensyTimerTool.h>
#include <TeensyThreads.h>


#if (FBSENSORS_ADJUST_TIMER_FOR_OVERCLOCK > 0)
#define FBSENSORS_OVERCLOCK_MULTIPLIER(x) ((((int32_t)x)*(F_BUS_ACTUAL/1000000))/150)
#else
#define FBSENSORS_OVERCLOCK_MULTIPLIER(x) (x)
#endif


namespace FBSensors
    {

    TeensyTimerTool::PeriodicTimer _fbsensorsTimer(FBSENSORS_TIMER_NAME);
    

    ADC      _adc;                              // object for both ADC0 and ADC1

    INA219   _ina219(FBSENSORS_INA219_ADRESS);  // the charging current/voltage sensor (on the USB port). 

    AMS_5600 _ams5600;                          // the magnetic sensor for the craddle position.

    static volatile int _voltage = -1;
    static volatile int _charging_voltage = -1;
    static volatile int _charging_current = -1;
    static volatile int _servo_grip_raw_pos = -1;
    static volatile int _servo_tumble_raw_pos = -1;
    static volatile int _craddle_raw_pos = -1;
    static volatile int _count = 0;
    static volatile int _skip = 0;
    static volatile int _outvoltagesamples = 0; // number of samples outside the voltage range.
    static volatile int _nb_panic_craddle = 0; // number of consecutive measures where the craddle position is out of range.

    static volatile int _charging_status = CODE_NO_CHARGING;
    elapsedMillis _charging_status_timer;


    int count() { return _count; }


    SLOWFUN void waitForMeasurement()
        {
        const int c = _count; 
        while (c == _count) { threads.delay(1); }
        }

    float getVoltage() 
        { 
        static const float VMULT = (3.3f / (1 << FBSENSORS_ADC_RESOLUTION)) * (33.0f / 10.0f); // voltage divider with 23K and 10K resistors
        return VMULT * _voltage + 0.05f; // add small correction of 5mV
        }

    float getChargingVoltage() 
        { 
        return _charging_voltage * 0.001f; 
        }



    /** convert battery voltage to charge percent.*/
    static float voltageToBatteryPercent(float voltage)
        {
        float v = voltage / 2.0f; // voltage per cell
        if (voltage >= 8.20f) return 100.0f;
        if (voltage <= 6.20f) return 0.0f;
        struct { float v; float p; } lut[] = {
            {4.10f, 100.0f},
            {4.05f,  95.0f},
            {4.00f,  85.0f},
            {3.95f,  75.0f},
            {3.90f,  65.0f},
            {3.85f,  55.0f},
            {3.80f,  45.0f},
            {3.75f,  35.0f},
            {3.70f,  25.0f},
            {3.60f,  12.0f},
            {3.50f,   6.0f},
            {3.40f,   3.0f},
            {3.30f,   1.0f},
            {3.10f,   0.0f}
            };
        // interpolation
        for (int i = 0; i < (int)(sizeof(lut) / sizeof(lut[0])) - 1; i++)
            {
            if (v >= lut[i + 1].v && v <= lut[i].v)
                {
                float dv = lut[i].v - lut[i + 1].v;
                float dp = lut[i].p - lut[i + 1].p;
                float ratio = (v - lut[i + 1].v) / dv;
                return lut[i + 1].p + ratio * dp;
                }
            }
        return 0.0f; // just in case.
        }


    int getBatteryPercent()
        {
        return voltageToBatteryPercent(getVoltage());
        }



    float getChargingCurrent() 
        { 
        return _charging_current * 0.001f; 
        }

    int getServoGripRawPos() { return _servo_grip_raw_pos; }

    int getServoTumbleRawPos() { return _servo_tumble_raw_pos; }

    int getCraddleRawPos() { return _craddle_raw_pos; }



    void _startADCRead(int pin, int adc)
        {
        if (adc == 0)
            {
            _adc.adc0->startSingleRead(pin);
            } 
        else
            {
            _adc.adc1->startSingleRead(pin);
            }
        }


    int _endADCRead(int adc)
        {
        if (adc == 0)
            {
            while (_adc.adc0->isConverting()) {}
            return (_adc.adc0->isComplete()) ? _adc.adc0->readSingle() : -1;
            } 
        while (_adc.adc1->isConverting()) {}
        return (_adc.adc1->isComplete()) ? _adc.adc1->readSingle() : -1;
        }



    int chargeStatus()
        {
        const int st = _charging_status;
        if (st == CODE_CHARGING_STOP) { _charging_status = CODE_NO_CHARGING; }
        if (st == CODE_CHARGING_START) { _charging_status = CODE_CHARGING; }
        //Serial.print("Charge status = "); Serial.println(st);
        return st;
        }

    
    int peekChargeStatus()
        {
        return _charging_status;
        }


    /**   
    * The sensor feedback periodic timer interrupt.
    * 
    * Runs in:
    * 
    * - 50us normally  
    * - 100us when polling also the main voltage/usb volatage/current. 
    **/
    FASTRUN void _sensor_int()
        {
        /*
        // For debbugging purpose : check the duration of the interrupt.
        static int _mxt = 0;
        elapsedMicros emus = 0; 
        */

        // start reading the servo feeback voltages (both at the same time)
        _startADCRead(PIN_SERVO_TUMBLE_FB, SERVO_TUMBLE_ADC);
        _startADCRead(PIN_SERVO_GRIP_FB, SERVO_GRIP_ADC);

        // while reading the ADC, poll the craddle position sensor.
        _craddle_raw_pos = _ams5600.getRawAngle();
        const float tp = DRV8834::_getCraddleTheoreticalPos();
        const float p = DRV8834::craddlePos();
        int new_nb_panic_craddle = 0; 
        if (tp != CRADDLE_NO_POS)
            { // we must monitor the angle
            const float d = abs(DRV8834::_angle_sub(tp, p));
            if (d > DRV8834_AS5600_ANGLE_PANIC)
                {
                if ((!DRV8834::fastTurnOn())||(d > DRV8834_AS5600_ANGLE_PANIC_FAST_MOVE))
                    {
                    new_nb_panic_craddle = _nb_panic_craddle + 1;
                    }
                }
            if (new_nb_panic_craddle >= DRV8834_AS5600_NB_PANIC_MEASURES)
                {
                if (DRV8834::isPanicAllowed())
                    {
                    PANIC("Craddle position out of range\nposition = " << p << "\nexpected = " << tp << "\ndifference = " << d);
                    }
                else
                    {
                    if (getPanicFlag() == 0)
                        {
                        DRV8834::_setCraddleError(d); // set the panic flag, disable stepper and servos and register the error.
                        DEBUG("[NO PANIC] Craddle position out of range\nposition = " << p << "\nexpected = " << tp << "\ndifference = " << d);
                        }
                    }
                }
            }
        _nb_panic_craddle = new_nb_panic_craddle;

        CraddleMove::craddlemove_int(p, tp == CRADDLE_NO_POS); // call the method for advanced monitoring of the craddle movement

        // finish reading the ADCs
        int tpos = _endADCRead(SERVO_TUMBLE_ADC);
        if (tpos >= 0) _servo_tumble_raw_pos = tpos;
        int gpos = _endADCRead(SERVO_GRIP_ADC);
        if (gpos >= 0) _servo_grip_raw_pos = gpos;

        ++_skip;
     
        if (_skip == (FBSENSORS_NBSKIP_VOLTAGE/2))
            { // time to read charging voltage. 
            _charging_voltage = (int)_ina219.getBusVoltage_mV();
            }
        else if (_skip == FBSENSORS_NBSKIP_VOLTAGE)
            { // time to read battery voltage and charging current. 
            _startADCRead(PIN_BAT_VOLTAGE_FB, VOLTAGE_ADC);
            _charging_current = (int)_ina219.getCurrent_mA();

            if (_charging_current < (int)(100*CHARGING_CURRENT_SLOW) )
                { 
                if (((_charging_status == CODE_CHARGING) || (_charging_status == CODE_CHARGING_START)) && (_charging_status_timer > CHARGING_STATUS_TIMEOUT))
                    {
                    _charging_status = CODE_CHARGING_STOP;
                    _charging_status_timer = 0;
                    }
                else if ((_charging_status == CODE_CHARGING_STOP)&&(_charging_status_timer > CHARGING_STATUS_TIMEOUT))
                    { 
                    _charging_status = CODE_NO_CHARGING;
                    _charging_status_timer = 0; 
                    }
                }
            else 
                { // charging
                if (((_charging_status == CODE_NO_CHARGING) || (_charging_status == CODE_CHARGING_STOP)) && (_charging_status_timer > CHARGING_STATUS_TIMEOUT))
                    {
                    _charging_status = CODE_CHARGING_START;
                    _charging_status_timer = 0;
                    } 
                else if ((_charging_status == CODE_CHARGING_START) && (_charging_status_timer > CHARGING_STATUS_TIMEOUT)) 
                    { 
                    _charging_status = CODE_CHARGING; 
                    _charging_status_timer = 0; 
                    }
                }           
            _voltage = _endADCRead(VOLTAGE_ADC);
            _skip = 0;
            float V = getVoltage();
            _outvoltagesamples = ((V > CUTOFF_VOLTAGE_MAX) || (V < CUTOFF_VOLTAGE_MIN)) ? (_outvoltagesamples + 1) : 0;
            if (_outvoltagesamples >= VOLTAGE_PANIC_SAMPLES)
                {
                if (V > CUTOFF_VOLTAGE_MAX) PANIC("Voltage too high") else PANIC("Voltage too low")
                }
            }
        _count++; // increment the counter now that we are done.


        if (_skip == 1)
            {
            // check for powercycling of the ESPCAM
            ESPCAM::checkPowerCycling();
            }

        /*
        // For debbugging purpose : check the duration of the interrupt. 
        const int y = (int)emus;
        if (y > _mxt)
            {
            _mxt = y;
            Serial.print("Max time = ");
            Serial.println(y);
            }
        */
        return;
        }









    SLOWFUN void init(int polling_period_microseconds)
        {
        static int8_t _init_done = 0;
        if (_init_done) return;
        _init_done = 1;

        debug::init(); // just in case
        DEBUG("FBSensors::init() started")


        pinMode(PIN_BAT_VOLTAGE_FB, INPUT_DISABLE);
        pinMode(PIN_SERVO_TUMBLE_FB, INPUT_DISABLE);
        pinMode(PIN_SERVO_GRIP_FB, INPUT_DISABLE);

        // set the ADCs
        _adc.adc0->setAveraging(FBSENSORS_ADC_AVERAGING);
        _adc.adc0->setResolution(FBSENSORS_ADC_RESOLUTION);
        _adc.adc0->setConversionSpeed(FBSENSORS_ADC_CONVERSION_SPEED);
        _adc.adc0->setSamplingSpeed(FBSENSORS_ADC_SAMPLING_SPEED);
        _adc.adc0->disableInterrupts();
        _adc.adc0->calibrate();
        _adc.adc0->wait_for_cal();

        _adc.adc1->setAveraging(FBSENSORS_ADC_AVERAGING);
        _adc.adc1->setResolution(FBSENSORS_ADC_RESOLUTION);
        _adc.adc1->setConversionSpeed(FBSENSORS_ADC_CONVERSION_SPEED);
        _adc.adc1->setSamplingSpeed(FBSENSORS_ADC_SAMPLING_SPEED);
        _adc.adc1->disableInterrupts();
        _adc.adc1->calibrate();
        _adc.adc1->wait_for_cal();

        // set the I2C bus
        Wire.begin();
        Wire.setClock(FBSENSORS_I2C_SPI_SPEED); 

        // init the INA219 sensor
        if (!_ina219.begin())
            { 
            ABORT("Error: cannot initialize the INA219 Sensor")
            }
        _ina219.setMaxCurrentShunt(FBSENSORS_INA219_MAXCURRENT, FBSENSORS_INA219_SHUNTRESISTOR);
        _ina219.setShuntResolution(FBSENSORS_INA219_RESOLUTION);
        // TODO : check if changing resolution really ionvrease polling time. 


        // init the AS5600 sensor
        if (_ams5600.detectMagnet() == 0)
            {
            ABORT("Error: cannot detect magnet of the AS5600 sensor")
            }
        // TODO : change filter speed. 
        
        _adc.adc0->wait_for_cal();
        _adc.adc1->wait_for_cal();

        // start the timer
        _fbsensorsTimer.begin(_sensor_int, FBSENSORS_OVERCLOCK_MULTIPLIER(FBSENSORS_PERIOD_US));
        NVIC_SET_PRIORITY(FBSENSORS_IRQ_NAME, FBSENSORS_IRQ_PRIORITY);

        DEBUG("FBSensors::init() completed\n")
        }


    }



/** end of file */