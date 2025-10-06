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

#include <tgx.h>

#include "Hardware.h"
#include "Config.h"
#include "debug.h"

#include "tjpgd.h"



// decide whether debug message are send to debug serial port. 
#define ENABLE_ESPCAM_DEBUG 1


#if ENABLE_ESPCAM_DEBUG
#define ESPCAM_DEBUG(V) { DEBUG(V); }
#else 
#define ESPCAM_DEBUG(V) 
#endif 



namespace ESPCAM
{


	/**
	* Structure that holds statistics about the espcam. 
	**/
	struct FrameStat
		{
		int32_t nb_valid;           // number of valid frame received
		int32_t nb_invalid;         // number of invalid frame received
		int32_t current_retry;      // current number of consecutif invalid frame received
		int32_t max_retry;          // max number of consecutive invalid frame received
		int32_t min_size;           // minimum size of a frame in bytes
		int32_t max_size;           // maximum size of a frame in bytes
		int32_t sum_size_kb;        // sum of the size of all frame grabbed (in kb)
		};





    /**
    * Initialise the ESPCAM.
    **/
    SLOWFUN void init();


	/**
	* Method to call inside an interrupt to check if 
    * the ESPCAM requires powercycling.
    **/
	SLOWFUN void checkPowerCycling();


	/**
	* Return statistics about frames grabbed by the ESPCAM. 
	**/
	FrameStat frameStats();


	// type of the output function
	typedef int (*TJPGDEC_OUTPUT_FUN)(JDEC*, void*, JRECT*);



	/**
	* Capture an image from the camera and put it in the 
    * image im (which must have the same same size as the 
    * image from the camera).
    * 
    * set nb_retry to -1 o retry infinitely until success. 
	**/
	SLOWFUN void capture_tgx(tgx::Image<tgx::RGB32>& im, int nb_retry = -1);


	/**
	* Capture an image, which is decompressed simultaneously.
    * 
	* In case of error, the output function can be called multiple times 
    * at the same location until the image is correctly written in memory. 
    * 
    * nb_retry = -1 to retry indefinitely, otherwise, abort after nb_retry
    * 
    * (lx,ly) are the (optional) expected size of the image: if given and not matched, abort is called. 
    * 
    * errors.
	**/
	SLOWFUN tgx::iVec2 capture(TJPGDEC_OUTPUT_FUN output_fun, int nb_retry = -1, int lx = -1, int ly = -1);


	/**
	* Capture an image.
    * 
    * Copy the raw jpeg data into a buffer and make sure the jpeg file is valid. 
    * 
	* set nb_retry = -1 to retry indefinitely, otherwise, abort after nb_retry
	* errors.
	*
    * Return the size of the data buffer.
	**/
	SLOWFUN int captureraw(uint8_t* buffer, int sizebuf, int nb_retry = -1);


	/**	
	* Discard the next frame in the camera buffer. 
    * (fast, no download)
	**/
    SLOWFUN void discardframe();


	/**
    * Send a raw command to the espcam module
    **/
    SLOWFUN void sendCommand(espcam_commands command, int value);


	/**
	* Set the serial transfer speed between 9600 and 4000000
	* (2000000 is a good value).
	**/
	SLOWFUN void setSerialSpeed(int baudrate);


	/**
	* Turn the flashlight of the camera on/off.
	**/
	inline void flashlight(bool on) { sendCommand(ESPCAM_COMMAND_FLASHLIGHT, on ? 1 : 0); }


	/**
	* Set the spi transfer speed
	* (80000000 is a good value)
	**/
	inline void setSpiSpeed(int val) 
		{ 
		sendCommand(ESPCAM_COMMAND_SPI_SPEED, val);         
		DEBUG("ESPCAM Serial speed set: " << val);  
		}


	/**
	* Set the size of a chunk for spi transfer.
	* The total size transfered per spi transaction adds 2 bytes (for checksum).
	**/
	inline void setSpiChunkSize(int val) { sendCommand(ESPCAM_COMMAND_SPI_CHUNK_SIZE, val); }



	/**
	* Set the size of the image taken by the camera.
	**/
	inline void setFramesize(espcam_resolution res) { sendCommand(ESPCAM_COMMAND_FRAMESIZE, (int)res); }


	/**
	* Set the compression ratio for the jpeg image
	* 0 = best quality, 63 = worst quality
	* (from the DSP)
	**/
	inline void setQuality(int quality) { sendCommand(ESPCAM_COMMAND_QUALITY, quality); }


	/**
	* Set the contrast of the image in [-2,2]]
	* (from the DSP)
	**/
	inline void setContrast(int v) { sendCommand(ESPCAM_COMMAND_CONSTRAST, v); }


	/**
	* Set the brightness of the image in [-2,2]]
	* (from the DSP)
	**/
	inline void setBrightness(int v) { sendCommand(ESPCAM_COMMAND_BRIGHTNESS, v); }


	/**
	* Set the saturation of the image in [-2,2]]
	* (from the DSP)
	**/
	inline void setSaturation(int v) { sendCommand(ESPCAM_COMMAND_SATURATION, v); }


	/**
	* Enable/disable image downsizing. What is this ?
	* (from the DSP)
	**/
	inline void setDCW(bool b) { sendCommand(ESPCAM_COMMAND_DCW, b ? 1 : 0); }


	/**
	* Enable/disable black pixel corection.
	* (from the DSP)
	**/
	inline void setBPC(bool b) { sendCommand(ESPCAM_COMMAND_BPC, b ? 1 : 0); }


	/**
	* Enable/disable white pixel corection.
	* (from the DSP)
	**/
	inline void setWPC(bool b) { sendCommand(ESPCAM_COMMAND_WPC, b ? 1 : 0); }


	/**
	* Enable/disable raw gamma corection.
	* (from the DSP)
	**/
	inline void setRawGamma(bool b) { sendCommand(ESPCAM_COMMAND_RAW_GMA, b ? 1 : 0); }


	/**
	* Enable/disable lense corection.
	* (from the DSP)
	**/	
	inline void setLenseCorrection(bool b) { sendCommand(ESPCAM_COMMAND_LENC, b ? 1 : 0); }


	/**
	* Enable/disable special effects
	* (from the DSP)
	* 0 - no effect (default);
	* 1 - negative
	* 2 - black and white
	* 3 - reddish
	* 4 - greenish
	* 5 - blue
	* 6 - retro
	**/
	inline void setSpecialEffect(int e) { sendCommand(ESPCAM_COMMAND_SPECIAL_EFFECT, e); }


	/**
	* Enable/disable horizontal flip of the image
	**/
	inline void setHorizontalFlip(bool b) { sendCommand(ESPCAM_COMMAND_HMIRROR, b ? 1 : 0); }


	/**
	* Enable/disable vertical flip of the image
	**/
	inline void setVerticalFlip(bool b) { sendCommand(ESPCAM_COMMAND_VFLIP, b ? 1 : 0); }


	/**
	* Enable/disable the colorbar
	**/
	inline void setColorbar(bool b) { sendCommand(ESPCAM_COMMAND_COLORBAR, b ? 1 : 0); }



	/**********************
	* GAIN CONTROL
	**********************/

	/**
	* Enable/disable automatic gain control. Set to false to manually adjust it.
	* (from the sensor)
	**/
	inline void setGainCtrl(bool b) { sendCommand(ESPCAM_COMMAND_AGC, b ? 1 : 0); }


	/**
	* Manual setting for the gain when automatic gain control is disabled.
	* value in [0,30] (4 is a good choice).
	* (from the sensor)
	**/
	inline void setAGCGain(int g) { sendCommand(ESPCAM_COMMAND_AGC_GAIN, g); }


	/**
	* Set the 'gainceiling value'. Useless when not in auto mode ?
	* value in [0,6].
	* (from the sensor)
	**/
	inline void setGainceiling(int gc) { sendCommand(ESPCAM_COMMAND_GAINCEILING, gc); }


	/**********************
	* EXPOSURE CONTROL
	**********************/

	/**
	* Enable/disable auto exposure
	* (from the sensor)
	**/
	inline void setExposureCtrl(bool b) { sendCommand(ESPCAM_COMMAND_AEC, b ? 1 : 0); }


	/**
	* Manual setting for the exposure when auto mode is off.
	* value in [0,1200] (900 is a good choice).
	* (from the sensor)
	**/
	inline void setExposureValue(int v) { sendCommand(ESPCAM_COMMAND_AEC_VALUE, v); }


	/**
	* Set the auto exposure level. Useless when not in auto mode ?
	* value in [-2,2].
	* (from the sensor)
	**/
	inline void setAutoExposureLevel(int v) { sendCommand(ESPCAM_COMMAND_AE_LEVEL, v); }


	/**
	* Enable/disable auto exposure control from the DSP.
	**/
	inline void setExposureCtrl2DSP(bool b) { sendCommand(ESPCAM_COMMAND_AEC2, b ? 1 : 0); }


	/**********************
	* WHITE BALANCE
	**********************/

	/**
	* Enable/disable automatic white balance
	**/
	inline void setAutoWhiteBalance(bool b) { sendCommand(ESPCAM_COMMAND_AWB, b ? 1 : 0); }


	/**
	* Set the white balance gain ? Why a boolean ?
	**/
	inline void setWhiteBalanceGain(bool b) { sendCommand(ESPCAM_COMMAND_AWB_GAIN, b ? 1 : 0); }


	/**
	* Set the white balance mode (0 or 1). Useless in manual white balance ?
	* 0 - default
	* 1 - sunny
	* 2 - cloudy
	* 3 - office
	* 4 - home
	**/
	inline void setWhiteBalanceMode(int m) { sendCommand(ESPCAM_COMMAND_WB_MODE, m); }






}

/** end of file */

