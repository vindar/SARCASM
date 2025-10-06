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
// along with this code. If not, see <http://www.gnu.org/licenses/>.#pragma once

#include <tgx.h>



namespace CubeRead
    {

    /**
    * Return the string representation of the cube state.
    *
    * the string is statically allocated and should not be modified.
    *
    * This method can be queried after completed the cube read.
    **/
    const char* cubeString();


    /**
     * Reads a face from an image and save the color of the stickers.
     *
     * @param   im      The image.
     * @param   face    The face (0=U, 1=R, 2=F, 3=D, 4=L, 5=B)
     * @param   rot     The rotation (0 =0°, 1=90°, 2=180°, 3=270° clockwise)
     *
     * For example, if face=D and rot=1, the image (upright in portrait mode) shows:
     *
     * D7 D4 D1
     * D8 D5 D2
     * D9 D6 D3
     */
    void readFace(const tgx::Image<tgx::RGB32> & im, int face, int rot);


    /**
     * The methods can be called after having called readFace for all 6 faces. 
     * It tries to find the real color of each sticker. 
     * 
     * return true if it succeeds, false if it fails. Even if it returns true, the cube may not be solvable. 
     */
    bool findConfiguration();




    /**
    * Take pictures, rotate the cube and find the configuration.
    * 
    * return:
    * - 0 if success,
    * - 1 if the cube is missing
    * - 2 if the cube is not solvable
    **/
    int readCubeAndFindConfiguration();


    /**
    * Return the average color of a face of the cube (order 0=U, 1=R, 2=F, 3=D, 4=L, 5=B).
    * 
    * This method can only be called after a successful call to readCubeAndFindConfiguration().
    **/
    tgx::RGB565 faceColor(int face);


    /**
    * Return the average color of a sticker (0 to 53).
    * 
    * This method can only be called after a successful call to readCubeAndFindConfiguration().
    **/
    tgx::RGB565 stickerAverageColor(int sticker);


    }


