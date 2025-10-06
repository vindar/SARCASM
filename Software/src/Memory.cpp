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

#include "Memory.h"
#include "Config.h"
#include "debug.h"
#include "teensythread.h"

#include <Arduino.h>




/**************************
* Thanks to
* https://forum.pjrc.com/threads/63339-How-to-watch-for-out-of-memory-errors?highlight=monitor+stack
*********************"******/

// from the linker
//  extern unsigned long _stextload;
extern unsigned long _stext;
extern unsigned long _etext;
//  extern unsigned long _sdataload;
extern unsigned long _sdata;
extern unsigned long _edata;
extern unsigned long _sbss;
extern unsigned long _ebss;
//  extern unsigned long _flexram_bank_config;
extern unsigned long _estack;




namespace Memory
{


/******************************************************
* DMAMEM
*******************************************************/


// main DMA buffer (479Kb)
DMAMEM DMASharedLayout dma_shared;

DMAMEM int espeak_stack[ESPEAK_STACK_SIZE / 4]; 


// serial buffer for communicating with the ESPCAM. 
DMAMEM uint8_t buffer_rx_serial_espcam[ESPCAM_RX_buffer_size];
DMAMEM uint8_t buffer_serialpacket_espcam[ESPCAM_SerialPacket_buffer_size];


/******************************************************
* DTCM
*******************************************************/

// memory for led strip
int led_display_memory[LED_COUNT * 8];
int led_drawing_memory[LED_COUNT * 8];


// memory for espcam transfer buffers
int buffer_espcam_dma1[(ESPCAM_SPI_CHUNK_SIZE + 2 * ESPCAM_SPI_CHUNK_TRAILING) / 4];
int buffer_espcam_dma2[(ESPCAM_SPI_CHUNK_SIZE + 2 * ESPCAM_SPI_CHUNK_TRAILING) / 4];

// diff buffer for the display
uint8_t buffer_diff1[ILI9341_DIFFBUFFER_SIZE];
uint8_t buffer_diff2[ILI9341_DIFFBUFFER_SIZE];

uint16_t framebuffer_main[ILI9341_LX * ILI9341_LY];    // main framebuffer 

// stack for basic moves (tumbling, turning, etc.)
int basicmoves_stack[BASICMOVES_STACK_SIZE / 4];



/******************************************************
* EXTMEM (8MB)
*******************************************************/


EXTMEM uint16_t zbuffer_extmem[ILI9341_LX * ILI9341_LY];               // buffer moved from DMAMEM to EXTMEM  when kociemba is active
EXTMEM uint16_t framebuffer_extmem[ILI9341_LX * ILI9341_LY];           //
EXTMEM uint16_t framebuffer_extmem_2[ILI9341_LX * ILI9341_LY];         //

EXTMEM uint32_t camerabuffer_extmem[ESPCAM_CAMERA_CONFIG_FRAMESIZE_LX* ESPCAM_CAMERA_CONFIG_FRAMESIZE_LY]; // camera buffer in extmem

EXTMEM uint16_t cube_stickers_texture[CUBE_TEXTURE_LX * CUBE_TEXTURE_LX * CUBE_NB_STICKERS]; // texture for the cube stickers (64x64 pixels, 64 stickers)

EXTMEM debug::CoutStr panic_cout_extmem;
EXTMEM debug::CoutStr panic_cout_extmem2;

// for text formatting
EXTMEM char speak_text_buffer[SPEAK_TEXT_BUFFER_SIZE];

// list of moves in the previous solution used later for playback on the screen (empty = 0 if none).
EXTMEM char previous_solve_moves[100];  




SLOWFUN void init()
	{
	static int8_t _init_done = 0;
	if (_init_done) return;
	_init_done = 1;
	DEBUG("Memory::init() started")
	memset(&dma_shared, 77, sizeof(dma_shared));

	cube_stickers_texture[0] = 0; 
	camerabuffer_extmem[0] = 0; 

    memset(previous_solve_moves, 0, sizeof(previous_solve_moves));

	teensythread::fillStack(Memory::espeak_stack, ESPEAK_STACK_SIZE);
	teensythread::fillStack(Memory::basicmoves_stack, BASICMOVES_STACK_SIZE);


	info(); 
	stackUsage();
	DEBUG("Memory::init() completed\n")
	}





SLOWFUN void info() 
	{
	#if defined(DEBUG_MEMORY)
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Warray-bounds"
	uint32_t flexram_config = IOMUXC_GPR_GPR17;
	INFO("IOMUXC_GPR_GPR17: " << debug::Hex(flexram_config) << "  IOMUXC_GPR_GPR16: "<< debug::Hex(IOMUXC_GPR_GPR16) << "  IOMUXC_GPR_GPR14: "<< debug::Hex(IOMUXC_GPR_GPR14) << "\n");	
	INFO("Initial Stack pointer: " << debug::Hex(& _estack) << "\n");
	uint32_t dtcm_size = 0;
	uint32_t itcm_size = 0;
	for (; flexram_config; flexram_config >>= 2) 
		{
		if ((flexram_config & 0x3) == 0x2) dtcm_size += 32768;
		else if ((flexram_config & 0x3) == 0x3) itcm_size += 32768;
		}
	INFO("ITCM allocated: "  << itcm_size <<"  DTCM allocated: "<< dtcm_size <<"\n");	
	INFO("ITCM init range: " << debug::Hex(& _stext) << " - " << debug::Hex(&_etext) << " Count: " << (uint32_t)&_etext - (uint32_t)&_stext << "\n");
	INFO("DTCM init range: " << debug::Hex(&_sdata)  << " - " << debug::Hex(&_edata) << " Count: " << (uint32_t)&_edata - (uint32_t)&_sdata << "\n");		
	INFO("DTCM cleared range: "<< debug::Hex(&_sbss) << " - " << debug::Hex(&_ebss) << " Count: " << (uint32_t)&_ebss - (uint32_t)&_sbss <<"\n");
	INFO("Now fill rest of DTCM with known pattern " << debug::Hex(&_ebss + 1) << " - " << debug::Hex(&itcm_size - 10) << "\n");
	INFO("maximum stack size " << ((uint32_t)&_estack) - (uint32_t)(&_ebss + 1)  << "\n\n");
	// Guess of where it is safe to fill memory... Maybe address of last variable we have defined - some slop...
	for (uint32_t* pfill = (&_ebss + 32); pfill < (&itcm_size - 10); pfill++) 
		{
		*pfill = 0x01020304;  // some random value
		}
	#pragma GCC diagnostic pop		
	#endif
	}



SLOWFUN void stackUsage()
	{	
	#if defined(DEBUG_MEMORY)
	uint32_t* pmem = (&_ebss + 32);
	while (*pmem == 0x01020304) pmem++;
	INFO("Estimated max stack usage: " << (uint32_t)&_estack - (uint32_t)pmem << "\n");
	INFO("still available : " << (uint32_t)pmem - (uint32_t)(&_ebss + 1) << "\n\n");
	#endif
	}




}

/* end of file */ 