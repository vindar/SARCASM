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

#include "DRV8834.h"
#include "panic.h"

#include <math.h>
#include <TeensyTimerTool.h>
#include <TeensyThreads.h>


namespace DRV8834
{


  

    SLOWFUN void cradleCalibration()
        {
        Serial.println("Put the cradle at angle 0\nthen press key to power up");
        while (Serial.read() <= 0)
            {
            Serial.println(FBSensors::getCraddleRawPos());
            delay(200);
            }
        Serial.println("Testing");
        DRV8834::enable(true);
        int d = 0;
        while (1)
            {
            Serial.printf("\nPosition %d deg: ", d*90);
            do {
                delay(500);
                Serial.print(FBSensors::getCraddleRawPos());
                Serial.print(" ");
                } while (Serial.read() <= 0);
            DRV8834::turn(1, 90, 0);
            d++;
            if (d == 4) { d = 0; }
            }
        }






    float craddlePos()
        {
        const int r = FBSensors::getCraddleRawPos();
        if (r < DRV8834_AS5600_OFFSET_0DEG)
            {
            const int u = r + (4096 - DRV8834_AS5600_OFFSET_270DEG);
            const int l = DRV8834_AS5600_OFFSET_0DEG + (4096 - DRV8834_AS5600_OFFSET_270DEG);
            return 270.0f + ((float)u) * (90.0f / ((float)l));
            }
        if (r < DRV8834_AS5600_OFFSET_90DEG)
            {
            const int u = r - DRV8834_AS5600_OFFSET_0DEG;
            const int l = DRV8834_AS5600_OFFSET_90DEG - DRV8834_AS5600_OFFSET_0DEG;
            return ((float)u) * (90.0f / ((float)l));
            }
        if (r < DRV8834_AS5600_OFFSET_180DEG)
            {
            const int u = r - DRV8834_AS5600_OFFSET_90DEG;
            const int l = DRV8834_AS5600_OFFSET_180DEG - DRV8834_AS5600_OFFSET_90DEG;
            return 90.0f +  ((float)u) * (90.0f / ((float)l));
            }
        if (r < DRV8834_AS5600_OFFSET_270DEG)
            {
            const int u = r - DRV8834_AS5600_OFFSET_180DEG;
            const int l = DRV8834_AS5600_OFFSET_270DEG - DRV8834_AS5600_OFFSET_180DEG;
            return 180.0f + ((float)u) * (90.0f / ((float)l));
            }
        const int u = r - DRV8834_AS5600_OFFSET_270DEG;
        const int l = DRV8834_AS5600_OFFSET_0DEG + (4096 - DRV8834_AS5600_OFFSET_270DEG);
        return 270.0f + ((float)u) * (90.0f / ((float)l));
        }



    #define CRADDLE_THEORETICAL_FACTOR 10000

    static volatile int _craddle_theoretical_pos = CRADDLE_NO_POS * CRADDLE_THEORETICAL_FACTOR; // use int instead of float for atomic op. 

    //static volatile float _craddle_theoretical_pos = CRADDLE_NO_POS;

    void _setCraddleTheoreticalPos(float pos)
        {
        //_craddle_theoretical_pos = pos;
        _craddle_theoretical_pos = (int)(pos * CRADDLE_THEORETICAL_FACTOR);
        }


    void _setCraddleTheoreticalPosCurrent()
        {
        _setCraddleTheoreticalPos(craddlePos());
        }


    float _getCraddleTheoreticalPos()
        {
        //return _craddle_theoretical_pos;
        const int r = _craddle_theoretical_pos;
        return ((float)r) / CRADDLE_THEORETICAL_FACTOR;
        }


    float _angle_sub(float a, float b)
        {
        a = fmodf(a, 360.0f);
        b = fmodf(b, 360.0f);
        if (a < 0) a += 360.0f; // ensure a is in [0,360[
        if (b < 0) b += 360.0f; // ensure b is in [0,360[

        if (a <= b)
            {
            const float d = b - a;
            return (d < 180) ? d : (d - 360);
            }
        const float d = a - b;
        return -((d < 180) ? d : (d - 360));
        }


    volatile bool _isEnabled = false;

    SLOWFUN void init()
        {
        static int8_t _init_done = 0;
        if (_init_done) return;
        _init_done = 1;

        allowPanic(true); // panic enabled by default

        _isEnabled = false;

        debug::init(); // just in case
        DEBUG("DRV8834::init() started")

        FBSensors::init(); // just in case, this take care of the feedback initialization.

        digitalWrite(PIN_DRV8834_SLEEP, LOW);
        pinMode(PIN_DRV8834_SLEEP, OUTPUT);

        pinMode(PIN_DRV8834_STEP, OUTPUT);
        digitalWrite(PIN_DRV8834_STEP, LOW);

        pinMode(PIN_DRV8834_DIR, OUTPUT);
        digitalWrite(PIN_DRV8834_DIR, LOW);

        setMicrostepping(DRV8834_STEPS_MICROSTEPPING);

        DEBUG("DRV8834::init() completed\n")
        }



    SLOWFUN void setMicrostepping(int microstep)
        {
        switch (microstep)
            {
        case 1:
            pinMode(PIN_DRV8834_M0, OUTPUT);
            digitalWrite(PIN_DRV8834_M0, LOW);
            pinMode(PIN_DRV8834_M1, OUTPUT);
            digitalWrite(PIN_DRV8834_M1, LOW);
            break;
        case 2:
            pinMode(PIN_DRV8834_M0, OUTPUT);
            digitalWrite(PIN_DRV8834_M0, HIGH);
            pinMode(PIN_DRV8834_M1, OUTPUT);
            digitalWrite(PIN_DRV8834_M1, LOW);
            break;
        case 4:
            pinMode(PIN_DRV8834_M0, INPUT_DISABLE);            
            pinMode(PIN_DRV8834_M1, OUTPUT);
            digitalWrite(PIN_DRV8834_M1, LOW);
            break;

        case 8:
            pinMode(PIN_DRV8834_M0, OUTPUT);
            digitalWrite(PIN_DRV8834_M0, LOW);
            pinMode(PIN_DRV8834_M1, OUTPUT);
            digitalWrite(PIN_DRV8834_M1, HIGH);
            break;
        case 16:
            pinMode(PIN_DRV8834_M0, OUTPUT);
            digitalWrite(PIN_DRV8834_M0, HIGH);
            pinMode(PIN_DRV8834_M1, OUTPUT);
            digitalWrite(PIN_DRV8834_M1, HIGH);
            break;
        case 32:
            pinMode(PIN_DRV8834_M0, INPUT_DISABLE);
            pinMode(PIN_DRV8834_M1, OUTPUT);
            digitalWrite(PIN_DRV8834_M1, HIGH);
            break;
            }
        delayMicroseconds(DRV8834_DELAY_COMMAND_US); // wait for driver to register change (1ms according to datasheet, we take some margin). 
        }


    SLOWFUN void panic_off()
        {
        digitalWrite(PIN_DRV8834_SLEEP, false);
        }


    SLOWFUN void enable(bool enable) 
        { 
        if (_isEnabled == enable) return; // nothing to do
        _isEnabled = enable;
        if (enable == false)
            {
            _setCraddleTheoreticalPos(CRADDLE_NO_POS); // stop monitoring position
            threads.delay(DRV8834_WAIT_BEFORE_DISABLE_MS); // wait for motor to immobilize
            }
        digitalWrite(PIN_DRV8834_SLEEP, enable); 
        delayMicroseconds(DRV8834_DELAY_ENABLE_US);
        if (enable == true)
            {
            threads.delay(DRV8834_WAIT_AFTER_ENABLE_MS); // wait for motor to stabilize after power on before monitoring
            _setCraddleTheoreticalPosCurrent();  // start monotoring position 
            }
        }


    bool isEnabled()
        {
        return _isEnabled;
        }


    SLOWFUN void makeSteps(int dir, int steps, float freq, float microstep)
        {
        digitalWrite(PIN_DRV8834_DIR, (dir > 0));   // set the direction       
        setMicrostepping(microstep);                // set microstepping mode (and wait until dir and microstep are registered). 
        const float angle_per_step = 360.0f / ((float)MOTOR_STEPS_PER_TURN * microstep);
        _setCraddleTheoreticalPosCurrent();  // reset the theoretical pos
        const float startpos = _getCraddleTheoreticalPos(); //and save it
        const int T = (int)(500000.0f / freq);      // half period
        for (int k = 0; k < steps; k++)
            {
            digitalWrite(PIN_DRV8834_STEP, HIGH);
            delayMicroseconds(T);
            digitalWrite(PIN_DRV8834_STEP, LOW);
            delayMicroseconds(T);
            _setCraddleTheoreticalPos(startpos + (k * angle_per_step * dir));
            }
        _setCraddleTheoreticalPosCurrent(); // reset the theoretical position
        }



    volatile int _turn_in_progress;  // 0 = no turn in progress, 1 = turn in progress
    volatile int _turn_dir;          // direction of the turn

    volatile float _alpha_ramp;        // proportion of time spent in acceleration (same time spent in deceleration)

    volatile float _angle_per_step;    // angle rotation (in degree) per step

    volatile int _nb_step_ramp;      // number of turns during acceleration and deceleration
    volatile int _nb_step_flat;      // number of turns at constant max speed
    volatile int _nb_step_overshoot; // number of turns during overshoot (in the forward direction and same number in the opposite direction)

    volatile int _additional_steps;  // number of steps added in the middle (for continuous spin)
    volatile int _continuous_spin;   // 0 = no continuous spin, 1 = continuous spin
    volatile int _nb_steps_per_turn; // number of steps per turn (depends on microstepping mode)

    volatile float _freq_start;        // start frequency
    volatile float _freq_max;          // frequency when at maximum speed
    volatile float _freq_inc;          // amount of frequency increase at each step during acceleration (and same at deceleration)  (_freq_max - freq_start)/_nb_step_ramp

    volatile float _current_freq;          // current frequency
    volatile int   _current_step;          // number of steps done
    volatile int   _current_half_period;   // current duration of a half period (in us)
    volatile int   _current_period_phase;  // 0 = first half period and 1 = second half period

    volatile float _theoretical_angle_start; // theoretical angle at the start of the turn

    TeensyTimerTool::PeriodicTimer _turn_timer(DRV8834_TIMER_NAME);



    FASTRUN void turn_timer_cb()
        {
        if (_current_period_phase == 0)
            { // end of first half period
            _current_period_phase = 1;
            digitalWrite(PIN_DRV8834_STEP, LOW);
            if (_current_step < _nb_step_ramp)
                { // increasing
                _current_freq += _freq_inc; 
                _current_half_period = (int)(500000.0f / _current_freq);
                _turn_timer.setPeriod(_current_half_period);
                }
            else
                {
                const int d = _nb_step_ramp + _nb_step_flat;
                if ((_current_step >= d) && (_current_step < d + _nb_step_ramp))
                    { // decreasing
                    _current_freq -= _freq_inc;
                    _current_half_period = (int)(500000.0f / _current_freq);
                    _turn_timer.setPeriod(_current_half_period);
                    }
                }              
            return;
            } 
        // end of full period
        _current_period_phase = 0;


        if (_current_step == (_nb_step_ramp + _nb_step_flat - 1))
            { // we are in the locking step in continuous spin
            if (_continuous_spin == 1)
                {
                _additional_steps++; 
                if (_additional_steps == _nb_steps_per_turn) { _additional_steps = 0; }
                }
            else
                {
                if (_additional_steps == 0)
                    {
                    _current_step++;
                    }
                else
                    { 
                    _additional_steps++;
                    if (_additional_steps == _nb_steps_per_turn) { _additional_steps = 0; }
                    }
                }
            }
        else
            {
            _current_step++;
            }

        const int f = (2*_nb_step_ramp) + _nb_step_flat + _nb_step_overshoot;
        if (_current_step >= f + _nb_step_overshoot)
            { // end of turn
            _turn_in_progress = 0;
            _turn_timer.stop();
            _turn_timer.end();
            return;
            }
        if (_current_step == f)
            { 
            digitalWrite(PIN_DRV8834_DIR, (_turn_dir < 0));   // change direction   
            }        
        digitalWrite(PIN_DRV8834_STEP, HIGH); // start a new step

        const float tp = _theoretical_angle_start;
        if (tp != CRADDLE_NO_POS)
            { // we monitor the angle so we save the new theoretical position. 
            _setCraddleTheoreticalPos(tp + current_turn_angle()*_turn_dir);
            }
            
        }




    SLOWFUN bool fastTurnOn()
        {
        return (_freq_max > 20000);
        }


    SLOWFUN void waitTurnComplete()
        {
        while (_turn_in_progress != 0) 
            { 
            threads.yield(); 
            if (getPanicFlag() != 0) return; // panic detected !
            }
        if (_getCraddleTheoreticalPos() != CRADDLE_NO_POS)
            { // we monitor the angle so we save the new theoretical position. 
            _setCraddleTheoreticalPosCurrent();
            }
        }


    SLOWFUN void stopContinuousSpin()
        {
        _continuous_spin = 0;
        }


    SLOWFUN int turn(int dir, float angle, float overshoot_angle, int microstep, float freq_start, float freq_max, float alpha_ramp, bool continuous_spin)
        {
        waitTurnComplete(); // in case a turn is already in progress. 

        if (overshoot_angle < 0) overshoot_angle = (dir == DRV8834_CLOCKWISE_DIR) ? DRV8834_CLOCKWISE_OVERSHOOT_ANGLE : DRV8834_ANTICLOCKWISE_OVERSHOOT_ANGLE;
         
        _turn_in_progress = 1; 
        _turn_dir = dir;
        _freq_start = freq_start;
        _freq_max = freq_max;
        _alpha_ramp = (alpha_ramp < 0.5f) ? alpha_ramp : 0.5f;
        _nb_steps_per_turn = MOTOR_STEPS_PER_TURN * microstep;
        _angle_per_step = 360.0f / ((float)_nb_steps_per_turn);
        
        _additional_steps = 0;
        _continuous_spin = (continuous_spin) ? 1 : 0;

        const int nbsteps = (int)(angle / _angle_per_step); // number of step to perform a rotation of angle.     
        if (nbsteps < 3)
            { // case to treat separately
            makeSteps(dir, nbsteps);   // use default values for freq and microstep
            _turn_in_progress = 0;
            return 0;
            }

        digitalWrite(PIN_DRV8834_DIR, (dir > 0));       // set the forward direction       
        setMicrostepping(microstep);                    // set microstepping mode (and wait until dir and microstep are registered). 

        // compute the number of steps during acceleration 
        const float MIN_FREQ_DIFF = 100;
        const float C = (_freq_max - freq_start < MIN_FREQ_DIFF) ? 1 : (1 - 2 * _alpha_ramp) * (_freq_max * log(_freq_max / _freq_start)) / (_freq_max - freq_start) + 2*_alpha_ramp;
        _nb_step_ramp = (_alpha_ramp * nbsteps) / ((C <=1) ? 1 : C); 
        if (_nb_step_ramp == 0) _nb_step_ramp = 1;

        // number of step at max speed
        _nb_step_flat = nbsteps - 2*_nb_step_ramp;  
        if (_nb_step_flat == 0) { _nb_step_ramp--; _nb_step_flat = 2; }

        // number of spet at min speed during overshoot
        _nb_step_overshoot = (int)(overshoot_angle / _angle_per_step);

        // compute the frequency increment
        _freq_inc = (_freq_max - _freq_start) / _nb_step_ramp;

        _current_freq = _freq_start;
        _current_step = 0;
        _current_half_period = (int)(500000.0f / _freq_start);
        _current_period_phase = 0;
              
        /*
        DEBUG("_turn_in_progress " << _turn_in_progress << "\n");
        DEBUG("_turn_dir " << _turn_dir << "\n");
        DEBUG("_alpha_ramp " << _alpha_ramp << "\n");
        DEBUG("_angle_per_step " << _angle_per_step << "\n");
        DEBUG("_nb_step_ramp " << _nb_step_ramp << "\n");
        DEBUG("_nb_step_flat " << _nb_step_flat << "\n");
        DEBUG("_nb_step_overshoot " << _nb_step_overshoot << "\n");
        DEBUG("_freq_start " << _freq_start << "\n");
        DEBUG("_freq_max " << _freq_max << "\n");
        DEBUG("_freq_inc " << _freq_inc << "\n");
        DEBUG("_current_freq " << _current_freq << "\n");
        DEBUG("_current_step " << _current_step << "\n");
        DEBUG("_current_half_period " << _current_half_period << "\n");
        DEBUG("_current_period_phase " << _current_period_phase << "\n");
        */

        _setCraddleTheoreticalPosCurrent(); // reset the theretical position 
        _theoretical_angle_start = _getCraddleTheoreticalPos(); // and save it

        digitalWrite(PIN_DRV8834_STEP, HIGH);

        _turn_timer.begin(turn_timer_cb, _current_half_period);
        NVIC_SET_PRIORITY(DRV8834_IRQ_NAME, DRV8834_IRQ_PRIORITY);

        return time_until_turn_completion();
        }



    float current_turn_angle()
        {
        if (_current_step < 2 * _nb_step_ramp + _nb_step_flat + _nb_step_overshoot)
            {
            return (_current_step + _additional_steps) * _angle_per_step;
            } 
        else
            {
            return (2 * (2*_nb_step_ramp + _nb_step_flat + _nb_step_overshoot) - _current_step) * _angle_per_step;
            }
        }
    

    SLOWFUN bool isTurnComplete()
        {
        if (_turn_in_progress == 0)
            {
            if (_getCraddleTheoreticalPos() != CRADDLE_NO_POS)
                { // we monitor the angle so we save the new theoretical position. 
                _setCraddleTheoreticalPosCurrent();
                }
            return true;
            }
        return false;
        }


    SLOWFUN float ratio_turn_completed()
        {
        if (_turn_in_progress == 0) return 1.0f;
        const int tots = 2 * _nb_step_ramp + _nb_step_flat + 2 * _nb_step_overshoot;
        return ((float)(_current_step)) / ((float)tots); // ratio of steps done over total steps        
        }



    SLOWFUN int time_until_turn_completion()
        {
        if (_turn_in_progress == 0) return 0;
        int tot = 0;
        if (_current_step < _nb_step_ramp)
            { // add remaining acceleration time
            const int r = _nb_step_ramp - _current_step;
            const int f = _freq_max - r * _freq_inc;            
            tot += (_freq_inc == 0) ? r * ((int)(1000000.0f / _freq_max))
                                    : (1000000.0f / _freq_inc) * log(1 + r*_freq_inc/f) 
                          //          - 1000000.0f * r/(2*(f/_freq_inc)*(f/_freq_inc + r))
                                    - r/2;            
            }
        if (_current_step < _nb_step_ramp + _nb_step_flat)
            { // add remaining flat time
            const int r = min(_nb_step_ramp + _nb_step_flat - _current_step, _nb_step_flat);
            tot += r*((int)(1000000.0f / _freq_max));
            }
        if (_current_step < 2*_nb_step_ramp + _nb_step_flat)
            { // add remaining deceleration time
            const int r = min(2*_nb_step_ramp + _nb_step_flat - _current_step, _nb_step_ramp);
            const int f = _freq_start;
            
            tot += (_freq_inc == 0) ? r * ((int)(1000000.0f / _freq_max)) 
                                    : (1000000.0f / _freq_inc) * log(1 + r*_freq_inc/f) 
                        //            - 1000000.0f * r/(2*(f/_freq_inc)*(f/_freq_inc + r)) 
                                    - r/2;
            }
        if (_current_step < 2*_nb_step_ramp + _nb_step_flat + 2*_nb_step_overshoot)
            { // add remaining overshoot time
            const int r = min(2*_nb_step_ramp + _nb_step_flat + 2*_nb_step_overshoot - _current_step, 2*_nb_step_overshoot);
            tot += r * ((int)(1000000.0f / _freq_start));
            // tot -= (2 * _nb_step_ramp + _nb_step_flat + 2 * _nb_step_overshoot - _current_step)*0.5f; // deduce 0.5us for each step..  
            }
        if (_turn_in_progress == 0) return 0;   // check again      
        return (tot == 0) ? 1 : tot; 
        }






    SLOWFUN void alignCraddle()
        {
        if (!DRV8834::isEnabled()) return;

        float olda = 1000.0f;  // previous pos
        int nb_same = 0;
        int nbt = 0;
        while (1)
            {
            nbt++;
            const float a = DRV8834::craddlePos();
            if (olda == a) nb_same++; else nb_same = 0;
            olda = a;
            DRV8834::_setCraddleTheoreticalPosCurrent();
            float b;
            if ((a > 90 - 45) && (a <= 90 + 45)) b = 90;
            else if ((a > 180 - 45) && (a <= 180 + 45)) b = 180;
            else if ((a > 270 - 45) && (a <= 270 + 45)) b = 270;
            else b = 0;
            const float turn_angle = DRV8834::_angle_sub(a, b);
            float d = abs(turn_angle);
            if (d > DRV8834_AS5600_ANGLE_ERROR)
                {
                d -= (DRV8834_AS5600_ANGLE_ERROR / 3)*(1 - nb_same);
                DRV8834::turn(((turn_angle > 0) ? DRV8834_CLOCKWISE_DIR : DRV8834_ANTICLOCKWISE_DIR), d, 0);
                DRV8834::waitTurnComplete();
                threads.delay(10); // wait for the craddle to settle (importan, toherwise it will oscillate)
                } 
            else
                {
                DEBUG("Craddle aligned in [" << nbt << "] : " << DRV8834::craddlePos());
                DRV8834::_setCraddleTheoreticalPosCurrent();
                return;
                }
            }
        }




    static volatile bool _allow_panic = true;
    static volatile float _cradle_error = 0.0f;


    void allowPanic(bool enable)
        {
        _cradle_error = 0.0f;
        _allow_panic = enable;
        }


    bool isPanicAllowed()
        {
        return _allow_panic;
        }


    float cradleError()
        {
        return _cradle_error;
        }


    void _setCraddleError(float error)
        {
        _cradle_error = error;
        setPanicFlag(1);
        }




}




/** end of file */