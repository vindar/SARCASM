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

#include "Sarcasm3DAnimations.h"
#include "graphics3D.h"
#include "display.h"
#include "Memory.h"
#include "config.h"
#include "audioMAX98357A.h"
#include "AudioLed.h"

// the meshes to display
#include "3Dmodels/cam_small.h"
#include "3Dmodels/ILI9341_small.h"
#include "3Dmodels/mainframe_small.h"
#include "3Dmodels/cover_small.h"
#include "3Dmodels/bouton_noir_small.h"
#include "3Dmodels/bouton_rouge_small.h"
#include "3Dmodels/usb_small.h"
#include "3Dmodels/cradle_small.h"
#include "3Dmodels/reddots_small.h"
#include "3Dmodels/gripper_small.h"
#include "3Dmodels/gripper_red_small.h"
#include "3Dmodels/tumble_small.h"
#include "3Dmodels/sarcasm_aconym_small_texture.h"
#include "3Dmodels/companion_cube_texture.h"
#include "3Dmodels/companion_cube.h"



/****************** TO REMOVE *************************/




namespace Sarcasm3DAnimations
    {


    static tgx::Image<tgx::RGB565> im;  // the destinatin image
    static tgx::Image<tgx::RGB565> int_im; // internal framebuffer of the ILI9341_T4 driver. 



    SLOWFUN static void setUpRenderer(const tgx::Image<tgx::RGB565> & dst_im)
        {
        int_im.set(Memory::dma_shared.A.framebuffer_internal, ILI9341_LY, ILI9341_LX); // in portrait mode !
        im = dst_im;  // make a copy !

        graphics3D::renderer.setViewportSize(ILI9341_LX, ILI9341_LY); // viewport = screen 
        graphics3D::renderer.setOffset(0, 0); //  image = viewport
        graphics3D::renderer.setImage(&im); // set the image to draw onto (ie the screen framebuffer)
        graphics3D::renderer.setPerspective(60, ((float)(ILI9341_LX) / (ILI9341_LY)), 10.0f, 5000.0f);  // set the perspective projection matrix.     
        graphics3D::renderer.setLight(GRAPHICS3D_LIGHT_DIRECTION, GRAPHICS3D_LIGHT_AMBIANT, GRAPHICS3D_LIGHT_DIFFUSE, GRAPHICS3D_LIGHT_SPECULAR);


        //                      Z      X     Y
        //graphics3D::renderer.setLookAt({ 100*dd, -100*dd, 200*dd }, { 0,0,80*dd }, { 0,0,1 });
        }



    /**
     * Draw the SARCASM machine in 3D.
     *
     * @param           cradleRot       Rotation in degrees of the cradle
     * @param           gripperRot      Rotation in degree of the gripper (0 = full open)
     * @param           tumbleRot       Rotation in degree of the tumble (0 = horizontal, parked at +50)
     * @param           modelTransform  The model transform (post tranformatin to apply, such as a rotation of the whole model)
     * @param [in,out]  displayImage    The image to disoplay on the screen of the machine. Use the internal framebuffer by default if not set
     */
    SLOWFUN static void drawSARCASM3D(float cradleRot, float gripperRot, float tumbleRot, const tgx::fMat4& modelTransform, const tgx::Image<tgx::RGB565>* displayImage = nullptr, const bool wireframe = false)
        {
        const tgx::fVec2 t1 = { 0.0f, 1.0f }; // texture coordinates. 
        const tgx::fVec2 t2 = { 0.0f, 0.0f }; //
        const tgx::fVec2 t3 = { 1.0f, 0.0f }; //
        const tgx::fVec2 t4 = { 1.0f, 1.0f }; //

        tgx::fMat4 M;
        M.setIdentity();

        graphics3D::renderer.setModelMatrix(modelTransform * M);
        graphics3D::renderer.setShaders(tgx::SHADER_FLAT | tgx::SHADER_NOTEXTURE);

        // cover (white)
        graphics3D::renderer.setMaterial(tgx::RGBf(1.0f, 1.0f, 1.0f), 0.2f, 0.7f, 0.5f, 8);
        if (!wireframe) graphics3D::renderer.drawMesh(&cover_small, false); else graphics3D::renderer.drawWireFrameMesh(&cover_small, false);

        // main frame (black)
        graphics3D::renderer.setMaterial(tgx::RGBf(0.3f, 0.3f, 0.3f), 0.2f, 0.7f, 0.5f, 8);
        if (!wireframe) graphics3D::renderer.drawMesh(&mainframe_small, false); else graphics3D::renderer.drawWireFrameMesh(&mainframe_small, false);

        // display
        if (!displayImage) displayImage = &int_im; // use internal framebuffer
        const tgx::fVec3 Q1 = { 89.323831,-24.5161,46.692845 };
        const tgx::fVec3 Q2 = { 98.668825,-24.5161,11.181857 };
        const tgx::fVec3 Q3 = { 98.668825,24.4439,11.181857 };
        const tgx::fVec3 Q4 = { 89.323831, 24.4439, 46.692845 };
        graphics3D::renderer.setShaders(tgx::SHADER_FLAT | tgx::SHADER_TEXTURE_BILINEAR | tgx::SHADER_TEXTURE_CLAMP);
        graphics3D::renderer.setTextureWrappingMode(tgx::SHADER_TEXTURE_CLAMP);
        graphics3D::renderer.setMaterial(tgx::RGBf(1.0f, 1.0f, 1.0f), 0.2f, 0.7f, 0.5f, 8);
        if (!wireframe) graphics3D::renderer.drawQuad(Q1, Q2, Q3, Q4, nullptr, nullptr, nullptr, nullptr, &t1, &t4, &t3, &t2, &int_im); else graphics3D::renderer.drawWireFrameQuad(Q1, Q2, Q3, Q4);

        graphics3D::renderer.setShaders(tgx::SHADER_GOURAUD | tgx::SHADER_NOTEXTURE);

        // power button
        graphics3D::renderer.setMaterial(tgx::RGBf(0.3f, 0.3f, 0.3f), 0.2f, 0.7f, 0.5f, 8);
        if (!wireframe) graphics3D::renderer.drawMesh(&bouton_noir_small, false); else graphics3D::renderer.drawWireFrameMesh(&bouton_noir_small, false); // black part
        graphics3D::renderer.setMaterial(tgx::RGBf(1.0f, 0.0f, 0.0f), 0.2f, 0.7f, 0.5f, 8);
        if (!wireframe) graphics3D::renderer.drawMesh(&bouton_rouge_small, false); else graphics3D::renderer.drawWireFrameMesh(&bouton_rouge_small, false); // red part

        // usb plug
        graphics3D::renderer.setMaterial(tgx::RGBf(0.3f, 0.3f, 0.3f), 0.2f, 0.7f, 0.5f, 8);
        if (!wireframe) graphics3D::renderer.drawMesh(&usb_small, false); else graphics3D::renderer.drawWireFrameMesh(&usb_small, false);

        // camera eye
        graphics3D::renderer.setMaterial(tgx::RGBf(0.4f, 0.3f, 0.6f), 0.1f, 0.3f, 0.8f, 64);
        if (!wireframe) graphics3D::renderer.drawMesh(&cam_small, false); else graphics3D::renderer.drawWireFrameMesh(&cam_small, false);

        // ------- S.A.R.C.A.S.M. acronym ---------
        M.setIdentity();
        M.multScale({ 20.0f, 20.0f, 20.0f });
        M.multTranslate({ -42, -55.5, 72 });
        graphics3D::renderer.setModelMatrix(modelTransform * M);
        const tgx::fVec3 P1 = { -1.0f, 0.0f, 2.0f };
        const tgx::fVec3 P2 = { -1.0f, 0.0f, -2.0f };
        const tgx::fVec3 P3 = { 1.0f, 0.0f, -2.0f };
        const tgx::fVec3 P4 = { 1.0f, 0.0f, 2.0f };
        graphics3D::renderer.setMaterial(tgx::RGBf(1.0f, 1.0f, 1.0f), 0.2f, 0.7f, 0.5f, 8);
        graphics3D::renderer.setShaders(tgx::SHADER_FLAT | tgx::SHADER_TEXTURE_BILINEAR | tgx::SHADER_TEXTURE_CLAMP);
        if (!wireframe) graphics3D::renderer.drawQuad(P1, P2, P3, P4, nullptr, nullptr, nullptr, nullptr, &t1, &t2, &t3, &t4, &sarcasm_aconym_small_texture); else graphics3D::renderer.drawWireFrameQuad(P1, P2, P3, P4);

        // ------- cradle ---------
        M.setIdentity();
        M.multRotate(cradleRot, { 0,0,1 }); // rotation around Z
        M.multRotate(6, { 0,-1,0 }); // inclination
        M.multTranslate({ 52, 0, 72 }); // translation
        graphics3D::renderer.setModelMatrix(modelTransform * M);
        graphics3D::renderer.setShaders(tgx::SHADER_FLAT | tgx::SHADER_NOTEXTURE);
        graphics3D::renderer.setMaterial(tgx::RGBf(0.3f, 0.3f, 0.3f), 0.2f, 0.7f, 0.5f, 8);
        if (!wireframe) graphics3D::renderer.drawMesh(&cradle_small, false); else graphics3D::renderer.drawWireFrameMesh(&cradle_small, false); // black part
        graphics3D::renderer.setMaterial(tgx::RGBf(1.0f, 0.0f, 0.0f), 0.3f, 0.3f, 0.8f, 64);
        if (!wireframe) graphics3D::renderer.drawMesh(&reddots_small_1, false); else graphics3D::renderer.drawWireFrameMesh(&reddots_small_1, false); // red dots

        // ------- gripper ---------    
        M.setIdentity();
        M.multRotate(gripperRot, { 0,1,0 }); // inclination
        M.multTranslate({ 2, 0, 127 }); // translation to cradle position
        graphics3D::renderer.setModelMatrix(modelTransform * M);
        graphics3D::renderer.setShaders(tgx::SHADER_FLAT | tgx::SHADER_NOTEXTURE);
        graphics3D::renderer.setMaterial(tgx::RGBf(0.3f, 0.3f, 0.3f), 0.2f, 0.7f, 0.5f, 8);
        if (!wireframe) graphics3D::renderer.drawMesh(&gripper_small, false); else graphics3D::renderer.drawWireFrameMesh(&gripper_small, false); // black part
        graphics3D::renderer.setMaterial(tgx::RGBf(1.0f, 0.0f, 0.0f), 0.3f, 0.3f, 0.8f, 64);
        if (!wireframe) graphics3D::renderer.drawMesh(&gripper_red_small, false); else graphics3D::renderer.drawWireFrameMesh(&gripper_red_small, false); // red part


        // ------- tumble ---------
        M.setIdentity();
        M.multRotate(tumbleRot, { 0,1,0 }); // inclination
        M.multTranslate({ -19, 0, 72 }); // translation to cradle position
        graphics3D::renderer.setModelMatrix(modelTransform * M);
        graphics3D::renderer.setShaders(tgx::SHADER_FLAT | tgx::SHADER_NOTEXTURE);
        graphics3D::renderer.setMaterial(tgx::RGBf(0.3f, 0.3f, 0.3f), 0.2f, 0.7f, 0.5f, 8);
        if (!wireframe) graphics3D::renderer.drawMesh(&tumble_small, false); else graphics3D::renderer.drawWireFrameMesh(&tumble_small, false);

        }



    /**
     * Return the model matrix in order to positon a [-1,1]^3 cube in the cradle.
     *
     * @param   cradleRot       The current rotation of the cradle in degrees.
     * @param   modelTransform  The transform applied to the whole model (post transformation)
     *
     * @returns The model matrix (can be given to renderer.setModelMatrix() )
     */
    SLOWFUN static tgx::fMat4 cubePositionInCradle(float cradleRot, const tgx::fMat4& modelTransform)
        {
        tgx::fMat4 M;
        M.setIdentity();
        M.multRotate(cradleRot, { 0,0,1 }); // inclination
        M.multScale({ 27,27,27 }); // rubik sube is 54mm wide.
        M.multRotate(6, { 0,-1,0 }); // inclination
        M.multTranslate({ 0, 0, 24 }); // move up,
        M.multTranslate({ 52, 0, 72 }); // translation to cradle position
        return (modelTransform * M);
        }




    // these coordinate are used to place the camera for diving into the ILI9341_screen
    const tgx::fVec3 Cscreen = { 93.996328f, -0.036100f, 28.937351f }; // center of the ILI9341 screen in model coordinates
    const tgx::fVec3 Nscreen = { 0.967075f, 0, 0.254493f }; // normal vector exiting the ILI9341 screen


    /** used by lerp */
    SLOWFUN static float accel(float t, float a0, float a1)
        {
        t = std::clamp(t, 0.0f, 1.0f);
        const float t0 = (a0 == 1.0f) ? t : std::pow(t, a0);
        const float t1 = (a1 == 1.0f) ? (1 - t) : std::pow(1.0f - t, a1);
        return t0 / (t0 + t1);
        }

    /**
    * Perform a mapping from [x0,x1] -> [y0,y1]
    * with acceleration curve defined by a0 (at x0) and a1 (at x1).
    **/
    SLOWFUN static float lerp(float x, float x0, float x1, float y0 = 0.0f, float y1 = 1.0f, float a0 = 1.0f, float a1 = 1.0f)
        {
        const float t = (x - x0) / (x1 - x0); // convert to [0,1]
        if (t <= 0) return y0;
        if (t >= 1) return y1;
        return y0 + (y1 - y0) * accel(t, a0, a1);
        }








    /**************************************************************************************************
    *
    * Code specific for the solving animation
    *
    ***************************************************************************************************/


    const float GRIPPER_OPEN_DEG = 0.0f;
    const float GRIPPER_CLOSED_DEG = 90.0f;
    const float TUMBLE_PARKED_DEG = 55.0f;
    const float TUMBLE_UP_DEG = -70.0;


    /** draw sarcasm and the cube while tumbling */
    SLOWFUN void drawTumbling(const tgx::fMat4& M, float r)
        {
        const float a = 0.45f;
        const float b = 0.55f;
        float s;
        if (r < a)
            {
            s = lerp(r, 0.0f, a, 0.0f, 0.5f, 1.0f, 2.0f);
            } 
        else if (r > b)
            {
            s = lerp(r, b, 1.0f, 0.5f, 1.0f, 2.0f, 1.0f);
            } 
        else
            {
            s = 0.5f;
            }

        float cradleRot = 0;
        float gripperRot = GRIPPER_OPEN_DEG;
        float tumbleRot = (r < 0.5f) ? lerp(r, 0.0f, 0.5f, TUMBLE_PARKED_DEG, TUMBLE_UP_DEG, 1.0f, 2.0f) : lerp(r, 0.5f, 1.0f, TUMBLE_UP_DEG, TUMBLE_PARKED_DEG, 2.0f, 1.0f);
        drawSARCASM3D(cradleRot, gripperRot, tumbleRot, M);
        tgx::fMat4 MC;
        MC.setScale(1 / 3.0f, 1 / 3.0f, 1 / 3.0f);
        MC.multRotate(90, { 1,0,0 });
        MC.multRotate(90, { 0,0,1 });
        graphics3D::cube3D.draw_tumble(cubePositionInCradle(0, M) * MC, graphics3D::renderer, s);
        }


    /** draw sarcasm and the cube with the grip opening */
    SLOWFUN void drawOpeningGrip(const tgx::fMat4& M, float r)
        {
        float cradleRot = 0;
        float gripperRot = lerp(r, 0.0f, 1.0f, GRIPPER_CLOSED_DEG, GRIPPER_OPEN_DEG, 1.0f, 1.0f);
        float tumbleRot = TUMBLE_PARKED_DEG; // tumble parked
        drawSARCASM3D(cradleRot, gripperRot, tumbleRot, M);
        tgx::fMat4 MC;
        MC.setScale(1 / 3.0f, 1 / 3.0f, 1 / 3.0f);
        MC.multRotate(90, { 1,0,0 });
        MC.multRotate(90, { 0,0,1 });
        graphics3D::cube3D.drawCube(cubePositionInCradle(0, M) * MC, graphics3D::renderer);
        }


    /** draw sarcasm and the cube with the grip closing */
    SLOWFUN void drawClosingGrip(const tgx::fMat4& M, float r)
        {
        float cradleRot = 0;
        float gripperRot = lerp(r, 0.0f, 1.0f, GRIPPER_OPEN_DEG, GRIPPER_CLOSED_DEG, 1.0f, 1.0f);
        float tumbleRot = TUMBLE_PARKED_DEG; // tumble parked
        drawSARCASM3D(cradleRot, gripperRot, tumbleRot, M);
        tgx::fMat4 MC;
        MC.setScale(1 / 3.0f, 1 / 3.0f, 1 / 3.0f);
        MC.multRotate(90, { 1,0,0 });
        MC.multRotate(90, { 0,0,1 });
        graphics3D::cube3D.drawCube(cubePositionInCradle(0, M) * MC, graphics3D::renderer);
        }


    /** draw sarcasm and the cube with the grip closed */
    SLOWFUN void drawGripClosed(const tgx::fMat4& M)
        {
        drawOpeningGrip(M, 0);
        }


    /** draw sarcasm and the cube with the grip opened */
    SLOWFUN void drawGripOpened(const tgx::fMat4& M)
        {
        drawClosingGrip(M, 0);
        }


    /** draw sarcasm while rotating the whole cube */
    SLOWFUN void drawRotatingGrip_open(const tgx::fMat4& M, int dir, float r)
        {
        r = lerp(r, 0.0f, 1.0f, 0.0f, 1.0f, 2.0f, 2.0f); // acceleration and desceleration
        float cradleRot = -90 * dir * r;
        float gripperRot = GRIPPER_OPEN_DEG; // gripper fully open     
        float tumbleRot = TUMBLE_PARKED_DEG; // tumble parked
        drawSARCASM3D(cradleRot, gripperRot, tumbleRot, M);
        tgx::fMat4 MC;
        MC.setScale(1 / 3.0f, 1 / 3.0f, 1 / 3.0f);
        MC.multRotate(90, { 1,0,0 });
        MC.multRotate(90, { 0,0,1 });
        graphics3D::cube3D.draw_turn(cubePositionInCradle(0, M) * MC, graphics3D::renderer, dir, r);
        }


    /** draw SARCASM with a cube while rotating a slice */
    SLOWFUN void drawRotatingGrip_closed(const tgx::fMat4& M, int dir, float r)
        {
        r = lerp(r, 0.0f, 1.0f, 0.0f, 1.0f, 2.0f, 2.0f); // acceleration and desceleration
        float cradleRot = -90 * dir * r;
        float gripperRot = GRIPPER_CLOSED_DEG; // gripper fully open     
        float tumbleRot = TUMBLE_PARKED_DEG; // tumble parked
        drawSARCASM3D(cradleRot, gripperRot, tumbleRot, M);
        tgx::fMat4 MC;
        MC.setScale(1 / 3.0f, 1 / 3.0f, 1 / 3.0f);
        MC.multRotate(90, { 1,0,0 });
        MC.multRotate(90, { 0,0,1 });
        graphics3D::cube3D.draw_turn_bottom(cubePositionInCradle(0, M) * MC, graphics3D::renderer, dir, r);
        }



    SLOWFUN void drawOpenWithCube(const tgx::fMat4& M, float cradleRot)
        {
        const float gripperRot = GRIPPER_OPEN_DEG; // gripper fully open     
        const float tumbleRot = TUMBLE_PARKED_DEG; // tumble parked
        drawSARCASM3D(cradleRot, gripperRot, tumbleRot, M);
        tgx::fMat4 MC;
        MC.setScale(1 / 3.0f, 1 / 3.0f, 1 / 3.0f);
        MC.multRotate(90, { 1,0,0 });
        MC.multRotate(cradleRot + 90, { 0,0,1 });
        graphics3D::cube3D.drawCube(cubePositionInCradle(0, M) * MC, graphics3D::renderer);
        }

/**
 * Make the reserve sequence of a sequence of sarcasm moves.
 * (do not reset the cube3D, use the current config as the initial state)
 *
 * 'T' : tumble
 * 'R' : clockwise turn
 * 'V' : anticlowise turn
 * 'A' : double turn
 * 'r' : clockwide turn, gripper down
 * 'v' : anticlockwise turn, gripper down
 * 'a' : double turn, gripper down
 **/
SLOWFUN static int makeReverseCube3D(const char* seq)
    {
    if (!seq) return 0;
    const int cdir = 1; // direction for the clockwise turn
    int i = 0;
    while (seq[i] != 0) { i++; }; 
    const int tot = i;
    i--;
    while (i >= 0)
        {
        char m = seq[i];
        switch (m)
            {
            case 'T': { graphics3D::cube3D.tumble(); graphics3D::cube3D.tumble(); graphics3D::cube3D.tumble(); break; }
            case 'R': { graphics3D::cube3D.turn(-cdir); break; }
            case 'V': { graphics3D::cube3D.turn(cdir); break; }
            case 'A': { graphics3D::cube3D.turn(2); break; }
            case 'r': { graphics3D::cube3D.turn_bottom(-cdir); break; }
            case 'v': { graphics3D::cube3D.turn_bottom(cdir); break; }
            case 'a': { graphics3D::cube3D.turn_bottom(2); break; }
            default: break; // ignore
            }
        i--;
        }
    return tot;
    }




    const char* _seq;       // the sequence of moves
    int             _ii = 0;    // index of the current move
    int             _nbtot;     // total number of moves
    int             _phase = 0; // the phase 
    elapsedMillis   _em = 0;    // elapsed time in the current phase
    elapsedMillis   _ema = 0;    // elapsed time in the current phase
    elapsedMillis   _emb = 0;    // elapsed time in the current phase


SLOWFUN int totalMoves()
    {
    return _nbtot;
    }

SLOWFUN int remainingMoves()
    {
    return _nbtot - _ii;
    }

SLOWFUN void InitSolvingAnimation(const tgx::Image<tgx::RGB565>& dst_im, const char* seq)
    {
    setUpRenderer(dst_im);
    _nbtot = makeReverseCube3D(seq); 
    _seq = seq;
    _ii = 0;
    _phase = -1;
    _em = 0;
    _ema = 0;
    _emb = 0;
    tgx::fMat4 M;
    M.setIdentity();
    graphics3D::cube3D.drawCubePartial(M, -2, graphics3D::renderer, true); // reshuffle. 
    }


SLOWFUN  bool SarcasmSolvingAnimationSub(const tgx::fMat4& M)
    {
    const float TUMBLING_MS = 600.0f;
    const float ROT_MS = 500.0f;
    const float ROT2_MS = 800.0f;
    const float GRIP_CLOSE_MS = 180.0f;
    const float GRIP_OPEN_MS = 180.0f;
    const float PAUSE_AFTER_TUMBLE_MS = 450.0f;

    static int adir = 2;
    const int cdir = 1;
    const char mm = _seq[_ii]; // current move to execute
    const float t = (float)_em;

    //Serial.print("ii="); Serial.print(_ii); Serial.print(" move="); Serial.print(mm); Serial.print(" phase="); Serial.print(_phase); Serial.print(" t="); Serial.println(t);
    if (mm == 0)
        { // we are done
        drawGripOpened(M);
        _ema = 0;
        return true;
        }

    switch (mm)
        {
        case 0:
        {
        drawGripOpened(M);
        return true;
        }
        case 'T':
        {
        if (_phase == 0)
            {
            float r = lerp(t, 0, TUMBLING_MS, 0.0f, 1.0f, 1.0f, 1.0f);
            drawTumbling(M, r);
            if (t >= TUMBLING_MS)
                {
                graphics3D::cube3D.tumble();
                _em = 0; _phase = 1;
                }
            return false;
            } else
            {
            drawGripOpened(M);
            if (t >= PAUSE_AFTER_TUMBLE_MS)
                {
                _ii++; _em = 0; _phase = 0;
                }
            return false;
            }
        }
        case 'R':
        {
        float r = lerp(t, 0, ROT_MS, 0.0f, 1.0f, 2.0f, 2.0f);
        drawRotatingGrip_open(M, cdir, r);
        if (t >= ROT_MS)
            {
            graphics3D::cube3D.turn(cdir);
            _ii++; _em = 0; _phase = 0;
            }
        return false;
        }
        case 'V':
        {
        float r = lerp(t, 0, ROT_MS, 0.0f, 1.0f, 2.0f, 2.0f);
        drawRotatingGrip_open(M, -cdir, r);
        if (t >= ROT_MS)
            {
            graphics3D::cube3D.turn(-cdir);
            _ii++; _em = 0; _phase = 0;
            }
        return false;
        }
        case 'A':
        {
        float r = lerp(t, 0, ROT2_MS, 0.0f, 1.0f, 2.0f, 2.0f);
        drawRotatingGrip_open(M, adir, r);
        if (t >= ROT2_MS)
            {
            graphics3D::cube3D.turn(adir);
            _ii++; _em = 0; _phase = 0; adir = -adir;
            }
        return false;
        }
        }
    // moves with gripper down
    if (_phase == 0)
        { // close the grip
        float r = lerp(t, 0, GRIP_CLOSE_MS, 0.0f, 1.0f, 2.0f, 2.0f);
        drawClosingGrip(M, r);
        if (t >= GRIP_CLOSE_MS) { _em = 0; _phase = 1; }
        return false;
        }
    if (_phase == 2)
        {
        float r = lerp(t, 0, GRIP_OPEN_MS, 0.0f, 1.0f, 2.0f, 2.0f);
        drawOpeningGrip(M, r);
        if (t >= GRIP_OPEN_MS) { _ii++; _em = 0; _phase = 0; }
        return false;
        }
    // we are in phase 1
    switch (mm)
        {
        case 'r':
        {
        float r = lerp(t, 0, ROT_MS, 0.0f, 1.0f, 2.0f, 2.0f);
        drawRotatingGrip_closed(M, cdir, r);
        if (t >= ROT_MS)
            {
            graphics3D::cube3D.turn_bottom(cdir);
            _em = 0; _phase = 2;
            }
        return false;
        }
        case 'v':
        {
        float r = lerp(t, 0, ROT_MS, 0.0f, 1.0f, 2.0f, 2.0f);
        drawRotatingGrip_closed(M, -cdir, r);
        if (t >= ROT_MS)
            {
            graphics3D::cube3D.turn_bottom(-cdir);
            _em = 0; _phase = 2;
            }
        return false;
        }
        case 'a':
        {
        float r = lerp(t, 0, ROT2_MS, 0.0f, 1.0f, 2.0f, 2.0f);
        drawRotatingGrip_closed(M, adir, r);
        if (t >= ROT2_MS)
            {
            graphics3D::cube3D.turn_bottom(adir);
            _em = 0; _phase = 2; adir = -adir;
            }
        return false;
        }
        default:
        { // skip unknown moves
        _ii++; _em = 0; _phase = 0;
        }
        }
    return false;
    }





SLOWFUN  int SolvingAnimation()
    {
    const float ms_before_stop_close = 3000.0f;
    const float ms_before_wirecube_appear = 4000.0f;
    const float ms_before_cubelets_appear = 4500.0f;
    const float ms_before_cubelets_end = 7000.0f;
    const float ms_before_start_solve = 8000.0f; // time before starting the solving animation
    const float ms_before_dive = 2000.0f; // time before starting the dive into the screen
    const float ms_dive = 5000.0f; // duration of the dive into the screen
    const float ms_end = 3000.0f; // start signaling for end of animation. 
    const float ms_before_music_stop = 500.0f; // time before stopping the music before the end of the animation

    const float camera_height = 120.0f;  // initial height of the camera
    const float camera_look_height = 92.0f; // initial height of the point the camera is looking at
    float camera_distance = 140.0f; // initial distance of the camera from the screen (along the screen normal)
    const float camera_distance_far = 4000.0f; // initial distance of the camera from the screen (along the screen normal)


    float modelRotStart = sinf(_emb / 2000.0f) * 45; // initial rotation of the model
    if ((_phase == -1) && (_em < ms_before_stop_close))
        {
        modelRotStart += 360 * lerp(_em, 0, ms_before_stop_close, 0, 1, 1.0f, 2.0f);
        }


    camera_distance = 117 - 50 * cosf((((int)_emb - ms_before_stop_close) / 3300.0f));


    // set the camera position    
    switch (_phase)
        {
        case -1: // from far away
        {
        const float t = (float)_em;
        const float l = lerp(t, 0, ms_before_stop_close, camera_distance_far, camera_distance, 1.0f, 3.0f);
        const float he = camera_height;
        const float hd = camera_look_height;
        graphics3D::renderer.setLookAt(Cscreen + (Nscreen * l) + tgx::fVec3(0, 0, he), Cscreen + tgx::fVec3(0, 0, hd), { 0,0,1 });


        graphics3D::renderer.setLightDirection(-Cscreen - (Nscreen * l) - tgx::fVec3(0, 0, he));
        break;
        }
        case -3: // diving into the screen
        {
        const float t = (float)_em;
        const float l = camera_distance - lerp(t, 0, ms_dive, 0, 25.0f, 2.0f, 2.0f);  //lerp(t, 0, ms_dive, camera_distance, 15.0f, 2.0f, 1.0f);
        const float he = camera_height; // lerp(t, 0, ms_dive, camera_height, 0.0f, 2.0f, 2.0f);
        const float hd = camera_look_height; // lerp(t, 0, ms_dive, camera_look_height, 0.0f, 2.0f, 2.0f);
        graphics3D::renderer.setLookAt(Cscreen + (Nscreen * l) + tgx::fVec3(0, 0, he), Cscreen + tgx::fVec3(0, 0, hd), { 0,0,1 });
        break;
        }
        default:
        {
        const float l = camera_distance;
        const float he = camera_height;
        const float hd = camera_look_height;
        graphics3D::renderer.setLookAt(Cscreen + (Nscreen * l) + tgx::fVec3(0, 0, he), Cscreen + tgx::fVec3(0, 0, hd), { 0,0,1 });
        }
        }

    tgx::fMat4 M;

    if (_phase >= 0)
        { // in the solving phase
        M.setRotate(modelRotStart, { 0,0,1 });
        if (SarcasmSolvingAnimationSub(M))
            { // finished solving 
            _em = 0; _phase = -2;
            }
        return 0;
        }


    if (_phase == -1)
        { // not started resolution yet.   
        M.setRotate(modelRotStart, { 0,0,1 });

        float cradleRot = 0;
        if (_ema < ms_before_stop_close)
            {
            cradleRot = -90 * lerp(_ema, 0, ms_before_stop_close, 70, 0, 1.0f, 3.5f);
            } else if (_ema < ms_before_wirecube_appear)
                {
                cradleRot = 0;
                } else if (_ema < ms_before_cubelets_end)
                    {
                    cradleRot = lerp(_ema, ms_before_wirecube_appear, ms_before_cubelets_end, 0, 360, 1.5f, 1.5f);
                    }

                drawSARCASM3D(cradleRot, GRIPPER_OPEN_DEG, TUMBLE_PARKED_DEG, M);
                tgx::fMat4 MC;
                MC.setScale(1 / 3.0f, 1 / 3.0f, 1 / 3.0f);
                MC.multRotate(90, { 1,0,0 });
                MC.multRotate(cradleRot + 90, { 0,0,1 });
                if (_ema > ms_before_stop_close)
                    {
                    float rr = lerp(_ema, ms_before_cubelets_appear, ms_before_cubelets_end, 0.0f, 1.0f, 1.0f, 1.0f);
                    graphics3D::cube3D.drawCubePartial(cubePositionInCradle(0, M) * MC, rr, graphics3D::renderer, true);
                    }

                if (_em >= ms_before_start_solve)
                    { // begin resolution
                    _phase = 0; _em = 0;
                    }
                return 0;
        }

    float cradleRot = lerp(_ema, 0, ms_before_dive + ms_dive, 0, 10 * 90, 2.0, 2.0);

    if (_phase == -2)
        { // finshed solving, not yet diving
        M.setRotate(modelRotStart, { 0,0,1 });
        drawOpenWithCube(M, cradleRot);

        if (_em >= ms_before_dive)
            { // begin dive
            _phase = -3; _em = 0;
            }        
        return 0;
        }
    if (_phase == -3)
        { // diving into the screen
        const float t = (float)_em;
        const float modelRot = lerp(t, 0, ms_dive - 1000, modelRotStart, 0, 2.0f, 1.0f);
        M.setRotate(modelRot, { 0,0,1 });
        drawOpenWithCube(M, cradleRot);

        if (t >= ms_end)
            { // all done
            return 2;
            }
        if (t > ms_before_music_stop)
            {
            return 1;
            }

        return 0;
        }

    return 2;
    }








    /**************************************************************************************************
    *
    * Code specific for the dreaming animation
    *
    ***************************************************************************************************/

        static const float startModelRot = 4000;  // start rotating the model 
        static const float endModelRot = 22000;   // stop rotation of the model
        static const float totModelRotAngle = 350.0f; // total rotation angle of the model during the animation (in degrees)

        static const float startCradleRot = 0;  // start rotation of the cradle
        static const float endCradleRot = 26000; // end rotation of the cradle
        static const float totCradleRotAngle = 5 * 360.0f; // total rotation angle of the cradle during the animation (in degrees)

        static const float startDive = 20000; // start 'diving' into the screen for the camera
        static const float startLightUp = 20000; 
        static const float endDive = 26000;   // finish diving into the screen (end of the animation)

        static const float camera_height = 120.0f;  // initial height of the camera
        static const float camera_look_height = 80.0f; // initial height of the point the camera is looking at
        static const float camera_distance = 200.0f; // initial distance of the camera from the screen (along the screen normal)


    SLOWFUN float dreamingRatioDone()
        {
        return ((float)(_em))/ endDive;
        }



    SLOWFUN  void InitDreamingAnimation(const tgx::Image<tgx::RGB565>& dst_im)
        {
        setUpRenderer(dst_im);
        _em = 0;
        }


    SLOWFUN int DreamingAnimation()
        {  

        const float t = (uint32_t)_em;
        //set lightning same as audioled
        tgx::RGBf tint = MAX98357A::audioLed.getCurrentTint();
        tint.R *= 1.2f;
        tint.G *= 1.2f;
        tint.B *= 1.2f;
        graphics3D::renderer.setLightDiffuse(tint);


    
        // Camera movement
        { 
        const float l = lerp(t, startDive, endDive, camera_distance, 15.0f, 2.0f, 1.0f);
        const float he = lerp(t, startDive, endDive - 0, camera_height, 0.0f, 2.0f, 2.0f);
        const float hd = lerp(t, startDive, endDive - 0, camera_look_height, 0.0f, 2.0f, 2.0f);
        graphics3D::renderer.setLookAt(Cscreen + (Nscreen * l) + tgx::fVec3(0, 0, he), Cscreen + tgx::fVec3(0, 0, hd), { 0,0,1 });
        }

    
        // model rotation
        tgx::fMat4 M;
        {
        const float modelRot = lerp(t, startModelRot, endModelRot, totModelRotAngle, 0.0f, 2.0f, 2.0f);
        M.setRotate(modelRot, { 0,0,1 }); // rotation around Y
        }


        // cradle rotation
        float cradleRot;
        {
        cradleRot = lerp(t, startCradleRot, endCradleRot, 0.0f, totCradleRotAngle, 1.0f, 1.0f);
        }


        float gripperRot = 0; // gripper fully open
        float tumbleRot = 55; // tumble parked

        drawSARCASM3D(cradleRot, gripperRot, tumbleRot, M);

        // draw the 3 companions cube on the side
        {
        graphics3D::renderer.setShaders(tgx::SHADER_GOURAUD | tgx::SHADER_TEXTURE_BILINEAR | tgx::SHADER_TEXTURE_CLAMP);
        graphics3D::renderer.setMaterial(tgx::RGBf(1.0f, 1.0f, 0.0f), 0.2f, 0.7f, 0.5f, 8);
        tgx::fMat4 S;
        
        S.setRotate(0, { 0,0,1 }); // inclination
        S.multScale({ 27,27,27 }); // rubik sube is 54mm wide.
        S.multTranslate({ 0, 160, 0 }); // move up,   
        graphics3D::renderer.setModelMatrix(M * S);
        graphics3D::renderer.drawMesh(&companion_cube_1, false);

        S.setRotate(0, { 0,0,1 }); // inclination
        S.multScale({ 27,27,27 }); // rubik sube is 54mm wide.
        S.multTranslate({ -30, 150, 54 }); // move up,   
        graphics3D::renderer.setModelMatrix(M * S);
        graphics3D::renderer.drawMesh(&companion_cube_1, false);

        S.setRotate(0, { 0,0,1 }); // inclination
        S.multScale({ 27,27,27 }); // rubik sube is 54mm wide.
        S.multTranslate({ -65, 140, 0 }); // move up,   
        graphics3D::renderer.setModelMatrix(M * S);
        graphics3D::renderer.drawMesh(&companion_cube_1, false);
        }

        // draw the companion cube inside the cradle
        {
        graphics3D::renderer.setModelMatrix(cubePositionInCradle(cradleRot, M));
        graphics3D::renderer.drawMesh(&companion_cube_1, false);
        }

        if (t >= endDive)
            { // all done
            return 2;
            }
        if (t > startLightUp)
            {
            return 1; // decease opacity
            }
        return 0;
        }





    }

/** end of file */
