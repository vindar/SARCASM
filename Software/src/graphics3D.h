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

#include "RubikCube3D.h"
#include <tgx.h>


//#define TGX_3D_LOADED_SHADERS (tgx::SHADER_ZBUFFER | tgx::SHADER_PERSPECTIVE | tgx::SHADER_GOURAUD | tgx::SHADER_TEXTURE | tgx::SHADER_FLAT | tgx::SHADER_NOTEXTURE | tgx::SHADER_TEXTURE_NEAREST | tgx::SHADER_TEXTURE_BILINEAR | tgx::SHADER_TEXTURE_CLAMP)

#define TGX_3D_LOADED_SHADERS (tgx::SHADER_ZBUFFER | tgx::SHADER_PERSPECTIVE | tgx::SHADER_GOURAUD | tgx::SHADER_TEXTURE | tgx::SHADER_FLAT | tgx::SHADER_NOTEXTURE | tgx::SHADER_TEXTURE_BILINEAR | tgx::SHADER_TEXTURE_CLAMP)

#define GRAPHICS3D_LIGHT_DIRECTION (tgx::fVec3(-1, -2, -1))
#define GRAPHICS3D_LIGHT_AMBIANT (tgx::RGBf(1.0f, 1.0f,1.0f))
#define GRAPHICS3D_LIGHT_DIFFUSE (tgx::RGBf(1.0f, 1.0f,1.0f))
#define GRAPHICS3D_LIGHT_SPECULAR (tgx::RGBf(1.0f, 1.0f,1.0f))

namespace graphics3D
    {


    static const tgx::RGB565 colorCubeEdges = tgx::RGB32(45, 95, 79);
    static const tgx::RGB565 colorCubeFace = tgx::RGB32(65, 135, 113);

    #define CUBE3D_STICKER_MASK_WIDTH 5
    #define CUBE3D_STICKER_MASK_RADIUS 6

    extern RubikCube3D cube3D; // the 3D cube object

    extern tgx::Renderer3D<tgx::RGB565, TGX_3D_LOADED_SHADERS, uint16_t> renderer; // the 3D renderer object

    /**
    * Init the 3D graphics system (renderer)
    **/
    void init3D();


    /**
    * Updates the display. Redraw the UI, voltage and activity and then the cube (depending on type)
    * type_move : -1 : do not redraw
    *              0 : console only, no 3D cube
    *              1 : 3D cube at its current pos.
    *              2 : 3D cube, tumbling
    *              3 : 3D cube, turning  
    *              4 : 3D cube, bottom turn 
    **/
    void updateAll(int type_move, int turn = 0, const tgx::fMat4 * M = nullptr);
   
    void updateAll(const tgx::Image<tgx::RGB565>& im_dest, const tgx::Image<tgx::RGB565>& ui_blank, int type_move, int turn = 0, const tgx::fMat4 * M = nullptr);


    /**
    * Set how the 3D renderer display the cube. 
    * state = 0 : big cube
    *         1 : going from big to small  
    *         2 : small cube  
    *         3 : going from small to big  
    * when selecting 1 or 3, the timer is reset to 0 to perform the transiiton from the start. 
    **/
    void setRenderer(int state, const tgx::Image<tgx::RGB565>& dest_im);


    /** Request a transition in the renderer (from big to small or small to big). */
    void makeRendererTransition();

    /** Request a transition in the renderer to big (do nothing if already big) */
    void makeRendererTransitionToBig();

    /** Request a transition in the renderer to small (do nothing if already small) */
    void makeRendererTransitionToSmall();

    /** Enable or disable ransiton by touching the screen*/
    void allowTouchTransition(bool allow);


    /**
    * Update the 3D renderer to use the current selected display mode for the cube. 
    * (this method is automatically called by updateAll()).
    **/
    void updateRenderer(const tgx::Image<tgx::RGB565>& dest_im);



    /** 
    * Return the sticker image associated with a given index 
    **/
    tgx::Image<tgx::RGB565> stickerImage(int index);


    /**
    * Reset all the sticker images to their default 'blank' color
    * (and optionally apply a mask and numbering)
    **/
    void resetBlankStickers(bool applyMask, bool applyNumbering);


    /**
    * Reset all the stickers to the default cube colors. 
    **/
    void resetColorStickers(bool applyMask);


    /**
    * Apply the boundary mask to a given sticker
    **/
    void applyStickerMask(int sticker_index, tgx::RGB565 boundary_color = colorCubeEdges, int dist = CUBE3D_STICKER_MASK_WIDTH, int corner_radius = CUBE3D_STICKER_MASK_RADIUS, float opacity = 1.0f);


    
    }

/** end of file */
