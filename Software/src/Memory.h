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
#include "Config.h"
#include "debug.h"
#include <stdint.h>



namespace Memory
    {



    /*************************************************
    *                   DMA MEMORY
    *
    * -> AudioMemory() also allocates its audio
    *    blocks in DMAMEM (see MAX98357A::init())
    **************************************************/


    // memory shared. 
    union DMASharedLayout
        {
        int32_t _align; // just to make sure everything is correctly aligned to word boundary. 

        // layout A : when not using kociemba.
        struct
            {
            int buffer_tjpgd_work[ESPCAM_TJPGD_WORK_SIZE / 4]; // working buffer for the jpg decompressor

            uint16_t framebuffer_internal[ILI9341_LX * ILI9341_LY];   // framebuffer (for double buffering)            
            uint16_t zbuffer[ILI9341_LX * ILI9341_LY];           // z-buffer (for 3D)
            uint16_t framebuffer_alt[ILI9341_LX * ILI9341_LY];   // framebuffer (for double buffering)

            uint16_t cube_stickers_tmp[CUBE_TEXTURE_LX * CUBE_TEXTURE_LX];
            } A;

        // layout B : when using kociemba
        struct
            {
            char kociemba_479[479 * 1024]; // 479K reserved for storing precomputed tables. 
            } B;
        };

    extern DMASharedLayout dma_shared;

    // stack for espeak-ng (in DMAMEM, always available)
    extern int espeak_stack[ESPEAK_STACK_SIZE / 4];



    // memory for serial communication with espcam (always available)
    extern uint8_t buffer_rx_serial_espcam[ESPCAM_RX_buffer_size];              // serial receive buffer for espcam (serial6)
    extern uint8_t buffer_serialpacket_espcam[ESPCAM_SerialPacket_buffer_size]; // receive buffer for Serial packet with espcam


    /*************************************************
    *                  DTCM MEMORY                   *
    *                                                *
    * -> the RubikCube3D 'cube3D' object is in DTCM  *
    **************************************************/

    // memory for led strip
    extern int led_display_memory[LED_COUNT * 8]; // 25 * 8 * 4 = 800 bytes
    extern int led_drawing_memory[LED_COUNT * 8]; // 25 * 8 * 4 = 800 bytes

    // DMA buffers for espcam SPI transfers
    extern int buffer_espcam_dma1[(ESPCAM_SPI_CHUNK_SIZE + 2 * ESPCAM_SPI_CHUNK_TRAILING) / 4];
    extern int buffer_espcam_dma2[(ESPCAM_SPI_CHUNK_SIZE + 2 * ESPCAM_SPI_CHUNK_TRAILING) / 4];

    // diff buffers for ILI9341_T4
    extern uint8_t buffer_diff1[ILI9341_DIFFBUFFER_SIZE];
    extern uint8_t buffer_diff2[ILI9341_DIFFBUFFER_SIZE];

    // framebuffer in DTCM memory (fastest, always available).
    extern uint16_t framebuffer_main[ILI9341_LX * ILI9341_LY];

    // stack for basic moves (tumbling, turning, etc.)
    extern int basicmoves_stack[BASICMOVES_STACK_SIZE / 4];


    // for text formatting
    extern char speak_text_buffer[SPEAK_TEXT_BUFFER_SIZE];



    /*************************************************
    *                    EXTMEM
    *
    * -> espeak also mallocs it memory in EXTMEM (around 150kb)
    **************************************************/


    extern "C" uint8_t external_psram_size;			// extmem memory size

    extern uint16_t zbuffer_extmem[ILI9341_LX * ILI9341_LY];               // buffer moved from DMAMEM to EXTMEM when kociemba is active
    extern uint16_t framebuffer_extmem[ILI9341_LX * ILI9341_LY];           // 
    extern uint16_t framebuffer_extmem_2[ILI9341_LX * ILI9341_LY];         //

    extern uint32_t camerabuffer_extmem[ESPCAM_CAMERA_CONFIG_FRAMESIZE_LX * ESPCAM_CAMERA_CONFIG_FRAMESIZE_LY]; // camera buffer in extmem

    extern uint16_t cube_stickers_texture[CUBE_TEXTURE_LX * CUBE_TEXTURE_LX * CUBE_NB_STICKERS]; // texture for the cube stickers (64x64 pixels, 64 stickers)

    extern debug::CoutStr panic_cout_extmem;
    extern debug::CoutStr panic_cout_extmem2;

    extern char previous_solve_moves[100];

    /*
    #define VOICE_BUFFER_SIZE	(3 * 1024 * 1024)	// overkill but we have so much memory, why not use it :)
    #define SOUND_BUFFER_SIZE	(3 * 1024 * 1024)	// overkill but we have so much memory, why not use it :)

    extern unsigned int buffer_voice[VOICE_BUFFER_SIZE / 4];
    extern unsigned int buffer_sound[SOUND_BUFFER_SIZE / 4];
    */


    /*************************************************
    *            MEMORY RELATED FUNCTIONS
    **************************************************/


    /** Initialize the memory arrays. */
    SLOWFUN void init();


    /**
    * from the teensy forum. Used to understand DTCM/ITCM partitioning.
    * enabled when DEBUG_MEMORY is defined
    **/
    SLOWFUN void info();


    /**
    * Estimate the max stack usage.
    * enabled when DEBUG_MEMORY id defined
    **/
    SLOWFUN void stackUsage();


    }

/* end of file */

