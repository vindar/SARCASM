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

#include <TeensyThreads.h>

#include "graphics3D.h"
#include "config.h"
#include "debug.h"
#include "Memory.h"
#include "panic.h"
#include "display.h"
#include "Console.h"
#include "BasicMoves.h"
#include "UI.h"

#include "font_SourceCodePro_AA4.h"



namespace graphics3D
    {


    RubikCube3D cube3D; // the 3D cube object

    tgx::Renderer3D<tgx::RGB565, TGX_3D_LOADED_SHADERS, uint16_t> renderer; // the 3D renderer object

    static tgx::Image<tgx::RGB565> _image3D; // the image inserted into the 3D renderer


    /** Return the sticker image associated with a given index */
    tgx::Image<tgx::RGB565> stickerImage(int index)
        {
        if (index < 0 || index >= CUBE_NB_STICKERS)
            {
            index = CUBE_NB_STICKERS-1;
            }
        tgx::Image<tgx::RGB565> tex = Display::image_stickers_texture;
        return tex.getCrop({ 0, CUBE_TEXTURE_LX-1, (index*CUBE_TEXTURE_LX), (index*CUBE_TEXTURE_LX) + CUBE_TEXTURE_LX - 1 });
        }


    SLOWFUN void applyStickerMask(int sticker_index, tgx::RGB565 boundary_color, int dist, int corner_radius, float opacity)
        {
        auto sticker = stickerImage(sticker_index); // get the sticker image
        tgx::Image<tgx::RGB565> tmpim = Display::image_stickers_tmp; // get the temporary image
        const tgx::RGB565 mask_color = tgx::RGB565_White; // the color to use for masking
        tmpim.clear(boundary_color); // clear the temporary image
        tmpim.fillRoundRect( { dist, CUBE_TEXTURE_LX - 1 - dist, dist, CUBE_TEXTURE_LX - 1 - dist }, corner_radius, mask_color);
        sticker.blitMasked(tmpim, mask_color, { 0,0 }, opacity);
        return;
        }



    SLOWFUN void resetBlankStickers(bool applyMask, bool applyNumbering)
        {
        tgx::Image<tgx::RGB565> tex = Display::image_stickers_texture;
        tex.clear(colorCubeFace); // clear the texture with the face color
        for (int i = 0; i < 54; i++)
            {
            if (applyMask) applyStickerMask(i, colorCubeEdges);
            if (applyNumbering)
                {
                char le[8] = { 'U', 'R', 'F', 'D', 'L', 'B', 'X', 'Y' }; // the letters for the stickers            
                char tt[3];
                tt[0] = le[i / 9];
                tt[1] = '1' + (i % 9);
                tt[2] = 0;
                auto sticker = stickerImage(i); // get the sticker image
                sticker.drawTextEx(tt, { 32,32 }, font_SourceCodePro_AA4_32, tgx::Anchor::CENTER, false, false, tgx::RGB565_Red, 1.0f); // draw the sticker letter on the sticker image
                }
            }
        }


    SLOWFUN void resetColorStickers(bool applyMask)
        {
        tgx::Image<tgx::RGB565> tex = Display::image_stickers_texture;
        tex.clear(colorCubeFace); // clear the texture with the face color
        for (int i = 0; i < 54; i++)
            {
            auto sticker = stickerImage(i); // get the sticker image
            tgx::RGB565 col;
            switch (i / 9)
                {
                case 0: { col = tgx::RGB565_White; break; } // U : white
                case 1: { col = tgx::RGB565_Red; break; } // R : red
                case 2: { col = tgx::RGB565_Green; break; } // F : green
                case 3: { col = tgx::RGB565_Yellow; break; } // D : yellow
                case 4: { col = tgx::RGB565_Orange; break; } // L : orange
                case 5: { col = tgx::RGB565_Blue; break; } // B : blue
                }
            sticker.clear(col); // clear the sticker with the face color
            if (applyMask) applyStickerMask(i, colorCubeEdges);
            }
        }




    /**
    * Init the 3D graphics system (renderer)
    **/
    SLOWFUN void init3D()
        {
        static int _initdone = 0; 
        if (_initdone) return; // already initialized
        _initdone = 1;
        DEBUG("graphics3D::init() started")

        // setup the 3D renderer.
        setRenderer(true, Display::image_main);
        renderer.setZbuffer(Memory::dma_shared.A.zbuffer); // set the z buffer for depth testing
        renderer.setShaders(tgx::SHADER_GOURAUD);
        renderer.setLight(GRAPHICS3D_LIGHT_DIRECTION, GRAPHICS3D_LIGHT_AMBIANT, GRAPHICS3D_LIGHT_DIFFUSE, GRAPHICS3D_LIGHT_SPECULAR);

        // setup the cube        
        resetBlankStickers(false, false); 
        tgx::Image<tgx::RGB565> tex = Display::image_stickers_texture;
        cube3D.init(tex, CUBE_NB_STICKERS, CUBE_NB_STICKERS-1, colorCubeEdges);
        const int def_stickers[54] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53 }; // default sticker values (0 to 53)
        cube3D.setStickers_int(def_stickers);

        graphics3D::resetColorStickers(true);
        graphics3D::cube3D.resetPosition();

        DEBUG("graphics3D::init() completed\n")
        }



    #define RENDTRANSITION_MIN_MS 200
    #define RENDTRANSITION_DURATION_MS 1500

    #define REND_BIG_CX 160
    #define REND_BIG_CY 140
    #define REND_BIG_R  100

    #define REND_BIG_LX 0.0f
    #define REND_BIG_LY 25.0f
    #define REND_BIG_LZ 36.0f

    #define REND_SMALL_CX 260
    #define REND_SMALL_CY 80
    #define REND_SMALL_R  66

    #define REND_SMALL_LX 10.0f
    #define REND_SMALL_LY 25.0f
    #define REND_SMALL_LZ 36.0f

    #define CUBE_SCALE_X 3.2f
    #define CUBE_SCALE_Y 3.2f
    #define CUBE_SCALE_Z 3.2f


    static elapsedMillis _rendscale_em; 
    static volatile int _rendtransition = 0; // 0 = big, 1 = going from big to small, 2 = small, 3 = going from small to big
    static volatile uint32_t _lasttouched = 0;

    static volatile char _make_transition = 0; // set to 1 to trigger a transition (if none in progress). 
    static volatile char _allow_touch_transition = 1; // set to 0 to disable touch based transitions.

    SLOWFUN void setRenderer(int state, const tgx::Image<tgx::RGB565> & dest_im)
        {
        const uint32_t nt = Display::touchedTime(); 
        if (nt > _lasttouched) { _lasttouched = nt; } // discard touch event. 
        _rendtransition = state;
        _rendscale_em = 0; // reset the transition timer
        updateRenderer(dest_im);
        }

    SLOWFUN void allowTouchTransition(bool allow)
        {
        _allow_touch_transition = allow ? 1 : 0;
        _lasttouched = Display::touchedTime();
        }

    SLOWFUN void makeRendererTransition()
        {
        _make_transition = 1; 
        }


    void makeRendererTransitionToBig()
        {
        if ((_rendtransition == 1) || (_rendtransition == 2)) { _make_transition = 1; }
        }


    void makeRendererTransitionToSmall()
        {
        if ((_rendtransition == 0) || (_rendtransition == 3)) { _make_transition = 1; }
        }




    SLOWFUN void updateRenderer(const tgx::Image<tgx::RGB565> & dest_im)
        {        
        const uint32_t nt = Display::touchedTime(); // get the last touch time.
        if ((nt > _lasttouched)||(_make_transition))
            { // differs from previous record.             
            if ((_rendtransition == 0)&&(((nt > _lasttouched + RENDTRANSITION_MIN_MS)&& (_allow_touch_transition)) || (_make_transition)))
                { // currently big, start transitiin big -> small
                _rendscale_em = 0; 
                _rendtransition = 1; // set the transition to going small
                }
            else if ((_rendtransition == 2) && (((nt > _lasttouched + RENDTRANSITION_MIN_MS) && (_allow_touch_transition))||(_make_transition)))
                { // currently small, start transition small -> big
                _rendscale_em = 0;
                _rendtransition = 3; // set the transition to going big
                }
            _lasttouched = nt; // update the last touched value
            _make_transition = 0;
            }
        int cx = REND_BIG_CX, cy = REND_BIG_CY, cr = REND_BIG_R;
        float lx = REND_BIG_LX, ly = REND_BIG_LY, lz = REND_BIG_LZ;
        switch (_rendtransition)
            {
            case 0: 
                {
                cx = REND_BIG_CX;
                cy = REND_BIG_CY;
                cr = REND_BIG_R;
                lx = REND_BIG_LX;
                ly = REND_BIG_LY;
                lz = REND_BIG_LZ; // big cube
                break;
                }
            case 1:
                {
                float r = ((float)((int)_rendscale_em)) / RENDTRANSITION_DURATION_MS; // ratio of the transition
                if (r >= 1.0f) { r = 1.0f; _rendtransition = 2; }
                if (r < 0.5f) { r = 2 * r * r; }else { r = 1 - 2 * (1-r)*(1-r); } // ease in and out
                cx = (int)(REND_BIG_CX + (REND_SMALL_CX - REND_BIG_CX) * r);
                cy = (int)(REND_BIG_CY + (REND_SMALL_CY - REND_BIG_CY) * r);
                cr = (int)(REND_BIG_R + (REND_SMALL_R - REND_BIG_R) * r);
                lx = REND_BIG_LX + (REND_SMALL_LX - REND_BIG_LX) * r;
                ly = REND_BIG_LY + (REND_SMALL_LY - REND_BIG_LY) * r;
                lz = REND_BIG_LZ + (REND_SMALL_LZ - REND_BIG_LZ) * r;                
                break;
                }
            case 2:
                {
                cx = REND_SMALL_CX;
                cy = REND_SMALL_CY;
                cr = REND_SMALL_R;
                lx = REND_SMALL_LX;
                ly = REND_SMALL_LY;
                lz = REND_SMALL_LZ; // small cube
                break;
                }
            case 3: 
                {
                float r = ((float)((int)_rendscale_em)) / RENDTRANSITION_DURATION_MS; // ratio of the transition
                if (r >= 1.0f) { r = 1.0f; _rendtransition = 0; }
                if (r < 0.5f) { r = 2 * r * r; } else { r = 1 - 2 * (1 - r) * (1 - r); } // ease in and out
                cx = (int)(REND_SMALL_CX + (REND_BIG_CX - REND_SMALL_CX) * r);
                cy = (int)(REND_SMALL_CY + (REND_BIG_CY - REND_SMALL_CY) * r);
                cr = (int)(REND_SMALL_R + (REND_BIG_R - REND_SMALL_R) * r);
                lx = REND_SMALL_LX + (REND_BIG_LX - REND_SMALL_LX) * r;
                ly = REND_SMALL_LY + (REND_BIG_LY - REND_SMALL_LY) * r;
                lz = REND_SMALL_LZ + (REND_BIG_LZ - REND_SMALL_LZ) * r;
                break;
                }
            default:
                ABORT("graphics3D::_setRenderer() : unknown _rendtransiton" << _rendtransition);
            }
        _image3D = dest_im.getCrop(tgx::iBox2(cx-cr, cx+cr-1, cy-cr, cy+cr-1));
        renderer.setLight(GRAPHICS3D_LIGHT_DIRECTION, GRAPHICS3D_LIGHT_AMBIANT, GRAPHICS3D_LIGHT_DIFFUSE, GRAPHICS3D_LIGHT_SPECULAR);
        renderer.setViewportSize(_image3D.lx(), _image3D.ly());
        renderer.setOffset(0, 0); //  image = viewport
        renderer.setImage(&_image3D); // set the image to draw onto (ie the screen framebuffer)
        renderer.setPerspective(45, ((float)_image3D.lx()) / _image3D.ly(), 1.0f, 1000.0f);  // set the perspective projection matrix.     
        renderer.setLookAt({ lx, ly, lz }, { 0,0,0 }, { 0,1,0 }); // set the camera position and orientation
        }


    SLOWFUN void updateAll(int type_move, int turn, const tgx::fMat4* M)
        {
        updateAll(Display::image_main, Display::image_alt, type_move, turn, M);
        }


    SLOWFUN void updateAll(const tgx::Image<tgx::RGB565>& im_dest, const tgx::Image<tgx::RGB565>& ui_blank, int type_move, int turn, const tgx::fMat4* M)
        {
        if (type_move <= -1) return;
        if (type_move == 0)
            {
            tgx::Image<tgx::RGB565> im_ui = Display::image_alt; 
            UI::drawMainUI(im_ui);
            }

        Console::drawFromBlank(ui_blank, im_dest); // erase everything and redraw the background, console, voltage and activity        
        tgx::fMat4 M2;
        if (type_move != 0)
            { // we will do some 3D...
            graphics3D::renderer.clearZbuffer();
            updateRenderer(im_dest);
            if (M == nullptr)
                {
                M2.setScale(CUBE_SCALE_X, CUBE_SCALE_Y, CUBE_SCALE_Z);
                }
            else
                {
                M2 = *M; // use the provided matrix
                M2.multScale(CUBE_SCALE_X, CUBE_SCALE_Y, CUBE_SCALE_Z);
                }
            }
        switch (type_move)
            {
            case 0:
                break;
            case 1:
                graphics3D::cube3D.drawCube(M2, graphics3D::renderer); // draw the cube at its current position
                break;
            case 2:
                graphics3D::cube3D.draw_tumble(M2, graphics3D::renderer, BasicMoves::tumble_progress()); // draw the cube tumbling
                break;
            case 3:
                graphics3D::cube3D.draw_turn(M2, graphics3D::renderer, turn, BasicMoves::turn_progress()); // draw the cube turning
                break;
            case 4:
                graphics3D::cube3D.draw_turn_bottom(M2, graphics3D::renderer, turn, BasicMoves::turn_progress()); // draw the cube turning
                break;
            default:
                ABORT("updateAll() : unknown type_move: " << type_move);
            }
        Display::update(im_dest);
        return;
        }






    }

/** end of file */
