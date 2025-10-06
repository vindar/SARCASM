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
#include <TeensyThreads.h>
#include <tgx.h>
#include "config.h"




namespace Sarcasm3DAnimations
    {
    /**
    * Initialisation before performing the 'cube replay' animation
    * Set up the renderer and the cube and set the sequanece of moves.
    **/
    SLOWFUN  void InitSolvingAnimation(const tgx::Image<tgx::RGB565> & dst_im, const char* sequence);


    /**
    * Performs the 'cube replay animation' by drawing the next 3D frame.
    * Return true if the animation is finished, false otherwise
    **/
    SLOWFUN  int SolvingAnimation();

    /**
    * Return the number of moves remaining in the solving animation
    * 0 if finished.
    **/
    SLOWFUN int remainingMoves();

    /**
    * total number of moves to perform for the solving animation. 
    **/
    SLOWFUN int totalMoves();



    /**
    * Intialisation before performing the dreaming animation.
    **/
    SLOWFUN  void InitDreamingAnimation(const tgx::Image<tgx::RGB565> & dst_im);

    /**
    * Performs the dreaming animation by drawing the next 3D frame.
    * Return true if the animation is finished, false otherwise
    **/
    SLOWFUN  int DreamingAnimation();

    /**
    * Return the ration in [0,1] done for the dreaming animation.
    **/
    SLOWFUN float dreamingRatioDone();

    }

/** end of file */
