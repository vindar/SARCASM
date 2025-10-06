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

#include <Arduino.h>
#include <TeensyThreads.h>
#include <Audio.h>



namespace faceAction
    {


    /**
    * Make the face appear for the intro sequence.
    **/
    void appear_intro(float scale = 1.0f);


    /**
    * Make the face appear in ms milliseconds (-1 for random time)
    **/
    void appear(int ms = -1);
    

    /**
    * Make the face disappear in ms milliseconds (-1 for random time)    
    **/
    void disapear(int ms = -1);

    /**
    * Make the face disappear for the shutdown sequence
    **/
    void disapear_shutdown(int ms);


    /**
    * Make the face shut its eyes for a given time in ms
    **/
    void shuteyes(uint32_t ms);

    /**
    * Make the cube appear int ms milliseconds (-1 for random time)
    * 'im' must be the image the cube is drawn onto.
    **/
    void cubeAppear(tgx::Image<tgx::RGB565>& im, int ms = -1);


    /**
    * Make the cube appear int ms milliseconds (-1 for random time)
    * 'im' must be the image the cube is drawn onto.
    * go ninto solved mode (more visible, no hline/quant).
    **/
    void cubeAppearSolved(tgx::Image<tgx::RGB565>& im, int ms = 1);


    /**
    * Make the cube appear int ms milliseconds (-1 for random time)
    * 'im' must be the image the cube is drawn onto.
    **/
    void cubeAppearReplay(tgx::Image<tgx::RGB565>& im, int ms = -1);


    /**
    * Make the cube disapear in ms milliseconds (-1 for random time)
    **/
    void cubeDisappear(int ms = -1);


    /** for the dreaming animation */
    void dreamingAppear(tgx::Image<tgx::RGB565>& im, int ms);

    /** for the dreaming animation */
    void dreamingEnd(tgx::Image<tgx::RGB565>& im, int ms);

    /** for the dreaming animation */
    void dreamingDisappear(int ms);




    /**
    * Return the current face opacity (0.0 to 1.0)
    **/
    float currentFaceOpacity();

    /**
    * Return whether the current face opacity is over a given threshold.
    **/
    bool currentFaceVisible();

    /**
    * For internal use. Draw the face. 
    **/
    void drawFace(tgx::Image<tgx::RGB565>& im);


    /**
    * Use a given image instead of the face. 
    * Set to nullptr to use the face again.
    **/
    void useImageInsteadOfFace(const tgx::Image<tgx::RGB565> & src_image);

    /**
    * Go back to using face instead of a specific image.
    **/
    void useFace();


    /**
    * For internal us. 
    * Called from audio led to update the face status. 
    *  - appearance/disapperance  
    *  - lipsync
    **/
    void audioReact(audio_block_t * block);


    /**
    * Enable / disable the audio reaction for appearance/disappearance of the face
    **/
    void audioReacEnable(bool enable);


    }


/** end of file */



