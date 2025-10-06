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

#include <Arduino.h>





/***********************************************************************************
*
*                               MAIN CONFIGURATION FILE
*
************************************************************************************/


/*********************************************************
* DEBUGGING AND MEMORY MANAGMENT
**********************************************************/


// define this to enable DEBUG() messages. 
// the message are printed on the auxiliarry SerialUSB port so USB Type must be "double Serial" in VS/Arduino for compiling
//
// *** When set (and USB Type, Dual Serial selected in compiler), both Serial port must be open to start (i.e. initAll() waits until serials are ready) ***
// 
#define ENABLE_DEBUG_VIA_SERIAL 1


#ifdef CDC2_DATA_INTERFACE // c.f. usb_desc.h
    #define _DEBUG_SERIAL	SerialUSB1  // use the second USB serial for debugging
#else
    #if ENABLE_DEBUG_VIA_SERIAL
    #ifdef __cplusplus
    #warning "Debug disabled, please select 'USB Type Dual Serial' in the compiler settings"
    #endif
    #endif
    #undef ENABLE_DEBUG_VIA_SERIAL
    #define ENABLE_DEBUG_VIA_SERIAL 0   // no debug serial port available : disable debbuging
#endif


/*********************************************************
* TEENSYTHREADS
**********************************************************/

// each time slice is 10us
#define TEENSYTHREADS_SLICE_US 40 

// each thread runs for 10 slices (i.e. 100us) before yielding to the next thread
#define TEENSYTHREADS_DEFAULT_TICKS 10



/*********************************************************
* MEMORY MANAGMENT
**********************************************************/

// define this to enable memory debugging via DumpMemoryInfo(); and EstimateStackUsage();
#define DEBUG_MEMORY 1


// Choose whether 'slow' function run from flash.
#define RUN_SLOWFUN_FROM_FLASH 1


#if (RUN_SLOWFUN_FROM_FLASH)
#define SLOWFUN FLASHMEM				
#else
#define SLOWFUN
#endif


/*********************************************************
* PANIC
**********************************************************/

// number of milliseconds before powering off when panic detected. 
#define PANIC_POWEROFF_DELAY_MS 20000


/*********************************************************
* VOLTAGE
**********************************************************/

// max voltage (to protect DRV8834). Emergency shutdown above this voltage
#define CUTOFF_VOLTAGE_MAX	9.0f		

// min voltage (to protect batteries). Emergency shutdown below this voltage
#define CUTOFF_VOLTAGE_MIN	6.2f		

// below this voltage, we consider we are on low battery
#define VOLTAGE_THRESHOLD_LOW 7.4f 

// number of consecutive under voltage readings needed before declaring under voltage panic 
#define VOLTAGE_PANIC_SAMPLES 10

#define CHARGING_STATUS_TIMEOUT 4000 // time (ms) without change before changing the charging status

#define CHARGING_CURRENT_SLOW 0.2f // below this current, we consider we are not charging
#define CHARGING_CURRENT_FAST 0.8f // below this current, we consider we are slow charging


/***************************************
* LEDSTRIP
****************************************/

#define LED_CONFIG (WS2811_GRBW | WS2811_800kHz) //uses SK6812 (GRBW) leds. 800kHz data rate.




/*********************************************************
* BASIC MOVES
**********************************************************/

#define BASICMOVES_STACK_SIZE        4096
#define BASICMOVES_TUMBLE_TIME_MS    1000
#define BASICMOVES_CLOSEGRIP_TIME_MS 1000
#define BASICMOVES_OPENGRIP_TIME_MS  1000


/*********************************************************
* SERVOS
**********************************************************/


// half period for the PWM signal (10ms for 50Hz)
#define SERVO_PWM_HALFPERIOD  10000     

#define SERVO_DEFAULT_STALL_DELTA 10        // value under which we consider the servo has not move (to increase the stall count)
#define SERVO_DEFAULT_REACH_TOLERANCE 70    // value under which we consider to have reached the position
#define SERVO_DEFAULT_PANIC_TOLERANCE 70    // value over which we stop the servo and start the panic mode when stalled (must be larger than SERVO_DEFAULT_REACH_TOLERANCE)

#define SERVO_TUMBLE_MSG_QUEUE_SIZE 12
#define SERVO_GRIP_MSG_QUEUE_SIZE 12


/**** Parameter for Tumble *******/

// -> associated with the calibration arrays: tumble_PWMtoV[] and tumble_VtoPWM[]
#define SERVO_TUMBLE_MIN_PULSE  450     // hardware values: set before full calibration procedure
#define SERVO_TUMBLE_MAX_PULSE  2200    // MIN : tumble dow, MAX : tumble up

#define SERVO_TUMBLE_MIN_V_FB  657      // values set after the full calibration procedure.
#define SERVO_TUMBLE_MAX_V_FB  3025     //

#define SERVO_TUMBLE_CALIB_TOLERANCE 150 // tolerance when performing fast offset calibration (previous 100)

#define SERVO_TUMBLE_STALL_COUNT_REACHED    3   // number of consecutive read to declare stall at position
#define SERVO_TUMBLE_STALL_COUNT_AWAY       6   // number of consecutive read to declare stall away from position
#define SERVO_TUMBLE_STALL_COUNT_PANIC      12  // number of consecutive read to declare stall panic


#define SERVO_TUMBLE_REST_POS 1880        // rest (safe) position for the tumble servo
#define SERVO_TUMBLE_REST_MIN 1780        // minimum safe value   
#define SERVO_TUMBLE_CUBE_TOUCH 1550      // position where the tumble start touching the cube
#define SERVO_TUMBLE_CUBE_UPRIGHT 650     // posiiton upright of the tumble
#define SERVO_TUMBLE_CUBE_SPEED_UP 60     // speed when going up during a tumble
#define SERVO_TUMBLE_CUBE_SPEED_DOWN 90   // speed when going down during a tumble



/**** Parameter for Grip  *******/

// -> associated with the calibration arrays: grip_PWMtoV[] and grip_VtoPWM[]
#define SERVO_GRIP_MIN_PULSE  670      // hardware values: set before full calibration procedure
#define SERVO_GRIP_MAX_PULSE  2030     // MIN : grip down, MAX = grip up

#define SERVO_GRIP_MIN_V_FB  976      // values set after the full calibration procedure.
#define SERVO_GRIP_MAX_V_FB  2760     //


#define SERVO_GRIP_CALIB_TOLERANCE 150 // tolerance when performing fast offset calibration

#define SERVO_GRIP_STALL_COUNT_REACHED    3   // number of consecutive read to declare stall at position
#define SERVO_GRIP_STALL_COUNT_AWAY       8   // number of consecutive read to declare stall away from position
#define SERVO_GRIP_STALL_COUNT_PANIC      14  // number of consecutive read to declare stall panic


#define SERVO_GRIP_STORE_POS  1950      // value for the grip when it is at rest, safe and "stored" for a long time
#define SERVO_GRIP_REST_POS   1500      // value for the grip when it is at rest (safe position)
#define SERVO_GRIP_REST_MIN   1400      // minimum safe value (below this, we may touch when umbling/turning)
#define SERVO_GRIP_CUBE_TOUCH_MIN  730  // minimum position where the grip can touch the cube
#define SERVO_GRIP_CUBE_TOUCH_MAX  1000 // maximum position where the grip can touch the cube
#define SERVO_GRIP_CUBE_TOUCH_OFF  5    // offset to the cube position to prevent bumping into it





/*********************************************************
* STEPPER MOTOR DRIVER
**********************************************************/

// microstepping mode for the motor driver when doing inc. steps. 
// One of: 1,2,4,8,16,32
#define DRV8834_STEPS_MICROSTEPPING      16

// step frequency when doing inc. steps.
#define DRV8843_STEPS_FREQ               4000

// microstepping mode for the motor driver when turning.
// One of: 1,2,4,8,16,32
#define DRV8834_TURN_MICROSTEPPING      16

// default start frequency when doing turns
#define DRV8843_TURN_FREQ_START_DEFAULT       4000 // 5000

// default max frequency when doing turns
#define DRV8843_TURN_FREQ_MAX_DEFAULT         10000 // 12000

// default ratio of time spent accelerating/decelerating when doing turns
#define DRV8843_TURN_ALPHA_DEFAULT            0.3f // 0.2f


// start frequency when doing 90 turns
#define DRV8843_TURN_FREQ_START_90DEG       4000 

// max frequency when doing 90 turns
#define DRV8843_TURN_FREQ_MAX_90DEG         10000 

// ratio of time spent accelerating/decelerating when doing 90 turns
#define DRV8843_TURN_ALPHA_90DEG            0.3f // 0.2f


// start frequency when doing 180 turns
#define DRV8843_TURN_FREQ_START_180DEG       4000

// max frequency when doing 180  turns
#define DRV8843_TURN_FREQ_MAX_180DEG         14000

// ratio of time spent accelerating/decelerating when doing 180 turns
#define DRV8843_TURN_ALPHA_180DEG            0.2f // 0.2f


// start frequency when doing kociemba fast rotation
#define DRV8843_TURN_FREQ_START_KOCIEMBA       4000

// max frequency when doing  kociemba fast rotation
#define DRV8843_TURN_FREQ_MAX_KOCIEMBA         30000

// ratio of time spent accelerating/decelerating when doing  kociemba fast rotation
#define DRV8843_TURN_ALPHA_KOCIEMBA            0.25f





// set to true to invert clockise and anti-clockwise dir. 
#define DRV8834_INVERT_DIR              true

// define the clockwise and anticlockwise directions. 
#define DRV8834_CLOCKWISE_DIR           (DRV8834_INVERT_DIR ? 1 : -1)
#define DRV8834_ANTICLOCKWISE_DIR       (DRV8834_INVERT_DIR ? -1 : 1)

// angle overshoot when doing a clockwise turn
#define DRV8834_CLOCKWISE_OVERSHOOT_ANGLE   10.0f

// angle overshoot when doing an anticlockwise turn
#define DRV8834_ANTICLOCKWISE_OVERSHOOT_ANGLE   10.0f

// default overshoot: DRV8834_CLOCKWISE_OVERSHOOT_ANGLE for clockwise, DRV8834_ANTICLOCKWISE_OVERSHOOT_ANGLE for anticlockwise
#define DRV8834_DEFAULT_OVERSHOOT_ANGLE -1.0f

// time to wait for motor to stabilize after enabling power on motor driver
#define DRV8834_WAIT_AFTER_ENABLE_MS 50 

// time to wait for motor to immobilize after disabling power on motor driver
#define DRV8834_WAIT_BEFORE_DISABLE_MS 50 

// offset to center the craddle (between 0 and 4095)
#define DRV8834_AS5600_OFFSET_0DEG 7
#define DRV8834_AS5600_OFFSET_90DEG 1040
#define DRV8834_AS5600_OFFSET_180DEG 2048
#define DRV8834_AS5600_OFFSET_270DEG 3085

#define DRV8834_AS5600_ANGLE_ERROR 0.8f // maximum admissible angle error at end of turn w.r.t the target angle: if larger, repositioning is required
#define DRV8834_AS5600_ANGLE_PANIC 10.0f // maximum admissible angle error before trigerring panic mode
#define DRV8834_AS5600_ANGLE_PANIC_FAST_MOVE 60.0f // maximum admissible angle error before trigerring panic mode when rotating very fast. 
#define DRV8834_AS5600_NB_PANIC_MEASURES 4 // number of consecutive measures above DRV8834_AS5600_ANGLE_PANIC to declare panic mode.

#define CRADDLE_NO_POS 1000 // value representing unknown craddle position 


/*********************************************************
* TJpgDec System Configurations (replace file tjpgdcnf.h)
**********************************************************/


/* Specifies size of stream input buffer */
#define	JD_SZBUF		1024 // default 512

/* Specifies output pixel format.  0: RGB888 (24-bit/pix)  1: RGB565 (16-bit/pix)  2: Grayscale (8-bit/pix)*/
#define JD_FORMAT		0

/* Switches output descaling feature.  0: Disable  1: Enable */
#define	JD_USE_SCALE	1

/* Use table conversion for saturation arithmetic. A bit faster, but increases 1 KB of code size.
   0: Disable
   1: Enable
*/
#define JD_TBLCLIP		0

/* Optimization level
   0: Basic optimization. Suitable for 8/16-bit MCUs.
   1: + 32-bit barrel shifter. Suitable for 32-bit MCUs.
   2: + Table conversion for huffman decoding (wants 6 << HUFF_BIT bytes of RAM)
*/
#define JD_FASTDECODE	1








/*********************************************************
* ESP32-CAM
**********************************************************/

// baudrate for serial communication 
#define ESPCAM_SERIAL_SPEED 500000

//  baudrate for spi communication
#define ESPCAM_SPI_SPEED 60000000


// (additional) size of the RX serial buffer for communicating with the ESPCAM. 
#define ESPCAM_RX_buffer_size       1024    

// size of the SerialPacket receive buffer  
#define ESPCAM_SerialPacket_buffer_size   2048    

// size of the workspace for the tjpgd decoder. 
#define ESPCAM_TJPGD_WORK_SIZE  4096

// size of spi transfer. must be a multiple of JD_SZBUF
#define ESPCAM_SPI_CHUNK_SIZE   (JD_SZBUF * 4)

// additional bytes at end of SPI transfer (in case fifo is not empty). // DO NOT CHANGE !
#define ESPCAM_SPI_CHUNK_TRAILING 16

// number of microseconds before we retry a DMA transfer
#define ESPCAM_DMA_RETRY_US 10000

// number of milliseconds to turn off the module for power cycling
#define ESPCAM_POWERCYCLING_DELAY_MS 1000

// number of milliseconds between each check to whether we should perform powercycling.
#define ESPCAM_POWERCYCLING_CHECK_MS 100


/********* Camera settings *******************/
#include "ESPCAMConfig.h"  // includes enum for framesizes


#define ESPCAM_CAMERA_CONFIG_FRAMESIZE      ESPCAM_RES_800x600
#define ESPCAM_CAMERA_CONFIG_FRAMESIZE_LX   800
#define ESPCAM_CAMERA_CONFIG_FRAMESIZE_LY   600

#define ESPCAM_CAMERA_CONFIG_QUALITY        5 // bonne qualité

#define ESPCAM_CAMERA_CONFIG_AWB            0 // s->set_whitebal(s, 0); Désactive la balance des blancs auto
#define ESPCAM_CAMERA_CONFIG_AWB_GAIN       0 // s->set_awb_gain(s, 0); Désactive le gain RGB auto
#define ESPCAM_CAMERA_CONFIG_AEC            0 // s->set_exposure_ctrl(s, 0);  Désactive l’exposition automatique
#define ESPCAM_CAMERA_CONFIG_AGC            0 // s->set_gain_ctrl(s, 0); Désactive le gain automatique

#define ESPCAM_CAMERA_CONFIG_WB_MODE        0  // s->set_wb_mode(s, 0);  Mode auto désactivé (inutile si whitebal=0)
#define ESPCAM_CAMERA_CONFIG_AEC_VALUE      140 // Exposition manuelle: ajuste si trop sombre/clair (0 a 1200) plus grand = plus lent. 
#define ESPCAM_CAMERA_CONFIG_AGC_GAIN       0   // s->set_agc_gain(s, 0); gain electronique (mettre 0 et augmenter plutot le temps d'expo avec AEC_VALUE)

#define ESPCAM_CAMERA_CONFIG_GAINCEILING    0  // Ignoré puisque AGC off
#define ESPCAM_CAMERA_CONFIG_AEC2           0  // s->set_aec2(s, c0) ignoré car ESPCAM_CAMERA_CONFIG_AEC = 0
#define ESPCAM_CAMERA_CONFIG_AE_LEVEL       0  // s->set_ae_level(s, 0) ignoré car ESPCAM_CAMERA_CONFIG_AEC = 0

#define ESPCAM_CAMERA_CONFIG_BPC            1  // correction des pixels noirs
#define ESPCAM_CAMERA_CONFIG_WPC            1  // correction des pixels blanc
#define ESPCAM_CAMERA_CONFIG_RAW_GMA        1  // correction du gamma
#define ESPCAM_CAMERA_CONFIG_LENC           1  // Correction lentille activée
//#define ESPCAM_CAMERA_CONFIG_DENOISE        1  // Réduction du bruit (utile si image sombre)


#define ESPCAM_CAMERA_CONFIG_BRIGHTNESS     0 // 0 = neutre (-2 a 2)
#define ESPCAM_CAMERA_CONFIG_CONTRAST       0 // 0 = neutre (-2 a 2)
#define ESPCAM_CAMERA_CONFIG_SATURATION     0 // 0 = neutre (-2 a 2)
//#define ESPCAM_CAMERA_CONFIG_SHARPNESS      0  // // Amélioration des contours

//#define ESPCAM_CAMERA_CONFIG_SPECIAL_EFFECT 0 // 0 = normal (1-7 = effets spéciaux)
//#define ESPCAM_CAMERA_CONFIG_HMIRROR        0 // miroir horizontal
//#define ESPCAM_CAMERA_CONFIG_VFLIP          0 // miroir vertical
//#define ESPCAM_CAMERA_CONFIG_COLORBAR       0 // 0 = pas de barre de couleur
#define ESPCAM_CAMERA_CONFIG_DCW            1 // 0 = pas de DCW (DCT) (1 = DCW activé)


/*********************************************************
* AUDIO
**********************************************************/

// number of memory blocks for the Audio library (1 bloc = 260 bytes)
// 12 blocks = 3120b (about 3K)
#define AUDIO_MEMORY_BLOCKS 12	

#define ESPEAK_STACK_SIZE 10000 // size of the stack for espeak-ng

#define SOUND_LEVEL 1.0f //1.1f //1.2f // amplication factor for all sound (music + voice)

#define DEFAULT_SOUND_LEVEL_MUSIC  1.0f // default sound amplification for music
#define DEFAULT_SOUND_LEVEL_ENDING_MUSIC  0.25f // default sound amplification for 'ending' music

#define DEFAULT_SOUND_LEVEL_VOICE  0.7f // default sound amplification for voice

#define AUDIOLED_VOICE_BRIGHTNESS 0.7f // brightness factor for the audio led when driven by voice
#define AUDIOLED_MUSIC_BRIGHTNESS 0.8f // brightness factor for the audio led when driven by music

#define TRANSITION_TAG_SOUND_LEVEL 0.5f // sound amplification for transition tags
#define START_SOLVE_SOUND_LEVEL 0.6f // sound amplification for start solve sentence
#define SHUTDOWN_ABORT_SOUND_LEVEL 0.6f // sound amplification for start solve sentence
#define PANIC_BEEP_SOUND_LEVEL 0.8f // sound amplification for start solve sentence

#define ANIMATION_IDLE_SOLVE_SOUND_LEVEL 0.12f
#define ANIMATION_DREAMING_SOUND_LEVEL 0.16f 
#define ANIMATION_REPLAY_SOUND_LEVEL 0.25f


#define FADE_IN_TIME_MS 100     // default fade time in duration in ms
#define FADE_IN_ENDING_TIME_MS 1000 // default fade time in duration in ms

#define FADE_OUT_TIME_MS 3000   // default fade time out duration in ms

#define SPEAK_TEXT_BUFFER_SIZE 100000 // size of the buffer for text to be spoken


/*********************************************************
* VIDEO 
**********************************************************/

// speed for spi communication with screen
#define ILI9341_SPI_SPEED 40000000

// portrait mode 240 x320
#define ILI9341_ORIENTATION 3

// target refreshrate
#define ILI9341_REFRESHRATE 120

// vsync spacing
#define ILI9341_VSYNC_SPACING 1

// size for the 2 diff buffers for the ILI9341_T4 driver
#define ILI9341_DIFFBUFFER_SIZE 8192

// diffbuffer gap
#define ILI8341_DIFFGAP 4

// touch calibation 
#define ILI9341_TOUCH_CALIBRATION {3817, 426, 3957, 569}


/*********************************************************
* 3D
**********************************************************/

// size of the cube stickers in pixels (64x64)
#define CUBE_TEXTURE_LX 64 

// number of stickers in the cube
#define CUBE_NB_STICKERS 64 



/*********************************************************
* CUBE SOLVING
**********************************************************/

#define TRIVAL_SOLVE_MAX_MOVES 4 // the solution is considered trivial if it uses at most this number of move

#define MODERATE_SOLVE_MAX_MOVES 12 // the solution is considered moderate if it uses at most this number of move

#define COUNTDOWN_MIN_MOVES 3 // countdown is spoken only when there is at least this number of move remaining. 



/*********************************************************
* RNG
**********************************************************/

# define RESET_RNG_SEED 1234567 // seed value for the rng for factory reset. 


/*********************************************************
* NVS PARAMETERS / EEPROM
**********************************************************/

#define NVS_SIZE 4000

// previous seed value for the rng
#define NVS_ADR_PREV_SEED                   0

// keep track of the previous transiton tags used
#define NVS_ADR_TRANSITION_TAG              4
#define NVS_ADR_TRANSITION_TAG_LEN          12


// keep track of the previous sentence spoken
#define NVS_ADR_GREETING                    20
#define NVS_ADR_GREETING_LEN                20



#define NVS_ADR_CURIOUS                     40
#define NVS_ADR_CURIOUS_LEN                 20

#define NVS_ADR_PLAYFUL                     60
#define NVS_ADR_PLAYFUL_LEN                 12

#define NVS_ADR_LOGIC                       80
#define NVS_ADR_LOGIC_LEN                   12

#define NVS_ADR_EXISTENTIAL                 100
#define NVS_ADR_EXISTENTIAL_LEN             16

#define NVS_ADR_HAUNTING                    120
#define NVS_ADR_HAUNTING_LEN                16



#define NVS_ADR_DRYLOGIC                    140
#define NVS_ADR_DRYLOGIC_LEN                12

#define NVS_ADR_MOCKING                     160
#define NVS_ADR_MOCKING_LEN                 20



#define NVS_ADR_VICTORY_NOCUBE              180
#define NVS_ADR_VICTORY_NOCUBE_LEN          13

#define NVS_ADR_VICTORY_INVALID             200
#define NVS_ADR_VICTORY_INVALID_LEN         12

#define NVS_ADR_VICTORY_ALREADYSOLVED       220
#define NVS_ADR_VICTORY_ALREADYSOLVED_LEN   13

#define NVS_ADR_VICTORY_NORMAL              240
#define NVS_ADR_VICTORY_NORMAL_LEN          14



#define NVS_ADR_RES_TRIVIAL                 260
#define NVS_ADR_RES_TRIVIAL_LEN             20

#define NVS_ADR_RES_TRIVIAL_TAIL            280
#define NVS_ADR_RES_TRIVIAL_TAIL_LEN        16

#define NVS_ADR_RES_MODERATE                300
#define NVS_ADR_RES_MODERATE_LEN            20

#define NVS_ADR_RES_MODERATE_TAIL           320
#define NVS_ADR_RES_MODERATE_TAIL_LEN       16



#define NVS_ADR_SOLVE_START                 340
#define NVS_ADR_SOLVE_START_LEN             15


#define NVS_ADR_RES_MUSIC_ABORT             360
#define NVS_ADR_RES_MUSIC_ABORT_LEN         7


#define NVS_ADR_SONG_SOLVE                  380
#define NVS_ADR_SONG_SOLVE_LEN              16

#define NVS_ADR_SONG_ENDING                 400
#define NVS_ADR_SONG_ENDING_LEN             16


#define NVS_ADR_SHUTDOWN_ABORTED            440
#define NVS_ADR_SHUTDOWN_ABORTED_LEN        18


#define NVS_ADR_TAG_IDLE_SOLVE              460   
#define NVS_ADR_TAG_IDLE_SOLVE_LEN          14



#define NVS_ADR_POWER_ON                    480   
#define NVS_ADR_POWER_ON_LEN                13

#define NVS_ADR_POWER_OFF                   500   
#define NVS_ADR_POWER_OFF_LEN               16

#define NVS_ADR_BATTERY_CRITICAL            520
#define NVS_ADR_BATTERY_CRITICAL_LEN        13

#define NVS_ADR_BATTERY_OK                  540
#define NVS_ADR_BATTERY_OK_LEN              13

#define NVS_ADR_BATTERY_CHARGING            560
#define NVS_ADR_BATTERY_CHARGING_LEN        18


#define NVS_ADR_TAG_DREAMING                580   
#define NVS_ADR_TAG_DREAMING_LEN            8

#define NVS_ADR_REPLAYCUBE                  600
#define NVS_ADR_REPLAYCUBE_LEN              14


#define NVS_ADR_SONG_DREAMING               620
#define NVS_ADR_SONG_DREAMING_LEN           7

#define NVS_ADR_SONG_IDLESOLVE              640
#define NVS_ADR_SONG_IDLESOLVE_LEN          6



/*********************************************************
* MISC
**********************************************************/


#define CODE_NOTHING            0

#define CODE_TOUCHED_SCREEN     1
#define CODE_SPINNED            2

#define CODE_CHARGING_START     3  
#define CODE_CHARGING_STOP      4

#define CODE_NO_CHARGING        5
#define CODE_CHARGING           6

#define CODE_CUBE_MISSING           1
#define CODE_CUBE_INVALID           2
#define CODE_CUBE_ALREADY_SOLVED    3
#define CODE_CUBE_TRIVIAL           4
#define CODE_CUBE_MODERATE          5
#define CODE_CUBE_FULL              6


/** end of file */





