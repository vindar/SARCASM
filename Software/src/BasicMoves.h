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

#pragma once


namespace BasicMoves
    {


    /******************************************************************************************
    * TUMBLING THE CUBE
    *******************************************************************************************/

    /**
    * Start tumbling the cube
    **/
    void tumble_start(bool disable_at_end = true);

    /**
    * Return the current tumble progress between 0.0 and 1.0f.
    * Return 1.0f when tumbling is completed / off. 
    **/
    float tumble_progress();


    /**
    * Tumble the cube. 
    * set 'disable_at_end' to disable the servo at the end of the move. Otherwise, it stays on.
    * 
    * Return when the operation has completed. 
    **/
    void tumble(bool disable_at_end = true);


    /******************************************************************************************
    * OPENING AND CLOSING THE GRIP
    *******************************************************************************************/

    /**
    * Start closing the grip.
    * true to use the previous position, otherwise, trie to detect the stalling position.
    *
    * Return when the operation has completed.
    **/
    void closeGrip_start(bool use_previous_pos = true, bool disable_at_end = true);


    /**
    * Estimate the progress of the grip closing operation.
    **/
    float closeGrip_progress();


    /**
    * Close the grip, 
    * true to use the previous position, otherwise, trie to detect the stalling position. 
    * 
    * Return when the operation has completed.
    **/
    void closeGrip(bool use_previous_pos= true, bool disable_at_end = true);


    /**
    * Return true if the missing cube flag is set
    * 
    * This flag is reset when the grip is closed and set if the closing goes too far below.
    **/
    bool missingCube();



    /**
    * Start opening the grip.
    * set 'disable_at_end' to disable the servo at the end of the move. Otherwise, it stays on.
    * set 'store' to move the grip to SERVO_GRIP_STORE_POS (i.e. the position where the cube is stored).
    * otherwise, it is only moved to SERVO_GRIP_REST_POS.
    *
    * Return when the operation has completed.
    **/
    void openGrip_start(bool store = false, bool disable_at_end = true);


    /**
    * Estimate the progress of the grip opening operation.
    **/
    float openGrip_progress();


    /**
    * Open the grip. 
    * set 'disable_at_end' to disable the servo at the end of the move. Otherwise, it stays on.
    * set 'store' to move the grip to SERVO_GRIP_STORE_POS (i.e. the position where the cube is stored).
    * otherwise, it is only moved to SERVO_GRIP_REST_POS.
    * 
    * Return when the operation has completed.
    **/
    void openGrip(bool store = false, bool disable_at_end = true);



    /******************************************************************************************
    * TURNING THE CRADDLE
    *******************************************************************************************/


    /**
    * Start aligning the craddle.
    **/
    void align_start();


    /**
    * Estimate the progress of the craddle alignment operation
    * Return 1.0f when craddle is correctly aligned. 
    **/
    float align_progress();


    /**
    * Align the craddle
    * Return when alignment completed.
    **/
    void align();



    /**
     * Start rotating the craddle.
     *
     * Return when the operation has completed.
     *
     * @param   t    N*DRV8834_CLOCKWISE_DIR, N*DRV8834_ANTICLOCKWISE_DIR where N is the number of quarter turns to perform.
     */
    void turn_start(int t);

    /**
     * Estimate the progress of the craddle rotation operation.
     *
     * Return a value between 0.0 and 1.0f, where 1.0f means the rotation is completed.
     **/
    float turn_progress();


    /**
     * Rotates the craddle.
     * 
     * Return when the operation has completed.
     *
     * @param   t    N*DRV8834_CLOCKWISE_DIR, N*DRV8834_ANTICLOCKWISE_DIR where N is the number of quarter turns to perform.

     */
    void turn(int t);

    }



/** end of file */