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


/****************************************************************************************************************
 * 
 * PINOUT FOR V1.2 (2024)
 * 
*****************************************************************************************************************/


/***********************************************************************************
*
* Hardware resources 
* 
*  Thread:
* 
*  [GPT1]                        : used by teensyThread
* 
*  Sensors:
*  
*  [128] QUADTIMER1 (TMR1)       : periodic sensor interrupt
*                                : -> read voltages/current/servo positions/craddle position
*                                : -> overvoltage/undervoltage protection
*                                
*  Servos et stepper:
*  
*  [80] QUADTIMER3 (TMR3)        : Servos Grip & Tumble (PWM generation)
*  
*  [64] GPT2                     : Stepper motor DRV8834 (Pulse generation)
*   
*  Outputs:
*  
*  [128] DMA interrupt           : OctoWS2811 Led strip driver.
*  [128] QUADTIMER4              : 
*  
*  [144] PIT (x1)                : ILI9341_T4 (screen driver)
*  [144] SPI FIFO interrupt      :
*  [144] DMA interrupt           :
* 
*  
*  SDcard and Audio playback ? 
* 
*  [144] QUADTIMER2 : playing the sequence ? 
* 
* Audio lib ? 
* 
* 
* Panic handler 
* [224] RTC 125ms : check for panic. IRQ_SNVS_IRQ
* 
************************************************************************************/




/***********************************************************************************
* 
* Pinout for the TeensyRubik v1.1 board.
*
* c.f. the schematic for details. 
*
************************************************************************************/



/***************************************
* PANIC
****************************************/
#define PANIC_IRQ_PRIORITY 224
#define PANIC_IRQ_NAME IRQ_SNVS_IRQ
#define PANIC_TIMER_NAME TeensyTimerTool::RTC
#define PANIC_PERIOD_US 15625 // 15625us = 16ms (64Hz)


/***************************************
* CURRENT FEEDBACK SENSORS
****************************************/
#define FBSENSORS_PERIOD_US 2500                        // poll every 2.5ms = 400Hz
#define FBSENSORS_NBSKIP_VOLTAGE 10                     // skip 10 polls before reading the voltage/current

#define FBSENSORS_TIMER_NAME    TeensyTimerTool::TMR1   // timer used for the feedback sensors
#define FBSENSORS_IRQ_NAME      IRQ_QTIMER1             // name of the associated IRQ
#define FBSENSORS_ADJUST_TIMER_FOR_OVERCLOCK 1          // **** correct the frequency when overclocking the teensy. Must be 1 when using QuadTimer (TMR) and 0 when using PIT Timer
#define FBSENSORS_IRQ_PRIORITY  128                     // priority the interrupt

#define FBSENSORS_ADC_AVERAGING        32                                  // 
#define FBSENSORS_ADC_RESOLUTION       12                                  //  
#define FBSENSORS_ADC_CONVERSION_SPEED ADC_CONVERSION_SPEED::MED_SPEED     //
#define FBSENSORS_ADC_SAMPLING_SPEED   ADC_SAMPLING_SPEED::MED_SPEED       //

#define FBSENSORS_I2C_SPI_SPEED         1000000                             // 1MHz

#define FBSENSORS_INA219_ADRESS         0x40
#define FBSENSORS_INA219_MAXCURRENT     3.4     // max current is 3.4A
#define FBSENSORS_INA219_SHUNTRESISTOR  0.1     // the sense resistor is 0.1 Ohm
#define FBSENSORS_INA219_RESOLUTION     9       // 9 bit resolution (fastest polling). 




/***************************************
* MAIN POWER
****************************************/
#define PIN_MAIN_PWR_CTRL		38		// connected to the 'OFF' pin of the Pololu mini push button module. Set this to HIGH to power off the board. 
#define PIN_BAT_VOLTAGE_FB		23		// (A9) Analog input pin for reading the main voltage (via a 23K - 10K voltage divider, clamped to 10V). -> can use both ADC 0 and 1
#define VOLTAGE_ADC             1       // ADC channel to use for reading the main voltage



/***************************************
* LED STRIPS 
****************************************/
#define PIN_LED 			39		// data input pin for the RGBX SK6812 led strip 
#define LED_COUNT			25		// number of leds in the strip
#define LED_TIMER_NAME      TeensyTimerTool::TMR4   // <- Timer used by the OctoWS2811 library (not directly by this code)
#define LED_IRQ_NAME        IRQ_QTIMER4             // <- name of the associated IRQ (reserved)
#define LED_IRQ_PRIORITY    128                     // <- priority the interrupt (reserved)



/***************************************
* SERVOS
****************************************/
#define PIN_SERVO_PWR_CTRL		22		// controls the buck converter powering the two servos. Set this to HIGH to enable the buck converter

#define PIN_SERVO_TUMBLE_PWM	15		// control the tumble servo via PWM
#define PIN_SERVO_TUMBLE_FB		17		// (A3) analog input pin for reading the tumble servo position (feedback) -> can use both ADC 0 and 1
#define SERVO_TUMBLE_ADC		0		// ADC used to read the the tumble servo position


#define PIN_SERVO_GRIP_PWM		14		// control the grip servo via PWM
#define PIN_SERVO_GRIP_FB		16		// (A2) analog input pin for reading the tumble servo position (feedback) -> can use both ADC 0 and 1
#define SERVO_GRIP_ADC			1		// ADC used to read the the tumble servo position


// IMPORTANT !!!!
// !!!! In the TeensyTimerTool library, file "default_config.h" !!!!!
//      -> change TMR_DEFAULT_PSC from PSC128 to PSC_32 to increase the resolution of the QuadTimer       
// 
#define SERVO_TIMER_NAME      TeensyTimerTool::TMR3 // timer used for the servos
#define SERVO_IRQ_NAME        IRQ_QTIMER3  // name of the associated IRQ
#define SERVO_ADJUST_TIMER_FOR_OVERCLOCK 1 // **** correct the frequency when overclocking the teensy. Must be 1 when using QuadTimer (TMR) and 0 when using PIT Timer
#define SERVO_IRQ_PRIORITY    80        // priority the interrupt



/***************************************
* STEPPER DRIVER Pololu DRV8834
****************************************/
#define PIN_DRV8834_SLEEP		35		// enable/disable the motor driver. 
#define PIN_DRV8834_DIR			37		// set the rotation direction 
#define PIN_DRV8834_STEP		36		// impulse pin for making steps
#define PIN_DRV8834_M0			33		// microstepping mode M0. (M0 = M1 = low for full steps). 
#define PIN_DRV8834_M1			34		// microstepping mode M1  See https://www.pololu.com/product/2134 for more steps modes. 

#define MOTOR_STEPS_PER_TURN    400     // number of steps per turn for the motor (here 17HM15-0904S)

#define DRV8834_DELAY_ENABLE_US 2000    // delay required when starting/stopping the motor driver (1ms according to datasheet)

#define DRV8834_DELAY_COMMAND_US 5      // delay for a command to register (2us according to datasheet). 

#define DRV8834_TIMER_NAME     TeensyTimerTool::GPT2 // timer used for the stepper motor
#define DRV8834_IRQ_NAME       IRQ_GPT2              // name of the associated IRQ
#define DRV8834_IRQ_PRIORITY   64                    // priority the interrupt


/***************************************
* ESP32-CAM
* 
* - uses Serial6
* - uses SPI0 (with teensy as slave) 
****************************************/
#define PIN_ESPCAM_POWER_CTRL	8		// high side switch via P-mosfet for the new version.
#define PIN_ESPCAM_EXTRA		2		// Unused in the new version 
#define PIN_ESPCAM_FLASH		9		// connected to esp-cam GPIO 0 (set to low to enter flashing mode at boot)
#define PIN_ESPCAM_CS			10		// SPI0. connected to esp-cam GPIO 15 (CS). 
#define PIN_ESPCAM_MISO			11		// SPI0. connected to esp-cam GPIO 12 (MISO)
#define PIN_ESPCAM_MOSI			12		// SPI0. connected to esp-cam GPIO 13 (MOSI) 
#define PIN_ESPCAM_CLK			13		// SPI0. connected to esp-cam GPIO 14 (CLK)
#define PIN_ESPCAM_RX			24		// Serial6. connected to esp-cam GPIO 3 (RX) 
#define PIN_ESPCAM_TX			25		// Serial6. connected to esp-cam GPIO 2 (TX)

#define ESPCAM_Serial           Serial6 



/***************************************
* ILI9341 screen + touchscreen 
* 
* uses SPI1
****************************************/

#define PIN_ILI9341_SCK			27		// spi clock pin (shared screen / touch)
#define PIN_ILI9341_MISO		1		// spi miso pin (shared screen / touch)
#define PIN_ILI9341_MOSI		26		// spi mosi pin (shared screen / touch)
#define PIN_ILI9341_DS			0		// data select pin for the screen
#define PIN_ILI9341_RESET		29		// screen reset pin 
#define PIN_ILI9341_LED			28		// screen led pin (use PWM to adjust brightness). 
#define PIN_ILI9341_CS			30		// chip select pin for the screen

#define PIN_ILI9341_TOUCH_IRQ	32      // disconnected !
#define PIN_ILI9341_TOUCH_CS	31    	// disconnected !

#define PIN_ILI9341_EXTRA    	3		// unused
#define ILI9341_LX              320     // screen width
#define ILI9341_LY              240     // screen height

#define ILI9341_IRQ_PRIORITY   144      // priority for the PIT timer interrupt


/***************************************
* AUDIO AMP I2S. MAX98357A
* 
* uses I2S
****************************************/
#define PIN_MAX98357A_PWR_CTRL	6		// enable/disable the MAX98357A chip. 
#define PIN_MAX98357_DIN		7		// data in. 
#define PIN_MAX98357A_LRC		20		// left/right channel selector pin
#define PIN_MAX98357A_BCLK		21		// clk pin 





/* end of file */





