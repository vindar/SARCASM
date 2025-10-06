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

#include "BasicMoves.h"
#include "config.h"
#include "debug.h"
#include "memory.h"
#include "panic.h"
#include "DRV8834.h"
#include "Servos.h"
#include <TeensyThreads.h>
#include "teensythread.h"


namespace BasicMoves
    {


    // for the BasicMove thread 
    static volatile bool _th_basicmove_p1;
    static volatile bool _th_basicmove_p2;
    static volatile int _th_basicmove_status = 0; // 0 = completed, 1 = started, 2 = in progress
    static elapsedMillis _basicmove_thread_em;


    void _tumble_sub(bool disable_at_end)
        {
        Servos::Tumble::waitQueueEmpty();
        if (getPanicFlag()) return;

        // make sure alignement is correct
        DRV8834::alignCraddle();

        // make sure servo grip is disabled because otherwise moving the tumble may change current
        // in the other servo and affect its position
        if (!Servos::Grip::isDisabled())
            {
            Servos::Grip::pushCommand(Servos::SCom(SCOM_POS_DISABLE));
            Servos::Grip::waitQueueEmpty();
            }

        // make sure the tumble is in an admissible initial position. 
        if (Servos::Tumble::feedbackPos() < SERVO_TUMBLE_REST_MIN)
            {
            if (!Servos::isPanicAllowed())
                {
                Servos::Tumble::_setError(Servos::Tumble::feedbackPos(), -1);
                DEBUG("[NO PANIC] Trying to tumble() while the tumble position\n is over SERVO_TUMBLE_REST_MIN !");
                return;
                }
            else
                {
                ABORT("Trying to tumble() while the tumble position\n is over SERVO_TUMBLE_REST_MIN !");
                }
            }

        // move up fast until we (almost) touch the cube
        Servos::Tumble::pushCommand(Servos::SCom(SERVO_TUMBLE_CUBE_TOUCH, 0, 80, SCOM_FEEDBACK | SCOM_ENDCMD_STALL_AT_POS | SCOM_ENDCMD_STALL_AWAY), false);

        // move up slowly while lifting the cube
        const int nbup = (SERVO_TUMBLE_CUBE_TOUCH - SERVO_TUMBLE_CUBE_UPRIGHT) / SERVO_TUMBLE_CUBE_SPEED_UP;
        Servos::Tumble::pushCommand(Servos::SCom(SERVO_TUMBLE_CUBE_TOUCH, -SERVO_TUMBLE_CUBE_SPEED_UP, nbup, SCOM_FEEDBACK | SCOM_ENDCMD_STALL_AT_POS | SCOM_ENDCMD_STALL_AWAY), false);
        Servos::Tumble::pushCommand(Servos::SCom(SERVO_TUMBLE_CUBE_UPRIGHT, 0, 10, SCOM_FEEDBACK | SCOM_ENDCMD_STALL_AT_POS | SCOM_ENDCMD_STALL_AWAY), false);

        // go down fast
        const int nbdown = (SERVO_TUMBLE_REST_POS - SERVO_TUMBLE_CUBE_UPRIGHT) / SERVO_TUMBLE_CUBE_SPEED_DOWN;
        Servos::Tumble::pushCommand(Servos::SCom(SERVO_TUMBLE_CUBE_UPRIGHT, SERVO_TUMBLE_CUBE_SPEED_DOWN, nbdown, SCOM_FEEDBACK | SCOM_ENDCMD_STALL_AT_POS | SCOM_ENDCMD_STALL_AWAY), false);
        Servos::Tumble::pushCommand(Servos::SCom(SERVO_TUMBLE_REST_POS, 0, 50, SCOM_FEEDBACK | SCOM_DISABLE_STALL_AT_POS | SCOM_ENDCMD_STALL_AT_POS | SCOM_ENDCMD_STALL_AWAY), false);

        // disable if required
        if (disable_at_end)
            {
            Servos::Tumble::pushCommand(Servos::SCom(SCOM_POS_DISABLE));
            }
        }


    static void _tumble_thread()
        {
        _th_basicmove_status = 2;
        _tumble_sub(_th_basicmove_p1);
        _th_basicmove_status = 0;
        }


    void tumble_start(bool disable_at_end)
        {
        if (_th_basicmove_status != 0) { ABORT("tumble_start() : basicmove thread already in progress !"); }
        _th_basicmove_p1 = disable_at_end;
        _th_basicmove_status = 1;
        int th = threads.addThread(_tumble_thread, 0, BASICMOVES_STACK_SIZE, Memory::basicmoves_stack);
        if (th < 0) { ABORT("tumble_start() failed to start thread !"); }
        teensythread::speedUp(th, 0.3f); // thread does not need much time
        _basicmove_thread_em = 0; // reset the elapsed time
        }


    float tumble_progress()
        {
        if ((_th_basicmove_status != 0) || (Servos::Tumble::queueSize() > 0))
            { // estimate progress solely based on recorded time need for tumble.
            float r = ((int)_basicmove_thread_em) / ((float)(BASICMOVES_TUMBLE_TIME_MS));
            const float start_part = 0.25f; 
            const float first_part = 0.5f;
            const float second_part = 0.7f;
            if (r < start_part) return 0.0f; // first part is slower, so we scale it down to 0.5f
            if (r < first_part) return ((r -start_part) / (first_part - start_part)) * 0.5f; // first part is slower, so we scale it down to 0.5f
            if (r < second_part) return 0.5f; 
            r = 0.5f + 0.5f * (r - second_part) / (1.0f - second_part);
            return min( ((int)_basicmove_thread_em) / ((float)(BASICMOVES_TUMBLE_TIME_MS)) , 0.9999f);
            }
        return 1.0f;
        }

    
    void tumble(bool disable_at_end)
        {
        _tumble_sub(disable_at_end);
        // wait for the end of the move
        Servos::Tumble::waitQueueEmpty();
        }



    static volatile int _missing_cube = 0;
    static volatile int _previouspos = -1; // unknown

    static void _closeGrip_sub(bool use_previous_pos, bool disable_at_end)
        {
        Servos::Grip::waitQueueEmpty();
        if (getPanicFlag()) return;

        // make sure alignement is correct
        DRV8834::alignCraddle();

        // make sure servo tumble is disabled because otherwise moving the grip may change current 
        // in the other servo and affect its position/
        if (!Servos::Tumble::isDisabled())
            {
            Servos::Tumble::pushCommand(Servos::SCom(SCOM_POS_DISABLE));
            Servos::Tumble::waitQueueEmpty();
            }

        // make sure the grip is an admissible position. 
        if (Servos::Grip::feedbackPos() < SERVO_GRIP_REST_MIN)
            {
            // this check is not needed normally. even below the min should still work.            
            // ABORT("Trying closeGrip() while the grip position is under SERVO_GRIP_REST_MIN !");
            }

        if (!use_previous_pos) _previouspos = -1;
        if (_previouspos < 0)
            { // we need to detect the cube position

            // move fast to the minimum rest position
            Servos::Grip::pushCommand(Servos::SCom(SERVO_GRIP_REST_MIN, 0, 80, SCOM_FEEDBACK | SCOM_ENDCMD_STALL_AT_POS), false);

            // move slowly to detect stall position            
            const int SERVO_GRIP_SPEED_DETECT = 80;
            const int nbdet = (SERVO_GRIP_REST_MIN - SERVO_GRIP_MIN_PULSE) / SERVO_GRIP_SPEED_DETECT + 2;
            Servos::Grip::pushCommand(Servos::SCom(SCOM_POS_FEEDBACK, -SERVO_GRIP_SPEED_DETECT, nbdet, SCOM_FEEDBACK | SCOM_ENDCMD_STALL_AT_POS | SCOM_ENDCMD_STALL_AWAY, SCOM_NO_PANIC), false);
            Servos::Grip::pushCommand(Servos::SCom(SCOM_POS_FEEDBACK, 0, 5), false);
            Servos::Grip::waitQueueEmpty();

            // get the feedback position
            _previouspos = Servos::Grip::feedbackPos() + SERVO_GRIP_CUBE_TOUCH_OFF; // add a little offset to be sure we are not too close to the stall position
            if (_previouspos < SERVO_GRIP_CUBE_TOUCH_MIN)
                {
                _missing_cube = 1; 
                } 
            else if (_previouspos > SERVO_GRIP_CUBE_TOUCH_MAX)
                {
                if (!Servos::isPanicAllowed())
                    {
                    Servos::Grip::_setError(_previouspos, -1);
                    DEBUG("closeGrip() ; stall too high! (1)");
                    return;
                    } 
                else
                    {
                    ABORT("closeGrip() ; stall too high!");
                    }
                }
            }

        // go to the correct position
        Servos::Grip::pushCommand(Servos::SCom(_previouspos, 0, 80, SCOM_FEEDBACK | SCOM_ENDCMD_STALL_AWAY | SCOM_ENDCMD_STALL_AT_POS), false);

        if (disable_at_end)
            Servos::Grip::pushCommand(Servos::SCom(SCOM_POS_DISABLE));
        else
            Servos::Grip::pushCommand(Servos::SCom(_previouspos));
        }


    static void _closeGrip_thread()
        {
        _th_basicmove_status = 2;
        _closeGrip_sub(_th_basicmove_p1, _th_basicmove_p2);
        _th_basicmove_status = 0;
        }


    void closeGrip_start(bool use_previous_pos, bool disable_at_end)
        {
        if (_th_basicmove_status != 0) { ABORT("closeGrip_start() : basicmove thread already in progress !"); }
        _missing_cube = 0;
        _th_basicmove_p1 = use_previous_pos;
        _th_basicmove_p2 = disable_at_end;
        _th_basicmove_status = 1;
        int th = threads.addThread(_closeGrip_thread, 0, BASICMOVES_STACK_SIZE, Memory::basicmoves_stack);
        if (th < 0) { ABORT("closeGrip_start() failed to start thread !"); }
        teensythread::speedUp(th, 0.3f); // thread does not need much time
        _basicmove_thread_em = 0;
        }


    float closeGrip_progress()
        {
        if ((_th_basicmove_status != 0) || (Servos::Grip::queueSize() > 0))
            { // estimate progress solely based on recorded time need for tumble.
            return min(((int)_basicmove_thread_em) / ((float)(BASICMOVES_CLOSEGRIP_TIME_MS)), 0.9999f);
            }
        return 1.0f;
        }


    void closeGrip(bool use_previous_pos, bool disable_at_end)
        {
        _missing_cube = 0;
        _closeGrip_sub(use_previous_pos, disable_at_end);
        // wait for the end of the move
        Servos::Grip::waitQueueEmpty();
        }


    bool missingCube()
        {
        return (_missing_cube != 0);
        }





    void openGrip_start(bool store, bool disable_at_end)
        {
        Servos::Grip::waitQueueEmpty();
        if (getPanicFlag()) return;

        // the craddle motor driver must be enabled
        //DRV8834::alignCraddle();  // not needed

        // make sure servo tumble is disabled because otherwise moving the grip may change current 
        // in the other servo and affect its position
        if (!Servos::Tumble::isDisabled())
            {
            Servos::Tumble::pushCommand(Servos::SCom(SCOM_POS_DISABLE));
            Servos::Tumble::waitQueueEmpty();
            }

        // move to the rest position
        Servos::Grip::pushCommand(Servos::SCom(store ? SERVO_GRIP_STORE_POS : SERVO_GRIP_REST_POS, 0, 80, SCOM_FEEDBACK | SCOM_ENDCMD_STALL_AT_POS), false);
        Servos::Grip::pushCommand(Servos::SCom(SCOM_POS_FEEDBACK, 0, 5), false);
        if (disable_at_end) Servos::Grip::pushCommand(Servos::SCom(SCOM_POS_DISABLE));
        _basicmove_thread_em = 0;
        }


    float openGrip_progress()
        {
        if (Servos::Grip::queueSize() > 0)
            { // estimate progress solely based on recorded time need for tumble.
            return min(((int)_basicmove_thread_em) / ((float)(BASICMOVES_OPENGRIP_TIME_MS)), 0.9999f);
            }
        return 1.0f; 
        }


    void openGrip(bool store, bool disable_at_end)
        {
        openGrip_start(store, disable_at_end);
        // wait for the end of the move
        Servos::Grip::waitQueueEmpty();
        }





    void turn(int t)
        {
        turn_start(t);       
        DRV8834::waitTurnComplete();
        // make sure alignement is correct
        DRV8834::alignCraddle();
        }


    void turn_start(int t)
        {
        Servos::Grip::waitQueueEmpty();
        Servos::Tumble::waitQueueEmpty();
        if (getPanicFlag()) return;

        // make sure the tumble is in an admissible initial position. 
        if (Servos::Tumble::feedbackPos() < SERVO_TUMBLE_REST_MIN)
            {
            if (!Servos::isPanicAllowed())
                {
                Servos::Tumble::_setError(Servos::Tumble::feedbackPos(), -2);
                DEBUG("[NO PANIC] Trying to rotate() while the tumble position\n is over SERVO_TUMBLE_REST_MIN ! (2)");
                return;
                } 
            else
                {
                ABORT("Trying to rotate() while the tumble position\n is over SERVO_TUMBLE_REST_MIN !");
                }            
            }

        // make sure alignement is correct
        DRV8834::alignCraddle();

        const int N = t / DRV8834_CLOCKWISE_DIR;
        if (N == 0) return; // nothing to do
        if (N > 0)
            {
            if (N == 1)
                {
                DRV8834::turn(DRV8834_CLOCKWISE_DIR, 90 * N, -1.0f, DRV8834_TURN_MICROSTEPPING, DRV8843_TURN_FREQ_START_90DEG, DRV8843_TURN_FREQ_MAX_90DEG, DRV8843_TURN_ALPHA_90DEG);
                }
            else
                {
                DRV8834::turn(DRV8834_CLOCKWISE_DIR, 90 * N, -1.0f, DRV8834_TURN_MICROSTEPPING, DRV8843_TURN_FREQ_START_180DEG, DRV8843_TURN_FREQ_MAX_180DEG, DRV8843_TURN_ALPHA_180DEG);
                }            
            } 
        else
            {
            if (N == -1)
                {
                DRV8834::turn(DRV8834_ANTICLOCKWISE_DIR, -90 * N, -1.0f, DRV8834_TURN_MICROSTEPPING, DRV8843_TURN_FREQ_START_90DEG, DRV8843_TURN_FREQ_MAX_90DEG, DRV8843_TURN_ALPHA_90DEG);
                } else
                {
                DRV8834::turn(DRV8834_ANTICLOCKWISE_DIR, -90 * N, -1.0f, DRV8834_TURN_MICROSTEPPING, DRV8843_TURN_FREQ_START_180DEG, DRV8843_TURN_FREQ_MAX_180DEG, DRV8843_TURN_ALPHA_180DEG);
                }
            }
        }


    float turn_progress()
        {
        return DRV8834::ratio_turn_completed();
        }












    static void _align_thread()
        {
        _th_basicmove_status = 2;
        align();
        _th_basicmove_status = 0;
        }
    
    void align_start()
        {
        if (_th_basicmove_status != 0) { ABORT("align_start() : basicmove thread already in progress !"); }
        _th_basicmove_status = 1;
        int th = threads.addThread(_align_thread, 0, BASICMOVES_STACK_SIZE, Memory::basicmoves_stack);
        if (th < 0) { ABORT("align_start() failed to start thread !"); }
        teensythread::speedUp(th, 0.3f); // thread does not need much time
        }


    float align_progress()
        {
        if (_th_basicmove_status != 0)
            { 
            return 0.0f;
            }
        return 1.0f;
        }



    void align()
        {

        // make sure the tumble is in an admissible initial position. 
        if (Servos::Tumble::feedbackPos() < SERVO_TUMBLE_REST_MIN)
            {
            if (!Servos::isPanicAllowed())
                {
                Servos::Tumble::_setError(Servos::Tumble::feedbackPos(), -3);
                DEBUG("[NO PANIC] Trying to BasicMoves::align() while the tumble position\n is over SERVO_TUMBLE_REST_MIN (3)!");
                return;
                } 
            else
                {
                ABORT("Trying to BasicMoves::align() while the tumble position\n is over SERVO_TUMBLE_REST_MIN !");
                }
            }

        // make sure alignement is correct
        DRV8834::alignCraddle();
        }



    }


/** end of file */
