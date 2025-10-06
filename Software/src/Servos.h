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



namespace Servos
    {

    /**
    * Initialize the servo module.
    **/
    SLOWFUN void init();


	/**
	* Disable the sevo power module (use only for emergency). 
	**/
	SLOWFUN void disablePower();


	/**
   * Set to true to enable triggering panic mode for the servos
   * when a position anomaly is detected.
   *
   * If set to false, panic mode will not be trigerred but power to the cradle and the servo is still cut.
   **/
	void allowPanic(bool enable);

	/**
	* Check whether panic mode if on.
	**/
	bool isPanicAllowed();



	/**
	* Structure to return the result of a SCom command
	**/
	struct SComRes
		{
		#define SCOMRES_UNAVAILABLE		-1 	// 
		#define SCOMRES_NOT_STARTED		0 	// the command was not yet executed
		#define SCOMRES_EXECUTING		1	// the command is currently being executed	
		#define SCOMRES_IDLE			2	// the command is idle
		#define SCOMRES_FINISHED		3	// the command was executed and completed normally

		#define SCOMRES_NO_STALL		0	// the command was executed and the servo did not stall
		#define SCOMRES_STALLED_AT_POS  1	// the command was executed but the servo stalled at the target position
		#define SCOMRES_STALLED_AWAY	2	// the command was executed but the servo stalled away from the target position

		volatile int16_t status;		// status of the command: either SCOMRES_NOT_STARTED, SCOMRES_EXECUTING, SCOMRES_IDLE, SCOMRES_FINISHED
		volatile int16_t result;		// result of the command: either SCOMRES_NO_STALL, SCOMRES_STALLED_AT_POS, SCOMRES_STALLED_AWAY -> the result can change multiple times while status = SCOMRES_EXECUTING
		volatile int16_t rep;			// number of times the command was repeated (number of ticks)
		volatile int16_t feedback_pos;  // feedback position -> the value changes in real time while status = SCOMRES_EXECUTING
		volatile int16_t target_pos;    // servo target position -> the value changes in real time while status = SCOMRES_EXECUTING


		SComRes() = default; 

		void set(int16_t status, int16_t result, int16_t rep, int16_t feedback_pos, int16_t target_pos) volatile
            { this->status = status; this->result = result; this->rep = rep; this->feedback_pos = feedback_pos; this->target_pos = target_pos; }


		/** for debug purpose */
		void _debug();
		};


	/**
	* Servo command structure
	**/
	struct SCom
		{


		#define SCOM_NO_FEEDBACK 0			   // no feedback (default)
		#define SCOM_FEEDBACK    1             // use feedback

		#define SCOM_ENDCMD_STALL_AT_POS 2		// go to the next command when stalling at the target position
		#define SCOM_ENDCMD_STALL_AWAY   4		// go to the next command when stalling away from the target position

		#define SCOM_DISABLE_STALL_AT_POS  8	// stop the servo if we stall at the target position
		#define SCOM_DISABLE_STALL_AWAY   16	// stop the servo if we stall away from the target position

		// special value for panic_tolerance
		#define SCOM_NO_PANIC -1				// never panic when stalling

		// special values for pos
		#define SCOM_POS_DISABLE 0				// position to disable the servo
		#define SCOM_POS_PREVIOUS  -1			// use the end position of the previous command
		#define SCOM_POS_FEEDBACK  -2			// use the current position of the servo according to the feedback


		volatile int16_t pos;	// position to reach:  > 0 or SCOM_POS_DISABLE or SCOM_POS_PREVIOUS or SCOM_POS_FEEDBACK
		volatile int16_t inc;    // increment to pos after each frame repeat, clamp at each of admissible pulse values.
		volatile int16_t rep;    // number of repetitions [0 = single frame]
		volatile int16_t feedback_mode; //  SCOM_FEEDBACK/SCOM_NO_FEEDBACK, SCOM_ENDCMD_STALL_AT_POS, SCOM_ENDCMD_STALL_AWAY

		volatile int16_t panic_tolerance; // tolerance between target pos and feeback pos before setting panic mode on stall: positive value or SCOM_NO_PANIC. Default value: SERVO_DEFAULT_PANIC_TOLERANCE
		volatile int16_t reach_tolerance; // tolerence between the target position and the feedback to decide it the target is reached. default SERVO_DEFAULT_REACH_TOLERANCE;
		volatile int16_t stall_delta;     // stall is detected when the change of feedback become smaller than this value between two frames for SERVO__[TUMBLE/GRIP]_STALL_COUNT_[EVENT] counts. Default: SERVO_DEFAULT_STALL_DELTA 

		volatile SComRes* res; // pointer to the structure to store the result of the command (or nullptr)


		/** Constructor */
		SCom(int16_t pos_ = 0, int16_t inc_ = 0, int16_t rep_ = 0, int16_t feedback_mode_ = SCOM_NO_FEEDBACK, int16_t panic_tolerance_ = SERVO_DEFAULT_PANIC_TOLERANCE, int16_t reach_tolerance_ = SERVO_DEFAULT_REACH_TOLERANCE, int16_t stall_delta_ = SERVO_DEFAULT_STALL_DELTA)
			: pos(pos_), inc(inc_), rep(rep_), feedback_mode(feedback_mode_), panic_tolerance(panic_tolerance_), reach_tolerance(reach_tolerance_), stall_delta(stall_delta_), res(nullptr) {}


		/** default copy constructor and assignement operator */
		SCom(const SCom&) = default;
		SCom & operator=(const SCom&) = default;
		
		/** copy a command (volatile struct) */
		void set(const SCom& com) volatile
			{ pos = com.pos; inc = com.inc; rep = com.rep; feedback_mode = com.feedback_mode; panic_tolerance = com.panic_tolerance; reach_tolerance = com.reach_tolerance; stall_delta = com.stall_delta; res = com.res; }

		void set(const volatile SCom& com) volatile
			{ pos = com.pos; inc = com.inc; rep = com.rep; feedback_mode = com.feedback_mode; panic_tolerance = com.panic_tolerance; reach_tolerance = com.reach_tolerance; stall_delta = com.stall_delta; res = com.res; }

		/** set the structure to store the result of the command */
		void setRes(volatile SComRes* res_) volatile { res = res_; }

		};







	namespace Grip
		{

		/**
		* Return the current position of the grip servo
		*
		* call calibrate_offset() before first use to get improve accuracy.
		**/
		int feedbackPos();


		/**
		* Return the current target position of the servo. 
        * Return SCOM_POS_DISABLE if the servo is currently disabled.
		**/
		int targetPos();


		/**
		* Disable the grip immediately and remove all commands from the queue.
		* Return when the servo is disabled.		
		**/
		SLOWFUN void disableNow();


		/**
		* Check if the grip is currently disabled
		**/
		bool isDisabled();


		/**
		* Wait until the command queue of the grip servo is empty.
		* A command posted after will be executed immediately.
		**/
		SLOWFUN void waitQueueEmpty();


		/**
		* Number of commands currently queued for the grip servo.
		* The command at position 1 is the one currently being executed
		* so the method returns 0 iif the servo is idle.
		**/
		SLOWFUN int queueSize();


        /**
         * Pushes a command to the grip servo queue (wait if the queue is full)
         *
         * @param       command             The command.
         * @param       waitUntilExecution  (Optional) True to wait for the beginning of the execution of
         *                                  the command (but not its end). Otherwise, the method return
         *                                  immediately.
         * @param [out] result              (Optional) Structure to store the result of the command.
         */
		SLOWFUN void pushCommand(const SCom& command, bool waitUntilExecution = false, SComRes * result = nullptr);


		/**
		*  Find the calibration offset for the grip servo w.r.t. the predefined values. 
        *  Called once at first use of servo to increase accuraccy. 
		*
		*  -> Enter panic mode if offset not within tolerance values
		**/
		SLOWFUN void calibrateOffset();


		/**
		* Perform a full calibration of the grip servo.
		* -> to recompute the arrays: _grip_PWMtoV[] and _grip_VtoPWM[]
		*
		* @param   nb_calib    Number of calibration array created (G1,G2,...)
		*
		* - Calibration takes a long times: about 0.9 * nb_calib hours to ruin !
		* - output progress in the serial debug port
		* - output results on SERIAL (that which should be copied into the python script)
		* - c.f. file Servos_calibration.cpp for the associated python script and more info on how to recreate the arrays.
		**/
		SLOWFUN void fullCalibration(int nb_calib = 3);


		/**
		* Check a position anomaly has been detected on the grip servo.
        * if so, put the details in fb and pos.
		**/
		bool getError(int & fb, int & pos);


		/**
		* For internal use by FBSensors.
		* Set the current error for the grip servo.
		**/
		void _setError(int16_t fb, int16_t pos);

		}


	namespace Tumble
		{

		/**
		* Return the current position of the tumble servo
		*
		* call calibrate_offset() before first use to get improve accuracy.
		**/
		int feedbackPos();


		/**
		* Return the current target position of the servo.
		* Return SCOM_POS_DISABLE if the servo is currently disabled.
		**/
		int targetPos();


		/**
		* Disable the tumble immediately and remove all commands from the queue.
        * Return when the servo is disabled.
		**/
		SLOWFUN void disableNow();


		/**
		* Check if the tumble is currently disabled
		**/
		bool isDisabled();


		/**
		* Wait until the command queue of the tumble servo is empty.
		* A command posted after will be executed immediately.
		**/
		SLOWFUN void waitQueueEmpty();


		/**
		* Number of commands currently queued for the tumble servo.
		* The command at position 1 is the one currently being executed
		* so the method returns 0 iif the servo is idle.
		**/
		SLOWFUN int queueSize();


		/**
		 * Pushes a command to the tumble servo queue (wait if the queue is full)
		 *
		 * @param       command             The command.
		 * @param       waitUntilExecution  (Optional) True to wait for the beginning of the execution of
		 *                                  the command (but not its end). Otherwise, the method return
		 *                                  immediately.
		 * @param [out] result              (Optional) Structure to store the result of the command.
		 */
		SLOWFUN void pushCommand(const SCom& command, bool waitUntilExecution = false, SComRes* result = nullptr);


		/**
		*  Find the calibration offset for the tumble servo w.r.t. the predefined values.
		*  Called once at first use of servo to increase accuraccy.
		*
		*  -> Enter panic mode if offset not within tolerance values
		**/
		SLOWFUN void calibrateOffset();


		/**
		* Perform a full calibration of the tumble servo.
		* -> to recompute the arrays: _tumble_PWMtoV[] and _tumble_VtoPWM[]
		*
		* @param   nb_calib    Number of calibration array created (T1,T2,...)
		*
		* - Calibration takes a long times: about 1.5 * nb_calib hours to ruin !
		* - output progress in the serial debug port
		* - output results on SERIAL (that which should be copied into the python script)
		* - c.f. file Servos_calibration.cpp for the associated python script and more info on how to recreate the arrays.
		**/
		SLOWFUN void fullCalibration(int nb_calib = 3);


		/**
		* Check a position anomaly has been detected on the grip servo.
		* if so, put the details in fb and pos.
		**/
		bool getError(int& fb, int& pos);


		/**
		* For internal use by FBSensors.
		* Set the current error for the grip servo.
		**/
		void _setError(int16_t fb, int16_t pos);

		}




    }

/** end of file */

