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
#include "Servos.h"
#include "DRV8834.h"
#include "panic.h"

#include <Arduino.h>
#include <TeensyTimerTool.h>
#include <TeensyThreads.h>



#if (SERVO_ADJUST_TIMER_FOR_OVERCLOCK > 0)
#define SERVO_OVERCLOCK_MULTIPLIER(x) ((((int32_t)x)*(F_BUS_ACTUAL/1000000))/150)
#else
#define SERVO_OVERCLOCK_MULTIPLIER(x) (x)
#endif




namespace Servos
    {






	static volatile bool _allow_panic = true;
	static volatile int16_t _fp_panic = 0;
    static volatile int16_t _pos_panic = 0;


	void allowPanic(bool enable)
		{
        _allow_panic = enable;
        _fp_panic = 0;
		_pos_panic = 0;
		}


	bool isPanicAllowed()
		{
        return _allow_panic;
		}






	void SComRes::_debug()
		{
		INFO("\nstatus: ");
		switch (status)
			{
			case SCOMRES_NOT_STARTED: INFO("NOT_STARTED\n"); break;
			case SCOMRES_EXECUTING: INFO("EXECUTING\n"); break;
			case SCOMRES_IDLE: INFO("IDLE\n"); break;
			case SCOMRES_FINISHED: INFO("FINISHED\n"); break;
			default: INFO("UNKNOWN\n"); break;
			}
		INFO("result: ");
		switch (result)
			{
			case SCOMRES_NO_STALL: INFO("NO_STALL\n"); break;
			case SCOMRES_STALLED_AT_POS: INFO("STALLED_AT_POS\n"); break;
			case SCOMRES_STALLED_AWAY: INFO("STALLED_AWAY\n"); break;
			default: INFO("UNKNOWN\n"); break;
			}
		INFO("repetition: " << rep << "\nfeedback_pos: " << feedback_pos << "\ntarget_pos: " << target_pos << "\n");
		}



	inline void _clamp(volatile int16_t & x, int16_t min, int16_t max) { if (x < min) x = min; else if (x > max) x = max; }

	TeensyTimerTool::PeriodicTimer _servoTimer(SERVO_TIMER_NAME);

	volatile int8_t _tumbleF;			// indicate if the PWN signal is HIGH or LOW for tumble
	volatile int8_t _gripF;				// indicate if the PWN signal is HIGH or LOW for grip
	volatile int8_t _interrupt_phase;	// indicate the current phase of the PWM signal
	
	volatile int16_t _prev_fb_grip;			// previous position of the grip servo
	volatile int16_t _prev_fb_tumble;		// previous position of the tumble servo
	volatile int16_t _fb_stall_grip_count;	//
	volatile int16_t _fb_stall_tumble_count;//
	volatile int16_t _grip_stall_center;
	volatile int16_t _tumble_stall_center;
	volatile SCom* CGrip;
	volatile SCom* CTumble;	
	volatile SComRes _dummyres_grip;		// the result of the current command when it is not needed
	volatile SComRes _dummyres_tumble;		// the result of the current command when it is not needed


	namespace Grip
		{
		volatile SCom _queue[SERVO_GRIP_MSG_QUEUE_SIZE + 1]; // the message queue, add an empty slot
		volatile int _queue_head = 0; // position of the head of the queue (read from interrupt)
		volatile int _queue_tail = 0; // position of the head of the queue (push new messages)
		}

	namespace Tumble
		{
		volatile SCom _queue[SERVO_TUMBLE_MSG_QUEUE_SIZE + 1]; // the message queue, add an empty slot. 
		volatile int _queue_head = 0; // position of the head of the queue (read from interrput)
		volatile int _queue_tail = 0; // position of the head of the queue (push new message)
		}




	SLOWFUN void _servoInt()
		{
		if (_interrupt_phase < 2)
			{ // PHASE 0/1 : GRIP SERVO
			if (_interrupt_phase == 0)
				{ // PHASE_0
				if (_gripF == 0) { digitalWriteFast(PIN_SERVO_GRIP_PWM, HIGH); _gripF = 1; }
				const int us_grip = CGrip->pos;
				_servoTimer.setNextPeriod(SERVO_OVERCLOCK_MULTIPLIER((us_grip > 0) ? (SERVO_PWM_HALFPERIOD - us_grip) : (SERVO_PWM_HALFPERIOD / 2)));
				_interrupt_phase = 1;
				return;
				} 
			else
				{ // PHASE 1
				const int us_grip = CGrip->pos;
				if (us_grip > 0) { digitalWriteFast(PIN_SERVO_GRIP_PWM, LOW); _gripF = 0; }
				const int us_tumble = CTumble->pos;
				_servoTimer.setNextPeriod(SERVO_OVERCLOCK_MULTIPLIER((us_tumble > 0) ? (us_tumble) : (SERVO_PWM_HALFPERIOD / 2)));
				_interrupt_phase = 2;
				
				// perform feedback check for stall if neeeded				
				int16_t new_fb_grip = Grip::feedbackPos();
				if (CGrip->feedback_mode & SCOM_FEEDBACK)
					{
					if (abs(new_fb_grip - _prev_fb_grip) < CGrip->stall_delta)
						{ 
						if (_fb_stall_grip_count == 0) 
							{ 
							_grip_stall_center = new_fb_grip; 
							_fb_stall_grip_count++;  
							}
						else
							{
							if (abs(new_fb_grip - _grip_stall_center) < CGrip->stall_delta) { _fb_stall_grip_count++; } else { _grip_stall_center = new_fb_grip; _fb_stall_grip_count = 1; }
							}  

						auto delta = abs(new_fb_grip - CGrip->pos);			
						if ((delta <= CGrip->reach_tolerance) && (_fb_stall_grip_count >= SERVO_GRIP_STALL_COUNT_REACHED))
                            { // we stall at the target position
							CGrip->res->result = SCOMRES_STALLED_AT_POS;
							if (CGrip->feedback_mode & SCOM_ENDCMD_STALL_AT_POS) { if (CGrip->rep > 0) { CGrip->rep = 0; } }
							if (CGrip->feedback_mode & SCOM_DISABLE_STALL_AT_POS) { CGrip->pos = SCOM_POS_DISABLE; CGrip->inc = 0; CGrip->panic_tolerance = SCOM_NO_PANIC; }
							_fb_stall_grip_count = 0;
							}
						else
							{
							const bool panicon = ((CGrip->panic_tolerance >= 0) && (delta > CGrip->panic_tolerance)); 
							if ((delta > CGrip->reach_tolerance) && (panicon) && (_fb_stall_grip_count >= SERVO_GRIP_STALL_COUNT_PANIC))
								{
								if (isPanicAllowed())
									{
									PANIC("Wrong position for Servo Grip.\nerror grip: " << new_fb_grip << "\npos: " << CGrip->pos); // PANIC !
									} 
								else
									{
									if (getPanicFlag() == 0)
										{
										Grip::_setError(new_fb_grip, CGrip->pos);
										DEBUG("[NO PANIC] Wrong position for Servo Grip.\nerror grip: " << new_fb_grip << "\npos: " << CGrip->pos);
										}
									}
								}
							else if ((delta > CGrip->reach_tolerance) && (!panicon) && (_fb_stall_grip_count >= SERVO_GRIP_STALL_COUNT_AWAY))
								{
								CGrip->res->result = SCOMRES_STALLED_AWAY;
								if (CGrip->feedback_mode & SCOM_ENDCMD_STALL_AWAY) { if (CGrip->rep > 0) { CGrip->rep = 0; } }
								if (CGrip->feedback_mode & SCOM_DISABLE_STALL_AWAY) { CGrip->pos = SCOM_POS_DISABLE; CGrip->inc = 0;  CGrip->panic_tolerance = SCOM_NO_PANIC; }
								_fb_stall_grip_count = 0;
								}
							}
                        }
					else
						{
						_fb_stall_grip_count = 0; 
						}
					}
				_prev_fb_grip = new_fb_grip;
				
				// load the next command for the grip servo
				if (CGrip->rep > 0)
					{ // current command is still active
					CGrip->rep--;
					if (CGrip->pos != SCOM_POS_DISABLE) { CGrip->pos += CGrip->inc;  }
					} 
				else
					{ // command completed, load the next one if available
					if (CGrip->rep == 0)
						{ // command just completed. 
						CGrip->res->status = SCOMRES_IDLE;
						Grip::_queue_head = ((Grip::_queue_head + 1) % (SERVO_GRIP_MSG_QUEUE_SIZE + 1)); // we remove it
						CGrip->inc = 0; // disable increment
						CGrip->rep = -1;
						}
					if (Grip::queueSize() > 0)
						{ // there is a command available
						auto _prev_pos = CGrip->pos;
						CGrip->res->status = SCOMRES_FINISHED;
						CGrip = &(Grip::_queue[Grip::_queue_head]); // load the new command
						if (CGrip->res == nullptr) { CGrip->res = &_dummyres_grip; }
						CGrip->res->status = SCOMRES_EXECUTING;
						CGrip->res->rep = 0;
						CGrip->res->result = SCOMRES_NO_STALL;
						if (CGrip->rep < 0) { CGrip->rep = 0; }
						if (CGrip->pos == SCOM_POS_FEEDBACK) { CGrip->pos = (new_fb_grip > 0) ? new_fb_grip : _prev_pos; }
						else if (CGrip->pos == SCOM_POS_PREVIOUS) { CGrip->pos = _prev_pos; }
						}
					}
				if (CGrip->pos != SCOM_POS_DISABLE)	_clamp(CGrip->pos, SERVO_GRIP_MIN_PULSE, SERVO_GRIP_MAX_PULSE);
				CGrip->res->target_pos   = CGrip->pos;
				CGrip->res->feedback_pos = new_fb_grip;
				CGrip->res->rep++;
				return;
				}
			} 
		else
			{ // PHASE 2:3 TUMBLE SERVO
			if (_interrupt_phase == 2)
				{ // PHASE 2
				if (_tumbleF == 0) { digitalWriteFast(PIN_SERVO_TUMBLE_PWM, HIGH); _tumbleF = 1; }
				const int us_tumble = CTumble->pos;
				_servoTimer.setNextPeriod(SERVO_OVERCLOCK_MULTIPLIER((us_tumble > 0) ? (SERVO_PWM_HALFPERIOD - us_tumble) : (SERVO_PWM_HALFPERIOD / 2)));
				_interrupt_phase = 3;
				return;
				} 
			else
				{ // PHASE 3
				const int us_tumble = CTumble->pos;
				if (us_tumble > 0) { digitalWriteFast(PIN_SERVO_TUMBLE_PWM, LOW);  _tumbleF = 0; }
				const int us_grip = CGrip->pos;
				_servoTimer.setNextPeriod(SERVO_OVERCLOCK_MULTIPLIER((us_grip > 0) ? (us_grip) : (SERVO_PWM_HALFPERIOD / 2)));
				_interrupt_phase = 0;

				// perform feedback check for stall if neeeded				
				int16_t new_fb_tumble = Tumble::feedbackPos();
				new_fb_tumble = Tumble::feedbackPos();
				if (CTumble->feedback_mode & SCOM_FEEDBACK)
					{
					if (abs(new_fb_tumble - _prev_fb_tumble) < CTumble->stall_delta)
						{ 
						if (_fb_stall_tumble_count == 0) 
							{ 
							_tumble_stall_center = new_fb_tumble; 
							_fb_stall_tumble_count++;  
							}
						else
							{
							if (abs(new_fb_tumble - _tumble_stall_center) < CTumble->stall_delta) { _fb_stall_tumble_count++; } else { _tumble_stall_center = new_fb_tumble; _fb_stall_tumble_count = 1; }
							}  


						auto delta = abs(new_fb_tumble - CTumble->pos);
						if ((delta <= CTumble->reach_tolerance) && (_fb_stall_tumble_count >= SERVO_TUMBLE_STALL_COUNT_REACHED))
							{ // we stall at the target position
							CTumble->res->result = SCOMRES_STALLED_AT_POS;
							if (CTumble->feedback_mode & SCOM_ENDCMD_STALL_AT_POS) { if (CTumble->rep > 0) { CTumble->rep = 0; } }
							if (CTumble->feedback_mode & SCOM_DISABLE_STALL_AT_POS) { CTumble->pos = SCOM_POS_DISABLE; CTumble->inc = 0; CTumble->panic_tolerance = SCOM_NO_PANIC; }
							_fb_stall_tumble_count = 0;
							} else
							{
							const bool panicon = ((CTumble->panic_tolerance >= 0) && (delta > CTumble->panic_tolerance));
							if ((delta > CTumble->reach_tolerance) && (panicon) && (_fb_stall_tumble_count >= SERVO_TUMBLE_STALL_COUNT_PANIC))
								{
								if (isPanicAllowed())
									{
									PANIC("Wrong position for Servo Tumble.\nerror tumble: " << new_fb_tumble << "\npos: " << CTumble->pos); // PANIC !
									} 
								else
									{
									if (getPanicFlag() == 0)
										{
										Tumble::_setError(new_fb_tumble, CTumble->pos);
										DEBUG("[NO PANIC] Wrong position for Servo Tumble.\nerror tumble : " << new_fb_tumble << "\npos : " << CTumble->pos);
										}
									}
								} 
							else if ((delta > CTumble->reach_tolerance) && (!panicon) && (_fb_stall_tumble_count >= SERVO_TUMBLE_STALL_COUNT_AWAY))
								{
								CTumble->res->result = SCOMRES_STALLED_AWAY;
								if (CTumble->feedback_mode & SCOM_ENDCMD_STALL_AWAY) { if (CTumble->rep > 0) { CTumble->rep = 0; } }
								if (CTumble->feedback_mode & SCOM_DISABLE_STALL_AWAY) { CTumble->pos = SCOM_POS_DISABLE; CTumble->inc = 0;  CTumble->panic_tolerance = SCOM_NO_PANIC; }
								_fb_stall_tumble_count = 0;
								}
							}
                        }
					else
						{
						_fb_stall_tumble_count = 0; 
						}
					}
				_prev_fb_tumble = new_fb_tumble;

				// load the next command for the tumble servo
				if (CTumble->rep > 0)
					{ // current command is still active
					CTumble->rep--;
					if (CTumble->pos != SCOM_POS_DISABLE) { CTumble->pos += CTumble->inc;  }
					} 
				else
					{ // command completed, load the next one if available
					if (CTumble->rep == 0)
						{ // command just completed. 
						CTumble->res->status = SCOMRES_IDLE;
						Tumble::_queue_head = ((Tumble::_queue_head + 1) % (SERVO_TUMBLE_MSG_QUEUE_SIZE + 1)); // we remove it
						CTumble->inc = 0; // disable increment
						CTumble->rep = -1;
						}
					if (Tumble::queueSize() > 0)
						{ // there is a command available
						auto _prev_pos = CTumble->pos;
						CTumble->res->status = SCOMRES_FINISHED;
						CTumble = &(Tumble::_queue[Tumble::_queue_head]); // load the new command
						if (CTumble->res == nullptr) { CTumble->res = &_dummyres_tumble; }
						CTumble->res->status = SCOMRES_EXECUTING;
						CTumble->res->rep = 0;
						CTumble->res->result = SCOMRES_NO_STALL;
						if (CTumble->rep < 0) { CTumble->rep = 0; }
						if (CTumble->pos == SCOM_POS_FEEDBACK) { CTumble->pos = (new_fb_tumble > 0) ? new_fb_tumble : _prev_pos; }
						else if (CTumble->pos == SCOM_POS_PREVIOUS) { CTumble->pos = _prev_pos; }
						}
					}
				if (CTumble->pos != SCOM_POS_DISABLE)	_clamp(CTumble->pos, SERVO_TUMBLE_MIN_PULSE, SERVO_TUMBLE_MAX_PULSE);
				CTumble->res->target_pos   = CTumble->pos;
				CTumble->res->feedback_pos = new_fb_tumble;
				CTumble->res->rep++;
				return;
				}			
			}
		}


	SLOWFUN void disablePower()
		{
		pinMode(PIN_SERVO_PWR_CTRL, OUTPUT);
		digitalWrite(PIN_SERVO_PWR_CTRL, LOW);
		}


	SLOWFUN void init()
		{
		static int8_t _init_done = 0;
		if (_init_done) return;
		_init_done = 1;

		allowPanic(true); // panic enabled by default

		debug::init(); // just in case
		DEBUG("begin Servos::init() ...");

		FBSensors::init(); // just in case, this take care of the feeback initialization

		pinMode(PIN_SERVO_PWR_CTRL, OUTPUT);
		digitalWrite(PIN_SERVO_PWR_CTRL, LOW);

		pinMode(PIN_SERVO_GRIP_PWM, OUTPUT);
		digitalWrite(PIN_SERVO_GRIP_PWM, HIGH);

		pinMode(PIN_SERVO_TUMBLE_PWM, OUTPUT);
		digitalWrite(PIN_SERVO_TUMBLE_PWM, HIGH);

		threads.delay(2);
		digitalWrite(PIN_SERVO_PWR_CTRL, HIGH);
		threads.delay(20);

		Grip::_queue_head = 0;
		Grip::_queue_tail = 1;
		Grip::_queue[Grip::_queue_head].set(SCom(SCOM_POS_DISABLE));
		CGrip = &(Grip::_queue[Grip::_queue_head]);
		CGrip->res = &_dummyres_grip;
	
		Tumble::_queue_head = 0;
		Tumble::_queue_tail = 1;
		Tumble::_queue[Tumble::_queue_head].set(SCom(SCOM_POS_DISABLE));
		CTumble = &(Tumble::_queue[Tumble::_queue_head]);
		CTumble->res = &_dummyres_tumble;

		_tumbleF = 1;
		_gripF = 1;
		_interrupt_phase = 0;
		_fb_stall_grip_count = 0;
		_fb_stall_tumble_count = 0;
		_grip_stall_center = -1;
		_tumble_stall_center = -1;

		FBSensors::waitForMeasurement();
		_prev_fb_grip = Grip::feedbackPos();
		_prev_fb_tumble = Tumble::feedbackPos();

		_servoTimer.begin(_servoInt, SERVO_OVERCLOCK_MULTIPLIER((SERVO_PWM_HALFPERIOD / 2)));
		NVIC_SET_PRIORITY(SERVO_IRQ_NAME, SERVO_IRQ_PRIORITY);
		//_servoTimer.setPrescaler(1); 
		
		DEBUG("Servos::init() complete\n");
		}





	namespace Grip
		{
		
		bool getError(int& fb, int& pos)
			{
			fb = _fp_panic;
            pos = _pos_panic;
            return (fb != pos);
			}

		void _setError(int16_t fb, int16_t pos)
			{
            _fp_panic = fb;
            _pos_panic = pos;
			setPanicFlag(3);
            }



		int targetPos()
			{
            return CGrip->pos;
            }
			


		SLOWFUN void disableNow()
			{
			__disable_irq();
			_queue_head = 0;
			_queue_tail = 1;
			Grip::_queue[Grip::_queue_head].set(SCom(SCOM_POS_DISABLE));
			CGrip = &(Grip::_queue[Grip::_queue_head]);
			CGrip->res = &_dummyres_grip;
			_interrupt_phase = 0;
			__enable_irq();
			waitQueueEmpty();
			}


		bool isDisabled()
			{
			return (CGrip->pos == SCOM_POS_DISABLE);
			}


		SLOWFUN void waitQueueEmpty()
			{
			while(queueSize() > 0) 
				{ 
				if (getPanicFlag() != 0) return; // panic detected !
				threads.yield(); 
				}
			}


		SLOWFUN int queueSize()
			{
			const int L = _queue_tail - _queue_head;
			return ((L >= 0) ? L : (SERVO_GRIP_MSG_QUEUE_SIZE + 1 + L));
			}


		SLOWFUN void pushCommand(const SCom& command, bool waitUntilExecution, SComRes* result)
			{
			while(queueSize() >= SERVO_GRIP_MSG_QUEUE_SIZE) { threads.yield(); } // wait for queue not full
			_queue[_queue_tail].set(command); // copy the command
			if (result != nullptr) { _queue[_queue_tail].setRes(result); result->set(SCOMRES_NOT_STARTED, SCOMRES_NO_STALL, 0, SCOMRES_UNAVAILABLE, SCOMRES_UNAVAILABLE); }
            _queue_tail = ((_queue_tail + 1) % (SERVO_GRIP_MSG_QUEUE_SIZE + 1)); // increment the tail
			if (waitUntilExecution)
				{
				while(queueSize() > 1) { threads.yield(); } // wait for the command to start execution
				}
			}


		}


	namespace Tumble
		{		

		bool getError(int& fb, int& pos)
			{
			fb = _fp_panic;
			pos = _pos_panic;
			return (fb != pos);
			}

		void _setError(int16_t fb, int16_t pos)
			{
			_fp_panic = fb;
			_pos_panic = pos;
			setPanicFlag(2);
			}


		int targetPos()
			{
			return CTumble->pos;
			}


		SLOWFUN void disableNow()
			{
			__disable_irq();
			_queue_head = 0;
			_queue_tail = 1;
			Tumble::_queue[Tumble::_queue_head].set(SCom(SCOM_POS_DISABLE));
			CTumble = &(Tumble::_queue[Tumble::_queue_head]);
			CTumble->res = &_dummyres_tumble;
			_interrupt_phase = 0;
			__enable_irq();
			waitQueueEmpty();
			}


		bool isDisabled()
			{
			return (CTumble->pos == SCOM_POS_DISABLE);
			}



		SLOWFUN void waitQueueEmpty()
			{
			while (queueSize() > 0) 
				{ 
				if (getPanicFlag() != 0) return; // panic detected !
				threads.yield(); 
				}
			}


		SLOWFUN int queueSize()
			{
			const int L = _queue_tail - _queue_head;
			return ((L >= 0) ? L : (SERVO_TUMBLE_MSG_QUEUE_SIZE + 1 + L));
			}


		SLOWFUN void pushCommand(const SCom& command, bool waitUntilExecution, SComRes* result)
			{
			while (queueSize() >= SERVO_TUMBLE_MSG_QUEUE_SIZE) { threads.yield(); } // wait for queue not full
			_queue[_queue_tail].set(command); // copy the command
			if (result != nullptr) { _queue[_queue_tail].setRes(result);  result->set(SCOMRES_NOT_STARTED, SCOMRES_NO_STALL, 0, SCOMRES_UNAVAILABLE, SCOMRES_UNAVAILABLE); }
			_queue_tail = ((_queue_tail + 1) % (SERVO_TUMBLE_MSG_QUEUE_SIZE + 1)); // increment the tail
			if (waitUntilExecution)
				{
				while (queueSize() > 1) { threads.yield(); } // wait for the command to start execution
				}
			}

		}






    }


/** end of file */

