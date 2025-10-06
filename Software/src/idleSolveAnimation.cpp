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

#include <Arduino.h>
#include <TeensyThreads.h>
#include <tgx.h>

#include "idleSolveAnimation.h"
#include "config.h"
#include "debug.h"
#include "FBSensors.h"
#include "faceAction.h"
#include "graphics3D.h"
#include "display.h"
#include "Console.h"
#include "UI.h"
#include "xorshift32.h"
#include "audioMAX98357A.h"
#include "audioLed.h"
#include "craddlemove.h"
#include "RubikCube3D.h"
#include "transition_tags.h"
#include "panic.h"
#include "songs.h"






static const char FILENAME_SOUND_START_IDLE_SOLVE[] PROGMEM = "act_sounds/tag_start_triolet.wav";
static const char FILENAME_SOUND_END_IDLE_SOLVE[] PROGMEM = "act_sounds/endsolve_idle.wav";
static const char FILENAME_SOUND_ABORT_IDLE_SOLVE[] PROGMEM = "act_sounds/tag_end_abort.wav";

#define MIN_SHUFFLE_MOVES 13
#define MAX_SHUFFLE_MOVES 15


/** A move, stored in a single byte. */
struct SMove
    {

    void set(int axe, int slice, int dir, int half)
        {
        _data = (uint8_t)((axe & 3) | (((slice + 1) & 3) << 2) | ((dir & 1) << 4) | ((half & 1) << 5));
        }

    void get(int& axe, int& slice, int& dir, int& half)
        {
        axe = _data & 3;
        slice = ((_data >> 2) & 3) - 1;
        dir = (_data >> 4) & 1;
        half = (_data >> 5) & 1;
        }

    uint8_t _data;
    };


/**
* Start from an intially solved cube.
* Scramble the cube with a 'len' random moves
* then save the reverse moves in list
**/
SLOWFUN static void makeScrambleList(SMove* list, int len)
    {
    graphics3D::cube3D.resetPosition();
    int axe = -1, slice = -1, dir = -1, half = -1; // current piece being turned, dir being the direction of the turn
    int oldaxe = -2;
    for (int i = 0; i < len; i++)
        {
        int naxe, nslice;
        do { // make sure we do not repeat the same move twice in a row
            naxe = rng::rand(3); // new random axe
            nslice = rng::rand(3) - 1; // new random slice
            } while (((naxe == axe) && (nslice == slice)) || ((axe == naxe) && (axe == oldaxe)));
        oldaxe = axe;
        axe = naxe;
        slice = nslice;
        dir = rng::rand(2);
        half = (rng::rand(3) <= 1) ? 0 : 1;
        float deg = ((dir == 0) ? 90.0f : -90.0f) * (half + 1);
        graphics3D::cube3D.rotateSlice(deg, axe, slice); // rotate the slice
        list[len - 1 - i].set(axe, slice, dir, half); // save the reverse move
        }
    return;
    }


/** upate the display, called only from idleSolveAnimation()*/
SLOWFUN static void update_display_cube(tgx::fMat4& M, uint32_t em, float r, bool draw_cube, bool drawwheel, float u = -1.0f)
    {
    const tgx::RGB565 COLOR_WHEEL = tgx::RGB565(18, 0, 0);
    tgx::Image<tgx::RGB565> im_alt = Display::image_alt;
    tgx::Image<tgx::RGB565> im_main = Display::image_main;

    im_alt.clear(tgx::RGB565_Black);

    // display the cube    
    if (draw_cube)
        {
        graphics3D::renderer.clearZbuffer();
        if (u >= 0)
            {
            graphics3D::cube3D.drawCubePartial(M, u, graphics3D::renderer); // draw the cube at its current position        
            } 
        else
            {
            graphics3D::cube3D.drawCube(M, graphics3D::renderer); // draw the cube at its current position        
            }
        }
    UI::drawMainUI(im_main); // copy im_alt to im_main with the deformation. 
    UI::drawUIvoltage(im_main, FBSensors::getVoltage(), FBSensors::getChargingCurrent()); // draw the voltage/charge status    

    if (r <= 0)
        {
        UI::drawUIactivity(im_main, { 0, -1 });
        }
    else
        {
        UI::drawUIactivity(im_main, { 0, (int)(8*r)});
        }

    // draw the console text
    Console::console.draw(im_main);

    // draw the spinning wheel
    if (drawwheel)
        {
        const tgx::fVec2 cc = Console::getCharBox({ 24, 0 }).center() - tgx::iVec2(5, 0);
        const float t = fmodf((em / 800.0f), 2.0f);
        const float R = 8.0f;
        if (t < 1.0f)
            {
            const float thick = R * t;
            im_main.drawThickCircleArcAA(cc, R, 0, 360 * t, thick, COLOR_WHEEL, 1.0f); // draw a blue circle arc
            } else
            {
            const float thick = R * (2.0f - t);
            im_main.drawThickCircleArcAA(cc, R, (t - 1.0f) * 360, 360, thick, COLOR_WHEEL, 1.0f); // draw a blue circle arc
            }
        }

    // display
    Display::update(im_main);
    threads.yield();
    }




SLOWFUN static bool getReturnCode(int& rcode, uint32_t lt)
    {
    if (CraddleMove::status() == 2) { rcode = CODE_SPINNED; return true; }
    if (Display::touchedTime() > lt + 200) { rcode = CODE_TOUCHED_SCREEN; return true; }
    const int cs = FBSensors::chargeStatus();
    if (cs == CODE_CHARGING_START) { rcode = CODE_CHARGING_START; return true; }
    if (cs == CODE_CHARGING_STOP) { rcode = CODE_CHARGING_STOP; return true; }
    return false; 
    }

#define LED_TINT_PERIOD_IDLESOLVE 8000

SLOWFUN int idleSolveAnimation()
    {
    
    const int INTRO_CUBE_MS = 5000; // lenght of intro animation in ms

    uint32_t lt = Display::touchedTime();
    int rcode; // return code
    // wait until the face is not visible anymore
    elapsedMillis em = 0; // timer for the animation
    while(faceAction::currentFaceVisible())
        {
        if (getReturnCode(rcode, lt)) { return rcode; }
        if (em > 10000)
            {
            ABORT("INFINITE LOCK: SHOULD NOT HAPPEN");
            }
        graphics3D::updateAll(0);
        threads.yield();
        }

    em = 0;
    tgx::fMat4 M;

    MAX98357A::setAudioLevelMusic(0.5f * START_SOLVE_SOUND_LEVEL);
    MAX98357A::audioLed.setDriver(MAX98357A::AudioLed::DRIVER_MUSIC);
    MAX98357A::playMusic(FILENAME_SOUND_START_IDLE_SOLVE);
    MAX98357A::audioLed.setVoiceLedTintAuto(LED_TINT_PERIOD_IDLESOLVE);

    // now print the message
    Console::console.clear();
    Console::console.setCurrentColor(TFT_CONSOLE_MESSAGE_COLOR);
    int i = 0;
    const int tag_index = transition_tags::picker_tag_idle_solve.next();
    const char* msg = transition_tags::tag_idle_solve[tag_index];

    while (msg[i])
        {
        if (em > 60) { Console::console.print(msg[i]); em = 0; i++; };
        if (getReturnCode(rcode, lt)) 
            { 
            MAX98357A::audioLed.setDriver(MAX98357A::AudioLed::DRIVER_MUSIC);
            MAX98357A::playMusic(FILENAME_SOUND_ABORT_IDLE_SOLVE);
            MAX98357A::audioLed.setVoiceLedTint();
            Console::console.clear();
            Console::console.print(msg);
            Console::console.setCurrentColor(TFT_CONSOLE_MESSAGE_COLOR_ABORT);
            Console::console.setText({ 21, 0 }, "STOP");
            Console::console.setCurrentColor();
            Console::console.setCursor( 0, 1);
            Console::console.showCursor(true); // show the cursor again   
            Console::console.setCurrentColor(TFT_CONSOLE_COLOR);
            return rcode;
            }
        update_display_cube(M, em, 0.0f, false, false);
        }
    Console::console.showCursor(false);

    MAX98357A::audioLed.setDriver(MAX98357A::AudioLed::DRIVER_VOICE); // set the audio LED driver to voice mode

    MAX98357A::setAudioLevelMusic(ANIMATION_IDLE_SOLVE_SOUND_LEVEL);
    MAX98357A::playMusic(songs::picker_song_idlesolve.next_str());

    em = 0;

    graphics3D::resetColorStickers(true);

    tgx::Image<tgx::RGB565> im_alt = Display::image_alt;

    faceAction::audioReacEnable(false);
    faceAction::cubeAppear(im_alt, (int)(INTRO_CUBE_MS*0.25f));

    graphics3D::renderer.setLight(GRAPHICS3D_LIGHT_DIRECTION, GRAPHICS3D_LIGHT_AMBIANT, GRAPHICS3D_LIGHT_DIFFUSE, GRAPHICS3D_LIGHT_SPECULAR);
    graphics3D::renderer.setViewportSize(im_alt.lx(), im_alt.ly());
    graphics3D::renderer.setOffset(0, 0); //  image = viewport
    graphics3D::renderer.setImage(&im_alt); // set the image to draw onto (ie the screen framebuffer)
    graphics3D::renderer.setPerspective(45, ((float)im_alt.lx()) / im_alt.ly(), 1.0f, 1000.0f);  // set the perspective projection matrix.     
    graphics3D::renderer.setLookAt({ 0, 25, 36 }, { 0,2.5f,0 }, { 0,1,0 }); // set the camera position and orientation

    const int nb_moves = rng::rand(MIN_SHUFFLE_MOVES, MAX_SHUFFLE_MOVES);
    SMove moves[MAX_SHUFFLE_MOVES];
    makeScrambleList(moves, nb_moves);
    int move_index = 0;

    M.setIdentity();
    M.multScale(2.5f, 2.5f, 2.5f);
    M.multRotate(rng::unif()*360.0f, {0,1,0});

    float cube_rot_speed = 0.8f; // rotation speed for the whole cube, in degree per frame
    const int frames_per_turn = 25; // rotation speed (number of frame per 90 degreee turn of a slice)
    const float delta_angle = 90.0f / frames_per_turn; // degrees per frame

    graphics3D::cube3D.drawCubePartial(M, -1, graphics3D::renderer); // reshuffle order of apparition of the cubelets
    while (em < INTRO_CUBE_MS)
        { 
        if (getReturnCode(rcode, lt))
            {            
            MAX98357A::audioLed.setDriver(MAX98357A::AudioLed::DRIVER_MUSIC);
            MAX98357A::playMusic(FILENAME_SOUND_ABORT_IDLE_SOLVE);
            MAX98357A::setAudioLevelMusic(0.5f * START_SOLVE_SOUND_LEVEL);
            MAX98357A::audioLed.setVoiceLedTint();
            faceAction::audioReacEnable(true);
            faceAction::cubeDisappear(500);
            Console::console.setCurrentColor(TFT_CONSOLE_MESSAGE_COLOR_ABORT);
            Console::console.setText({ 21, 0 }, "STOP");
            Console::console.setCurrentColor();
            Console::console.setCursor(0, 1);
            Console::console.showCursor(true); // show the cursor again   
            Console::console.setCurrentColor(TFT_CONSOLE_COLOR);
            return rcode;
            }
        M.multRotate(cube_rot_speed, { 0,1,0 });
        const float r = em / (INTRO_CUBE_MS * 0.75f);
        update_display_cube(M, em, 0.0f, true, true, r);
        }

    int axe = 0, slice = 0, dir = 1; // current piece being turned, dir being the direction of the turn    
    int frame = frames_per_turn; // current frame of the turn
    int frame_tot = frames_per_turn;

    while (1)
        {
        if (getReturnCode(rcode, lt))
            {
            MAX98357A::audioLed.setDriver(MAX98357A::AudioLed::DRIVER_MUSIC);
            MAX98357A::playMusic(FILENAME_SOUND_ABORT_IDLE_SOLVE);
            MAX98357A::setAudioLevelMusic(0.5f * START_SOLVE_SOUND_LEVEL);
            MAX98357A::audioLed.setVoiceLedTint();
            faceAction::audioReacEnable(true);
            faceAction::cubeDisappear(500);
            Console::console.setCurrentColor(TFT_CONSOLE_MESSAGE_COLOR_ABORT);
            Console::console.setText({ 21, 0 }, "STOP");
            Console::console.setCurrentColor();
            Console::console.setCursor(0, 1);
            Console::console.showCursor(true); // show the cursor again   
            Console::console.setCurrentColor(TFT_CONSOLE_COLOR);
            return rcode;
            }

        if (frame == frame_tot)
            { // load next move
            if (move_index == nb_moves) { break; }
            int h;
            moves[move_index].get(axe, slice, dir, h);
            dir = 2 * dir - 1;
            frame_tot = frames_per_turn * (h + 1);
            frame = 0;
            move_index++;
            //Serial.printf("Next move: axe=%d slice=%d dir=%d half=%d\n", axe, slice, dir, h);
            }        
        M.multRotate(cube_rot_speed, { 0,1,0 });  // turn slightly    
        update_display_cube(M,em, move_index /((float)nb_moves), true, true);
        frame++;
        graphics3D::cube3D.rotateSlice(delta_angle * dir, axe, slice); // rotate the slice
        }

    const int CUBE_FACEACTION_SOLVED_MS = 1500; // time for the cube got to 'state_visible_cube_solved' in faceAction
    const int CUBE_SHOW_SOLVED_MS = 3000; // time to show the solved cube
    const int CUBE_DISAPPEAR_MS = 1500; // time to show the solved cube
    faceAction::cubeAppearSolved(im_alt, CUBE_FACEACTION_SOLVED_MS);

    const char* endmsg = transition_tags::tag_idle_solve_end[tag_index];
    Console::console.setCursor(25 - strlen(endmsg), 0);
    Console::console.setCurrentColor(TFT_CONSOLE_MESSAGE_COLOR_DONE);
    Console::console.print(endmsg);

    MAX98357A::setAudioLevelMusic(0.5f * START_SOLVE_SOUND_LEVEL);
    MAX98357A::audioLed.setDriver(MAX98357A::AudioLed::DRIVER_MUSIC); // set the audio LED driver to voice mode
    MAX98357A::playMusic(FILENAME_SOUND_END_IDLE_SOLVE, false);
    MAX98357A::audioLed.setVoiceLedTint();

    uint32_t time_end = em;
    while (em < CUBE_SHOW_SOLVED_MS + time_end)
        { // turn slightly    
        if (getReturnCode(rcode, lt))
            {
            MAX98357A::audioLed.setDriver(MAX98357A::AudioLed::DRIVER_MUSIC);
            MAX98357A::playMusic(FILENAME_SOUND_ABORT_IDLE_SOLVE);
            faceAction::audioReacEnable(true);
            faceAction::cubeDisappear(500);
            Console::console.setCurrentColor();
            Console::console.setCursor(0, 1);
            Console::console.showCursor(true); // show the cursor again   
            Console::console.setCurrentColor(TFT_CONSOLE_COLOR);
            return rcode;
            }
        M.multRotate(cube_rot_speed, { 0,1,0 });
        update_display_cube(M,em, 1.0f,true,false);
        }

    MAX98357A::audioLed.setDriver(MAX98357A::AudioLed::DRIVER_VOICE); // set the audio LED driver to voice mode
    faceAction::cubeDisappear(CUBE_DISAPPEAR_MS);
    time_end = em;
    while (em < 2000 + time_end)
        { // turn slightly    
        if (getReturnCode(rcode, lt))
            {
            MAX98357A::audioLed.setDriver(MAX98357A::AudioLed::DRIVER_MUSIC);
            MAX98357A::playMusic(FILENAME_SOUND_ABORT_IDLE_SOLVE);
            faceAction::audioReacEnable(true);
            faceAction::cubeDisappear(500);
            Console::console.setCurrentColor();
            Console::console.setCursor(0, 1);
            Console::console.showCursor(true); // show the cursor again   
            Console::console.setCurrentColor(TFT_CONSOLE_COLOR);
            return rcode;
            }
        M.multRotate(cube_rot_speed, { 0,1,0 });
        update_display_cube(M, em, 1.0f, true, false);
        }
    faceAction::audioReacEnable(true);
    Console::console.setCursor(0, 1);
    Console::console.showCursor(true);
    Console::console.setCurrentColor(TFT_CONSOLE_COLOR);

    return 0;  // animation completed without interruption
    }



/** end of file */
