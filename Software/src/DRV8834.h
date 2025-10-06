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


/***********************************************************************************
*
* Using the stepper motor (for cube rotation) controlled via a pololu DRV8834 
* driver module.
*
************************************************************************************/

namespace DRV8834
{


    /**
    * Initialize the stepper motor driver.
    * Must be called first before using any function below. 
    **/
    SLOWFUN void init();


    /**
    * Set the microstepping mode for the DRV8834 stepper motor driver.
    *
    * microstep must be one of 1,2,4,8,16,32. Other values are ignored.
    **/
    SLOWFUN void setMicrostepping(int microstep);


    /**
    * Activate/desactive the driver (i.e. power on/off the coils on the motor).
    * 
    * Monitoring of the craddle position (i.e. stall/panic watchdog) starts as soon as 
    * the driver is active and stops as soon a the coils are off. 
    **/
    SLOWFUN void enable(bool enable);


    /**
    * Called by panic() to disable the stepper motor driver in case of emergency.
    **/
    SLOWFUN void panic_off();


    /**
    * Return true if the driver is enabled (i.e. the coils are on).
    **/
    bool isEnabled();



    /**
    *  Turn for a given number of steps
    *  Return only when all steps are performed. 
    *
    * @param   dir         The direction of rotation, either DRV8834_CLOCKWISE_DIR or DRV8834_ANTICLOCKWISE_DIR
    * @param   steps       Number of steps to perform .
    * @param   freq        (Optional) The frequency.
    * @param   microstep   (Optional) The microstepping value (one of 1,2,4,8,16,32).
    *
    **/
    SLOWFUN void makeSteps(int dir, int steps, float freq = DRV8843_STEPS_FREQ, float microstep = DRV8834_STEPS_MICROSTEPPING);


    /**
     * Start a turn with a given angle and overshoot. If a turn is already in progress, wait for it
     * to finish before starting this one.
     *
     * @param   dir             Direction of rotation, either DRV8834_CLOCKWISE_DIR or DRV8834_ANTICLOCKWISE_DIR.
     * @param   angle           Angle of rotation in degrees.
     * @param   overshoot_angle (Optional) The overshoot angle, in degrees.
     * @param   microstep       (Optional) The microstepping value (one of 1,2,4,8,16,32).
     * @param   freq_start      (Optional) The start frequency of the turn.
     * @param   freq_max        (Optional) The max frequency of the turn.
     * @param   alpha_ramp      (Optional) ratio of time spent accelerating/decelerating (must be
     *                          between 0.0f and 0.5f).
     * @param   continuous_spin (Optional) If true the cradle will keep spinning indefinitely until 
     *                          stopContinuousSpin() is called and then it will stop as soon as possible
     *                          while insuring stopping after an integer number of full turns more than
     *                          what was requested.
     *                          
     * @returns (approximate) number of microseconds before the end of the turn.
     */
    SLOWFUN int turn(int dir, float angle, float overshoot_angle = DRV8834_DEFAULT_OVERSHOOT_ANGLE, int microstep = DRV8834_STEPS_MICROSTEPPING, float freq_start = DRV8843_TURN_FREQ_START_DEFAULT, float freq_max = DRV8843_TURN_FREQ_MAX_DEFAULT, float alpha_ramp = DRV8843_TURN_ALPHA_DEFAULT, bool continuous_spin = false);


    /** 
     * Stops continuous spin mode (if it is on). 
     **/
    SLOWFUN void stopContinuousSpin();


    /**
    * Return how much turn angle (in degrees) has been done in the turn in progress. 
    * (may be more than the requested angle because of the overshoot).
    **/
    float current_turn_angle();


    /**
    * Return true if a 'fast' turn is in progress. 
    **/
    bool fastTurnOn();

    /**
    * Return the number of microseconds remaining before completion of the current turn.
    * 
    * Return 0 if and only if no turn is in progress.
    **/
    SLOWFUN int time_until_turn_completion();


    /**
    * Return the ratio completed of the turn in progress.
    **/
    SLOWFUN float ratio_turn_completed();


    /**
    * Return true is the turn has ended. 
    **/
    SLOWFUN bool isTurnComplete();


    /**
    * Wait until the current turn (if any in progress) is completed.
    **/
    SLOWFUN void waitTurnComplete();



    /**
    * Align the craddle with the closest of the 4 cardinal points (0, 90, 180, 270)
    * This method return as soon as alignment is correct. 
    **/
    SLOWFUN void alignCraddle();


    /**
    * Return the real position of the craddle in degrees in [0,360[
    **/
    float craddlePos();


    /**
    * Dirty but efficient calibration routine for the cradle stepper motor.
    * 1. Open Serial.
    * 2. Put the cradle by had at angle 0deg
    * 2. Press key and each note note the values at other cardinal angles.  
    *    (make more than one turn and average to get best value)
    **/
    SLOWFUN void cradleCalibration();


    /**
    * Set the craddle 'theoretical' pos in degree in [0,360[ or NO_CRADDLE_POS to unset.
    * 
    * - When the cradle position is set, it is compared at each polling (by FBSensors interrupt)
    *   of craddle the position to detect a possible stall in which case panic is triggered.
    * - When unset (ie NO_CRADDLE_POS), no checking is performed in FBSensors interrput.  
    * 
    * FOR SPECIAL USE.
    **/
    void _setCraddleTheoreticalPos(float pos = CRADDLE_NO_POS);


    /**
    * Set the craddle theoretical position as the current position.
    * 
    * FOR SPECIAL USE.
    **/
    void _setCraddleTheoreticalPosCurrent();


    /**
    * Return the craddle 'theoretical' position or NO_CRADDLE_POS if not set.
    * - use by FBSensors interrupt to compare with current position and detect a stall.
    * 
    * FOR SPECIAL USE.
    **/
    float _getCraddleTheoreticalPos();


    /** return the(smallest) angle in degree that should be added to go from a to b
    * can be positive or negative
    * FOR SPECIAL USE.
    **/
    float _angle_sub(float a, float b);


    /**
    * Set to true to enable triggering panic mode when the cradle is monitored (ie powered on)
    * and a position anomaly is detected.
    *
    * If set to false, panic mode will not be trigerred but power to the cradle and the servo is still cut.
    **/
    void allowPanic(bool enable);


    /**
    * Check whether panic mode if on.
    **/
    bool isPanicAllowed();


    /**
    * Check a position anomaly has been detected on the cradle while being monitored.
    *
    * Return 0.0f if no anomaly has been detected.
    * Return the error in degree if an anomaly has been detected.
    **/
    float cradleError();


    /**
    * For internal use by FBSensors.
    * Set the current position error of the craddle.
    **/
    void _setCraddleError(float error);


    }


/** end of file */

