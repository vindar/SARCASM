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


// default speed for serial after booting the EXPCAM
#define ESPCAM_DEFAULT_SERIAL_SPEED 115200

#define ESPCAM_GREETING_CODE 123456789

#define ESPCAM_MALFUNCTION_CODE 666666666



/* resolutions. Redefine from sensor.h in case enum change. */
enum espcam_resolution
    {
    ESPCAM_RES_160x120 = 0,
    ESPCAM_RES_176x144 = 2,
    ESPCAM_RES_240x176 = 3,
    ESPCAM_RES_320x240 = 4,
    ESPCAM_RES_400x296 = 5,
    ESPCAM_RES_640x480 = 6,
    ESPCAM_RES_800x600 = 7,
    ESPCAM_RES_1024x768 = 8,
    ESPCAM_RES_1280x1024 = 9,
    ESPCAM_RES_1600x1200 = 10,
    ESPCAM_RES_2048x1536 = 11
    };



/** commands for espcam */
enum espcam_commands
    {
    ESPCAM_COMMAND_PING = 1,
    ESPCAM_COMMAND_SERIAL_SPEED = 2,
    ESPCAM_COMMAND_SPI_SPEED = 3,
    ESPCAM_COMMAND_SPI_CHUNK_SIZE = 4,
    ESPCAM_COMMAND_FLASHLIGHT = 5,
    ESPCAM_COMMAND_FRAMESIZE = 6,
    ESPCAM_COMMAND_QUALITY = 7,
    ESPCAM_COMMAND_CONSTRAST = 8,
    ESPCAM_COMMAND_BRIGHTNESS = 9,
    ESPCAM_COMMAND_SATURATION = 10,
    ESPCAM_COMMAND_GAINCEILING = 11,
    ESPCAM_COMMAND_COLORBAR = 12,
    ESPCAM_COMMAND_AWB = 13,
    ESPCAM_COMMAND_AGC = 14,
    ESPCAM_COMMAND_AEC = 15,
    ESPCAM_COMMAND_HMIRROR = 16,
    ESPCAM_COMMAND_VFLIP = 17,
    ESPCAM_COMMAND_AWB_GAIN = 18,
    ESPCAM_COMMAND_AGC_GAIN = 19,
    ESPCAM_COMMAND_AEC_VALUE = 20,
    ESPCAM_COMMAND_AEC2 = 21,
    ESPCAM_COMMAND_DCW = 22,
    ESPCAM_COMMAND_BPC = 23,
    ESPCAM_COMMAND_WPC = 24,
    ESPCAM_COMMAND_RAW_GMA = 25,
    ESPCAM_COMMAND_LENC = 26,
    ESPCAM_COMMAND_SPECIAL_EFFECT = 27,
    ESPCAM_COMMAND_WB_MODE = 28,
    ESPCAM_COMMAND_AE_LEVEL = 29,
    ESPCAM_COMMAND_RESET = 30,
    ESPCAM_COMMAND_SHARPNESS = 31,
    ESPCAM_COMMAND_DENOISE = 32,
    ESPCAM_COMMAND_SERIAL_SNAPSHOT = 33,
    ESPCAM_COMMAND_SPI_SNAPSHOT = 34,
    ESPCAM_COMMAND_FRAME_STATS = 35,
    ESPCAM_COMMAND_DISCARD_FRAME = 36
    };



/** end of file */