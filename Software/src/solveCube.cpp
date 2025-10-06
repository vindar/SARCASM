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
#include <kociemba.h>
#include <TeensyThreads.h>
#include <tgx.h>

#include "Config.h"
#include "Hardware.h"
#include "debug.h"
#include "FBSensors.h"
#include "Memory.h"
#include "PowerCtrl.h"
#include "DRV8834.h"
#include "LedStrip.h"
#include "Servos.h"
#include "panic.h"
#include "display.h"
#include "audioMAX98357A.h"
#include "AudioLed.h"

#include "craddlemove.h"
#include "BasicMoves.h"
#include "CubeRead.h"
#include "CubeOrientation.h"
#include "CubeState.h"
#include "faceAction.h"

#include "Console.h"
#include "RubikCube3D.h"
#include "UI.h"
#include "graphics3D.h"
#include "teensythread.h"
#include "sentences.h"

#include "songs.h"
#include "misc.h"






/**
* Return the number of moves in the solution string.
**/
SLOWFUN static int solLen(const char* sol)
    {
    int nbm = 0;
    int b = 0;
    while (sol[0] != 0)
        {
        if ((sol[0] != ' ') && (b == 0)) { b = 1; nbm++; }
        if ((sol[0] == ' ') && (b == 1)) { b = 0; }
        sol++;
        }
    return nbm;
    }



// display the progress bar and remaining time before shutdown    
SLOWFUN void _updateWithPB(uint32_t em_poff, uint32_t poff_ms)
    {
    tgx::Image<tgx::RGB565> im_alt = Display::image_alt; // get the main image
    tgx::Image<tgx::RGB565> im_main = Display::image_main; // get the main image
    const int cy = TFT_CONSOLE_CHAR_LY - 3;
    UI::drawMainUI(im_alt);
    Console::drawFromBlank(im_alt, im_main); // erase everything and redraw the background, console, voltage and activity        
    // draw the progress bar on im. 
    float r = em_poff / ((float)poff_ms);
    if (r < 0.0f) r = 0.0f; else if (r > 1.0f) r = 1.0f;
    int y = (Console::getCharBox({ 0, cy + 1 }).center().y + Console::getCharBox({ 0, cy + 2 }).center().y) / 2;
    const int xmin = 15;
    const int xmax = 304;
    const int xmax2 = xmin + (int)((float)(xmax - xmin) * r);
    im_main.fillThickRoundRectAA(tgx::iBox2(xmin, xmax2, y - 18, y + 19), 5, 2, TFT_CONSOLE_MESSAGE_COLOR_ABORT, TFT_CONSOLE_MESSAGE_COLOR_ABORT, 0.5f);
    im_main.drawThickRoundRectAA(tgx::iBox2(xmin, xmax, y - 18, y + 19), 5, 2, TFT_CONSOLE_MESSAGE_COLOR, 1.0f);
    Display::update(im_main); // update
    }



const char FILENAME_SOUND_ABORT_SHUTDOWN[] PROGMEM = "act_sounds/panic_beep.wav";


SLOWFUN void panicLights(uint32_t em)
    {
    uint32_t intensity = em % 500;
    if (intensity > 250) intensity = 500 - intensity;
    LedStrip::setAllLed(255, 0, 0, intensity / 2);
    LedStrip::show();
    }


SLOWFUN void panicDuringSolve()
    {

    MAX98357A::stopMusic();
    MAX98357A::stopSpeak();

    MAX98357A::audioLed.setDriver(MAX98357A::AudioLed::DRIVER_NONE); // disable the led driver

    faceAction::audioReacEnable(false);
    faceAction::appear(1000);

    Console::console.clear();
    MAX98357A::setAudioLevelMusic(PANIC_BEEP_SOUND_LEVEL);
    MAX98357A::playMusic(FILENAME_SOUND_ABORT_SHUTDOWN);

    LedStrip::setAllLed(255, 0, 0, 0);
    LedStrip::show();

    elapsedMillis em_poff = 0;
    const uint32_t poff_ms = 20000; // 20 seconds to poweroff

    while (MAX98357A::isPlayingMusic())
        {
        panicLights(em_poff);
        graphics3D::updateAll(0);
        threads.yield();
        }

    Console::console.clear();
    const int cy = TFT_CONSOLE_CHAR_LY - 3;
    Console::console.setCursor(0, cy);
    Console::console.showCursor(true); // hide the cursor
    Console::console.setCurrentColor(TFT_CONSOLE_MESSAGE_COLOR);
    Console::console.print("Shutdown in progress...");
    Console::console.setCursor(0, 0);
    Console::console.setCurrentColor(TFT_CONSOLE_MESSAGE_COLOR_ABORT);
    MAX98357A::speak("PANIC MODE TRIGGERED.\n\n");

    while (MAX98357A::isSpeaking())
        {
        panicLights(em_poff);
        _updateWithPB(em_poff, poff_ms);
        threads.yield();
        }
    Console::console.setCurrentColor(TFT_CONSOLE_COLOR);
    switch (getPanicFlag())
        {
        case 1: 
            {// craddle position error
            MAX98357A::speakformatted("Craddle position error.\n<break time='500ms'/>  Offset: [] degrees.", (int)DRV8834::cradleError());
            break;
            }
        case 2: 
            {// tumble error
            int fb, pos;
            Servos::Tumble::getError(fb, pos);
            if (pos == -1) { MAX98357A::speakformatted("Tumble servo error 1.\n\n<break time='500ms'/>  Value: [] too high.", fb); }
            else if (pos == -2) { MAX98357A::speakformatted("Tumble servo error 2.\n\n<break time='500ms'/>  Value: [] too high.\n", fb); }
            else if (pos == -3) { MAX98357A::speakformatted("Tumble servo error 3.\n\n<break time='500ms'/>  Value: [] too high.\n", fb); }
            else { MAX98357A::speakformatted("Tumble servo error.\n<break time='500ms'/>  Position: []\n<break time='700ms'/>  Expected: {}", fb, pos); }
            break;
            }
        case 3: 
            {// grip error
            int fb, pos;
            Servos::Grip::getError(fb, pos);
            if (pos == -1) { MAX98357A::speakformatted("Grip servo error.\n\n<break time='500ms'/>  Value: [] too low.", fb); }
            else { MAX98357A::speakformatted("Grip servo error.\n<break time='500ms'/>  Position []\n<break time='700ms'/>  Expected {}", fb, pos); }
            break;
            }
        default:
            ABORT("Unknown panic flag");
        }
    
    while (MAX98357A::isSpeaking())
        {
        panicLights(em_poff);
        _updateWithPB(em_poff, poff_ms);
        threads.yield();
        }

    faceAction::shuteyes(1000000); // shut eyes until poweroff

    while (em_poff < poff_ms - 3600)
        {
        panicLights(em_poff);
        _updateWithPB(em_poff, poff_ms);
        threads.yield();
        }

    Console::console.clear();
    Console::console.setCursor(3, cy);
    Console::console.showCursor(true); // hide the cursor
    Console::console.setCurrentColor(TFT_CONSOLE_MESSAGE_COLOR);
    Console::console.print("Shutdown in progress...");
    Console::console.setCursor(0, 4);
    Console::console.setCurrentColor(TFT_CONSOLE_COLOR);
    faceAction::disapear(2800);
    MAX98357A::speak("  Goodbye cruel world.");

    while (em_poff < poff_ms)
        {
        panicLights(em_poff);
        _updateWithPB(em_poff, poff_ms);
        threads.yield();
        }

    PowerCtrl::powerOff();
    }



SLOWFUN void during_move_audio(int nbm, int nbr, int & phase)
    {
    static int prev_nbr = -1;
    static uint8_t music_index = 0; 
    static uint32_t lasttouched = 0;
    switch (phase)
        {
        case 1:
            { // trivial solution, do nothing until finish solving the cube. 
            break;
            }
        case 2: 
            { // moderate solution
            if (!MAX98357A::isSpeaking())
                { // done initial speaking, try countdown. 
                graphics3D::makeRendererTransitionToBig();
                if (nbr >= COUNTDOWN_MIN_MOVES)
                    { // initiate countdown
                    Console::console.print("\n\n");
                    MAX98357A::speakformatted(sentences::remaining_moves_str, nbr);
                    phase = 3; // move to phase 3
                    }
                else
                    {
                    phase = 4; // too late, no countdown
                    }
                }
            break;
            }
        case 3: 
            { // countdown in progress
            if ((!MAX98357A::isSpeaking())&&(prev_nbr != nbr)&&(nbr != 0))
                {
                Console::console.setCurrentColor(TFT_CONSOLE_MESSAGE_COUNT);
                MAX98357A::speakformatted("[]\n", nbr);               
                prev_nbr = nbr;
                }
            break;
            }
        case 4:
            { // too late for countdown do nothing
            break;
            }        
        case 5:
            { // normal solution 
            if (!MAX98357A::isSpeaking())
                { // done initial speaking, chooose music and announce it.
                music_index = songs::picker_song_solve.next(); // choose music
                MAX98357A::speakformatted(songs::song_solve_intro_sentence(music_index), nbr);               
                phase = 6;
                }
            break;
            }
        case 6:
            { // start music
            if ((!MAX98357A::isSpeaking()) && (prev_nbr != nbr))
                {
                graphics3D::allowTouchTransition(false);
                lasttouched = Display::touchedTime();
                Console::console.print("\n\n");
                MAX98357A::setAudioLevelMusic();
                MAX98357A::audioLed.setDriver(MAX98357A::AudioLed::DRIVER_MUSIC);
                MAX98357A::playMusic(songs::song_solve_filename(music_index), false);
                phase = 7;
                }
            break;
            }                       
        case 7:
            { // music in progress
            if ((prev_nbr != nbr) && (nbr != 0))
                {
                Console::console.setCurrentColor(TFT_CONSOLE_MESSAGE_COUNT);
                misc::formatText(Memory::speak_text_buffer, "[]\n", nbr);
                Console::console.print(Memory::speak_text_buffer);
                prev_nbr = nbr;
                if ((nbr == nbm/2)) { graphics3D::makeRendererTransitionToBig(); }
                }
            if (Display::touchedTime() > lasttouched + 200)
                { // user touched the screen, stop the music
                MAX98357A::fadeOutMusic();
                MAX98357A::stopSpeak();
                MAX98357A::audioLed.setDriver(MAX98357A::AudioLed::DRIVER_VOICE);
                misc::formatText(Memory::speak_text_buffer, sentences::picker_res_music_abort.next_str(), nbm);
                Console::console.setCurrentColor(TFT_CONSOLE_COLOR);
                Console::console.print("\n");
                MAX98357A::speak(Memory::speak_text_buffer);
                phase = 8;
                }
            break; 
            }
        case 8:
            { // music aborted, wait for speaking to finish
            if (!MAX98357A::isSpeaking())
                {
                graphics3D::allowTouchTransition(true);
                Console::console.print("\n\n");
                if (nbr >= COUNTDOWN_MIN_MOVES)
                    { // initiate countdown
                    MAX98357A::speakformatted(sentences::remaining_moves_str, nbr);
                    } 
                phase = 9; // move to final phase
                }
            break;
            }
        case 9:
            { // in countdown
            if ((!MAX98357A::isSpeaking()) && (prev_nbr != nbr) && (nbr != 0))
                {
                Console::console.setCurrentColor(TFT_CONSOLE_MESSAGE_COUNT);
                MAX98357A::speakformatted("[]\n", nbr);
                prev_nbr = nbr;
                if ((nbr == nbm / 2)) { graphics3D::makeRendererTransitionToBig(); }
                }
            break;
            }
        default:
            {
            ABORT("IMPOSSIBLE CASE")
            }
        }
    }

/**
 * - reprendre les phrase UTF etr formatagge.   
 * - mettre en francias  
 * - normaliser le niveau sonore des musiques. 
***/

SLOWFUN static int make_solve_moves(const char* sol, int & nbm)
    {    
    nbm = solLen(sol);  // number of moves in the solution
    int nbr = nbm;      // number of remaining moves

    memset(Memory::previous_solve_moves, 0, sizeof(Memory::previous_solve_moves)); // erase previous list of moves
    int ips = 0; // index in previous_solve_moves    

    MAX98357A::audioLed.setDriver(MAX98357A::AudioLed::DRIVER_VOICE); // set the audio LED driver to voice mode
    MAX98357A::waitEndSpeak(); // wait for end of previous speech is needed (probably not)
    if (getPanicFlag()) { panicDuringSolve(); }
    Console::console.print("\n\n"); // and move two lines down.

    int phase = 0;
    if (nbm <= TRIVAL_SOLVE_MAX_MOVES)
        { // trivial solution. 
        misc::formatText(Memory::speak_text_buffer,sentences::picker_res_trivial.next_str(), nbm);
        phase = 1; 
        }
    else if (nbm <= MODERATE_SOLVE_MAX_MOVES)
        { // moderate solution. 
        misc::formatText(Memory::speak_text_buffer,sentences::picker_res_moderate.next_str(), nbm);
        graphics3D::makeRendererTransitionToSmall();
        phase = 2;
        }
    else
        { // normal solution. 
        misc::formatText(Memory::speak_text_buffer,sentences::solution_found_str, nbm);
        graphics3D::makeRendererTransitionToSmall();
        phase = 5;
        }
    MAX98357A::speak(Memory::speak_text_buffer); // start the first sentence. 

    int da2 = (rng::rand(2) == 0) ? 2 : -2; // direction for half turns

    // start solving the cube
    DRV8834::enable(true);

    while (sol[0] != 0)
        { // main solving loop
        if (sol[0] == ' ') { sol++;  continue; }

        // ok, there is still a move to perfom 
        CubeState::CubeMove cm;
        sol = cm.set(sol);
        if (!cm.valid()) { ABORT("Invalid move"); }

        const char* mm = CubeOrientation::cube.find_move_for_top(cm.face());

        // move to rotate to the top 
        while (mm[0] != 0)
            {
            CubeOrientation::cube.make_move(mm[0]);
            switch (mm[0])
                {
                case 'T':
                {
                Memory::previous_solve_moves[ips++] = 'T';
                BasicMoves::tumble_start();
                while (BasicMoves::tumble_progress() < 1.0f) 
                    { 
                    during_move_audio(nbm, nbr, phase); 
                    graphics3D::updateAll(2); 
                    if (getPanicFlag()) { panicDuringSolve(); }
                    threads.yield();
                    }
                during_move_audio(nbm, nbr, phase);
                graphics3D::cube3D.tumble();                
                if (mm[1] == 'T')
                    { // double tumble so we close the grip in between to insure first tumble is ok.
                    BasicMoves::closeGrip_start(true, false);
                    while (BasicMoves::closeGrip_progress() < 1.0f) 
                        { 
                        during_move_audio(nbm, nbr, phase); 
                        graphics3D::updateAll(1); 
                        if (getPanicFlag()) { panicDuringSolve(); }
                        threads.yield();
                        }
                    BasicMoves::openGrip_start(false, true);
                    while (BasicMoves::openGrip_progress() < 1.0f) 
                        { 
                        during_move_audio(nbm, nbr, phase);  
                        graphics3D::updateAll(1); 
                        if (getPanicFlag()) { panicDuringSolve(); }
                        threads.yield();
                        }
                    }
                break;
                }
                case 'R':
                { // clockwise
                const int dir = 1;
                Memory::previous_solve_moves[ips++] = 'R';
                BasicMoves::turn_start(dir);
                while (BasicMoves::turn_progress() < 1.0f) 
                    { 
                    during_move_audio(nbm, nbr, phase); 
                    graphics3D::updateAll(3, dir); 
                    if (getPanicFlag()) { panicDuringSolve(); }
                    threads.yield();
                    }
                during_move_audio(nbm, nbr, phase);
                graphics3D::cube3D.turn(dir);
                break;
                }
                case 'V':
                { // anticlockwise
                const int dir = -1;
                Memory::previous_solve_moves[ips++] = 'V';
                BasicMoves::turn_start(dir);
                while (BasicMoves::turn_progress() < 1.0f) 
                    { 
                    during_move_audio(nbm, nbr, phase); 
                    graphics3D::updateAll(3, dir); 
                    if (getPanicFlag()) { panicDuringSolve(); }
                    threads.yield();
                    }
                during_move_audio(nbm, nbr, phase);
                graphics3D::cube3D.turn(dir);
                break;
                }
                case 'A':
                { // halfturn
                const int dir = da2; 
                Memory::previous_solve_moves[ips++] = 'A';
                da2 = -da2;
                BasicMoves::turn_start(dir);
                while (BasicMoves::turn_progress() < 1.0f) 
                    { 
                    during_move_audio(nbm, nbr, phase); 
                    graphics3D::updateAll(3, dir); 
                    if (getPanicFlag()) { panicDuringSolve(); }
                    threads.yield();
                    }
                during_move_audio(nbm, nbr, phase);
                graphics3D::cube3D.turn(dir);
                break;
                }
                default: ABORT("CubeOrientation::make_move() incorrect move");
                }
            mm++;
            }

        // perform the rotation

        BasicMoves::closeGrip_start(true, false);
        while (BasicMoves::closeGrip_progress() < 1.0f) 
            { 
            during_move_audio(nbm, nbr, phase); 
            graphics3D::updateAll(1); 
            if (getPanicFlag()) { panicDuringSolve(); }
            threads.yield();
            }
        DEBUG(" ROTATION TYPE " << cm.type() << "\n");
        const int dir = (cm.type() == 0 ? -1 : ((cm.type() == 1) ? 1 : da2)); // dir for the bottom turn
        if (cm.type() == 2) da2 = -da2;
        BasicMoves::turn_start(dir);
        while (BasicMoves::turn_progress() < 1.0f) 
            {
            during_move_audio(nbm, nbr, phase);
            graphics3D::updateAll(4, dir); 
            if (getPanicFlag()) { panicDuringSolve(); }
            threads.yield();
            }
        graphics3D::cube3D.turn_bottom(dir);

        CubeOrientation::cube.make_move(cm.type() == 0 ? 'V' : ((cm.type() == 1) ? 'R' : 'A'));
        Memory::previous_solve_moves[ips++] = ((cm.type() == 0) ? 'v' : ((cm.type() == 1) ? 'r' : 'a'));

        BasicMoves::openGrip_start(false, true);
        while (BasicMoves::openGrip_progress() < 1.0f) 
            {
            during_move_audio(nbm, nbr, phase);  
            graphics3D::updateAll(1); 
            if (getPanicFlag()) { panicDuringSolve(); }
            threads.yield();
            }

        nbr--; // one less move to do
        }

    BasicMoves::openGrip_start(true, true);
    while (BasicMoves::openGrip_progress() < 1.0f) 
        { 
        during_move_audio(nbm, nbr, phase);
        graphics3D::updateAll(1); 
        if (getPanicFlag()) { panicDuringSolve(); }
        threads.yield();
        }

    // done !
    DRV8834::enable(false);

    DEBUG("LIST OF MOVES FOR REPLAY: [" << Memory::previous_solve_moves << "]\n");

    Console::console.setCurrentColor(TFT_CONSOLE_COLOR);

    switch (phase)
        {
        case 1: 
            {
            while (MAX98357A::isSpeaking()) { graphics3D::updateAll(1); } // wait until done speaking
            Console::console.print("\n\n");
            MAX98357A::speakformatted(sentences::picker_res_trivial_tail.next_str(), nbm);
            break;
            }
        case 2:
        case 3:
        case 4:
            {
            while (MAX98357A::isSpeaking()) { graphics3D::updateAll(1); } // wait until done speaking
            if (Console::console.getCursor().x == 0) { Console::console.print("\n"); } else { Console::console.print("\n\n"); }
            MAX98357A::speakformatted(sentences::picker_res_moderate_tail.next_str(), nbm);
            break;
            }
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
            {
            MAX98357A::fadeOutMusic();
            MAX98357A::stopSpeak(); 
            MAX98357A::audioLed.setDriver(MAX98357A::AudioLed::DRIVER_VOICE);
            if (Console::console.getCursor().x == 0) { Console::console.print("\n"); } else { Console::console.print("\n\n"); }
            MAX98357A::speakformatted(sentences::picker_victory_normal.next_str(), nbm);            
            break;
            }
        default:
            {
            ABORT("IMPOSSIBLE CASE")
            }
        }
    if (getPanicFlag()) { panicDuringSolve(); }
    return ((nbm <= TRIVAL_SOLVE_MAX_MOVES) ? CODE_CUBE_TRIVIAL :  ((nbm <= MODERATE_SOLVE_MAX_MOVES) ? CODE_CUBE_MODERATE : CODE_CUBE_FULL));
    }




/**
* Returns: 
* - CODE_CUBE_MISSING : no cube in craddle
* - CODE_CUBE_INVALID : cube colors could not be read (cube is invalid)  
* - CODE_CUBE_ALREADY_SOLVED : already solved
* - 0 : ok, cube read correctly
**/
SLOWFUN static int read_cube()
    {
    // read the cube   
    int res = CubeRead::readCubeAndFindConfiguration();
    if (getPanicFlag()) { panicDuringSolve(); }

    if (res == CODE_CUBE_MISSING)
        { // there is no cube !
        MAX98357A::audioLed.setDriver(MAX98357A::AudioLed::DRIVER_VOICE); // set the audio LED driver to voice mode
        MAX98357A::speak(sentences::picker_victory_nocube.next_str());
        graphics3D::resetColorStickers(true);
        graphics3D::cube3D.resetPosition();
        return CODE_CUBE_MISSING;
        }

    if (res == CODE_CUBE_INVALID)
        { // cube is invalid
        BasicMoves::openGrip_start(true, true); // open grip completely
        while (BasicMoves::openGrip_progress() < 1.0f) 
            { 
            if (getPanicFlag()) { panicDuringSolve(); }
            graphics3D::updateAll(1); 
            threads.yield(); 
            }
        while (MAX98357A::isSpeaking()) { graphics3D::updateAll(1); threads.yield(); } // wait until done speaking
        Console::console.print("\n\n"); // and move two lines down.
        MAX98357A::speak(sentences::picker_victory_invalid.next_str());
        graphics3D::resetColorStickers(true);
        graphics3D::cube3D.resetPosition();
        return CODE_CUBE_INVALID;
        }

    if (CubeState::cube.isSolved())
        { // cube is already solved
        BasicMoves::openGrip_start(true, true); // open grip completely
        while (BasicMoves::openGrip_progress() < 1.0f) 
            { 
            if (getPanicFlag()) { panicDuringSolve(); }
            graphics3D::updateAll(1); threads.yield(); 
            }
        while (MAX98357A::isSpeaking()) { graphics3D::updateAll(1); threads.yield(); } // wait until done speaking
        Console::console.print("\n\n"); // and move two lines down.
        MAX98357A::speak(sentences::picker_victory_alreadysolved.next_str());
        graphics3D::resetColorStickers(true);
        graphics3D::cube3D.resetPosition();
        return CODE_CUBE_ALREADY_SOLVED;
        }

    // ok, put the stickers on the cube
    for (int i = 0; i < 54; i++)
        {
        tgx::RGB565 col = CubeRead::stickerAverageColor(i);
        graphics3D::stickerImage(i).clear(col); // clear the sticker image with the average color
        graphics3D::applyStickerMask(i);
        }
    if (getPanicFlag()) { panicDuringSolve(); }
    return 0;
    }







static volatile int finished_kociemba = 0;

SLOWFUN static void display_during_kociemba()
    {
    tgx::fMat4 M;
    M.setIdentity();
    while (!finished_kociemba)
        {
        M.multRotate(90.0f, { 0.0f, 1.0f, 0.0f }); // rotate the cube around the Y axis    
        graphics3D::updateAll(Display::image_extmem, Display::image_extmem_2, 1, 0, &M); // update the display with the cube at its current position
        threads.delay(150);
        }
    }


SLOWFUN static const char* compute_kociemba()
    {
    teensythread::fillStack(Memory::basicmoves_stack, BASICMOVES_STACK_SIZE);

    // save ui image in extmem
    tgx::Image<tgx::RGB565> im_extmem_2 = Display::image_extmem_2;
    im_extmem_2.blit(Display::image_alt, { 0,0 }); // restore the UI image   

    graphics3D::renderer.setZbuffer(Memory::zbuffer_extmem);
    graphics3D::renderer.setImage((tgx::Image<tgx::RGB565>*) & (Display::image_extmem));
    Display::displayToEXTMEM(true); // move display internal fb to EXTMEM. 

    finished_kociemba = 0;
    int th = threads.addThread(display_during_kociemba, 0, BASICMOVES_STACK_SIZE, Memory::basicmoves_stack);
    teensythread::speedUp(th, 0.5f);

    elapsedMillis ee = 0;
    kociemba::set_memory(Memory::dma_shared.B.kociemba_479);

    DRV8834::enable(true);
    DRV8834::turn(1, 2 * 360, 0, 16, DRV8843_TURN_FREQ_START_KOCIEMBA, DRV8843_TURN_FREQ_MAX_KOCIEMBA, DRV8843_TURN_ALPHA_KOCIEMBA, true);


    const char* sol = kociemba::solve(CubeRead::cubeString());  // test_cube[0]
    Serial.printf("kociemba in %d ms , stack %d\n", (int)ee, teensythread::stackUsage(Memory::basicmoves_stack, BASICMOVES_STACK_SIZE));

    DRV8834::stopContinuousSpin();

    finished_kociemba = 1;
    threads.wait(th);

    graphics3D::renderer.setZbuffer(Memory::dma_shared.A.zbuffer); // restore the zbuffer
    graphics3D::renderer.setImage((tgx::Image<tgx::RGB565>*) & (Display::image_main)); // restore the image
    Display::displayToEXTMEM(false); // restore the internal framebuffer

    //restore ui image from extmem
    tgx::Image<tgx::RGB565> im_ui = Display::image_alt;
    im_ui.blit(Display::image_extmem_2, { 0,0 });

    if (getPanicFlag()) { panicDuringSolve(); }

    if (sol == nullptr)
        {
        BasicMoves::openGrip_start(true, true); // open grip completely
        while (BasicMoves::openGrip_progress() < 1.0f) 
            { 
            if (getPanicFlag()) { panicDuringSolve(); }
            graphics3D::updateAll(1);
            threads.yield(); 
            }
        }

    while (!DRV8834::isTurnComplete())
        {
        if (getPanicFlag()) { panicDuringSolve(); }
        graphics3D::updateAll(1); // redraw the screen
        threads.yield(); // 
        }
    DRV8834::enable(false);

    if (sol == nullptr)
        {
        Console::console.print("\n\n"); // and move two lines down.
        MAX98357A::speak(sentences::picker_victory_invalid.next_str());
        return sol;
        }
    return sol;
    }


int solveCube(int& cube_difficulty, int& nb_moves)
    {
    graphics3D::allowTouchTransition(true); 

    DRV8834::allowPanic(false);  // we monitor panic ourself
    Servos::allowPanic(false);   //
    nb_moves = 0; 
    int b = read_cube();
    if (b == 0)
        { // cube read correctly
        const char* s = compute_kociemba();
        if (s)
            { // ok, solve it
            cube_difficulty = make_solve_moves(s, nb_moves);
            } 
        else 
            { // cube is invalid
            cube_difficulty = CODE_CUBE_INVALID;
            graphics3D::resetColorStickers(true);
            graphics3D::cube3D.resetPosition();
            }
        }
    else
        {
        cube_difficulty = b;
        }

    if (getPanicFlag()) { panicDuringSolve(); }
    DRV8834::allowPanic(true);  // restore panic
    Servos::allowPanic(true);   //

    DEBUG("basic moves stack: " << teensythread::stackUsage(Memory::basicmoves_stack, BASICMOVES_STACK_SIZE));
    DEBUG("espeak thread stack: " << teensythread::stackUsage(Memory::espeak_stack, ESPEAK_STACK_SIZE));
    elapsedMillis em = 0;
    uint32_t disappear_len = 1000;
    tgx::fMat4 M;
    while ((MAX98357A::isSpeaking()) || ( em < disappear_len))
        {
        if (CraddleMove::status() == CODE_SPINNED)
            { // let's do it again
            MAX98357A::stopMusic();
            return CODE_SPINNED;
            }
        float t = 1.0f - (em / ((float)disappear_len));
        if (t <= 0) { t = 0; }
        t *= t;
        if (t > 0)
            {
            M.setScale(t, t, t);
            graphics3D::updateAll(1, 0, &M);
            }
        else
            {
            graphics3D::updateAll(0);
            }
        threads.yield();
        }
    MAX98357A::stopMusic(); 
    Console::console.print("\n"); 
    return 0;
    }



/** end of file */

