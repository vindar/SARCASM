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


#include <Arduino.h>
#include <TeensyThreads.h>
#include <tgx.h>
#include "config.h"
#include "debug.h"
#include "display.h"
#include "sarcasm_bk.h"
#include "sarcasm_bk_blink.h"
#include "sarcasm_bk_closemouth.h"
#include "sarcasm_bk_closemouth_blink.h"
#include "sarcasm_bk_openmouth.h"
#include "sarcasm_bk_openmouth_blink.h"
//#include "sarcasm_bk_wink.h"
#include "panic.h"
#include "LipSync.h"
#include "xorshift32.h"
#include "faceAction.h"
#include "audioMAX98357A.h"


namespace faceAction
    {

 


    float normalize(float x, float tmin, float tmax)
        {
        const float d = tmax - tmin;
        if (d <= 0) return 0.5f;
        if (x < tmin) return 0.0f;
        if (x > tmax) return 1.0f;
        return (x - tmin) / d;
        }


    float lerp(float x, float a, float b)
        {
        return a + (b - a) * x;
        }


    struct FaceParams
        {
        float op;
        int BITQUANT;
        float WAVE_SPEED;
        float WAVE_AMPLITUDE;
        float WAVE_LENGTH;
        };


    FaceParams lerp(float r, FaceParams& startP, FaceParams& endP)
        {
        FaceParams p;
        p.op = lerp(r, startP.op, endP.op);
        p.BITQUANT = round(lerp(r, (float)startP.BITQUANT, (float)endP.BITQUANT));
        p.WAVE_SPEED = lerp(r, startP.WAVE_SPEED, endP.WAVE_SPEED);
        p.WAVE_AMPLITUDE = lerp(r, startP.WAVE_AMPLITUDE, endP.WAVE_AMPLITUDE);
        p.WAVE_LENGTH = lerp(r, startP.WAVE_LENGTH, endP.WAVE_LENGTH);
        return p;
        }



    /**
    * Blending with dithering and wave deformation.
    **/
    SLOWFUN void blendIm(tgx::Image<tgx::RGB565> dst, const tgx::Image<tgx::RGB565> src, const FaceParams& P, float phase)
        {
        if (!dst.isValid() || !src.isValid()) { ABORT("Impossible error: invalid images"); }
        
        const float WAVE_FREQ = (1.0f / P.WAVE_LENGTH);
        int MASK = (1 << P.BITQUANT) - 1;
        if (P.op >= 1.0f) { dst.blit(src, { 0,0 }); return; }
        if (P.op <= 0.0f) return;

        const uint32_t alpha = (uint32_t)(P.op * 255.0f);
        const uint32_t a1 = (alpha >> P.BITQUANT); // quantize
        const uint32_t r = alpha - (a1 << P.BITQUANT); // remainder

        for (int j = 0; j < 240; j++)
            {
            const int off = j * 320;
            tgx::RGB565* p = dst.data() + off;
            const tgx::RGB565* q = src.data() + off;
            uint32_t a = ((((rng::rand()) & MASK) < r) ? (a1 + 1) : a1); // dithering the alpha value to reduce banding            
            a <<= P.BITQUANT;
            int o = sin((j + phase) * WAVE_FREQ) * P.WAVE_AMPLITUDE;
            if (o >= 0)
                {
                for (int i = 0; i < 320 - o; i++)
                    {
                    (p[i + o]).blend256(q[i], a);
                    }
                } else
                {
                for (int i = 0; i < 320 + o; i++)
                    {
                    (p[i]).blend256(q[i - o], a);
                    }
                }
            }
        }





    static elapsedMillis _face_em;  // elapsed time since the beginning of the face animation
    static uint32_t _face_len;      // total length of the face animation

    FaceParams _face_start = { 0.0f, 7, 20.0f, 10.0f, 5.0f };    // parameter at the start of the animation
    FaceParams _face_cur;           // current parameter during the animation
    FaceParams _face_end;           // parameter at the end of the animation
    float _face_phase = 0;          // current phase for the wave effect

    int _face_state = 0;            // index of the face animation
    int _face_substate = 0;         // the substate of the face animation
    float _face_multscale = 0.0f;   // time scling factor for the face animation



    PROGMEM const FaceParams state_visible_cube_replay = { 0.7f, 0, 0.0f, 0.0f, 0.0f };

    

    PROGMEM const FaceParams state_visible_cube = { 0.6f, 0, 0.0f, 0.0f, 0.0f };
    PROGMEM const FaceParams state_visible_cube_solved = { 0.7f, 0, 0.0f, 0.0f, 0.0f };

    PROGMEM const FaceParams state_visible_good = { 0.4f, 0, 0.0f, 0.0f, 0.0f };
    PROGMEM const FaceParams state_visible_dark = { 0.2f, 3, 0.0f, 0.0f, 0.0f };


    PROGMEM const FaceParams state_visible_hlines = { 0.4f, 7, 0.0f, 0.0f, 0.0f };
    PROGMEM const FaceParams state_visible_waves = { 0.4f, 1, 1.0f, 4.0f, 2.0f };

    PROGMEM const FaceParams state_invisible = { 0.0f, 7, 20.0f, 10.0f, 5.0f };


    PROGMEM const FaceParams state_visible_dreaming = { 0.7f, 0, 0.0f, 0.0f, 30.0f };
    PROGMEM const FaceParams state_visible_dreaming_end = { 0.9f, 7, 0.1f, 10.0f, 50.0f };



    // for blinking
    static const uint32_t BLINK_MIN_MS = 1300;  // intervalle 2.4–6.0 s
    static const uint32_t BLINK_MAX_MS = 5000;
    static const uint32_t BLINK_LEN_MIN_MS = 60;    // durée fermeture 60–110 ms
    static const uint32_t BLINK_LEN_MAX_MS = 120;
    static const uint8_t  BLINK_DOUBLE_CHANCE = 15;    // % de double blink
    static bool          blink_active = false;        // yeux fermés ?
    static bool          blink_inited = false;        // init faite ?
    static uint32_t      blink_len = 0;               // durée de ce blink (ms)
    static elapsedMillis blink_em = 0;                // écoulé depuis début du blink
    static elapsedMillis blink_gap_em = 0;            // écoulé depuis le dernier blink
    static uint32_t      blink_next_ms = 0;           // délai avant prochain blink
    static LipSync::MouthState prevM = LipSync::MouthState::M_NEUTRAL;   

    static uint32_t shuteyes_len;
    static elapsedMillis shuteyes_em;

    static int _prev_good_state = -1; // previous good state alteration (0 = drak, 1 = hlines, 2 = waves)


    tgx::Image<tgx::RGB565> _src_image; // if not null, use this image instead of the face


    void drawFace(tgx::Image<tgx::RGB565>& im)
        {
        // compute current parameter
        const uint32_t t = _face_em;
        const float r = normalize((float)t, 0, (float)_face_len);
        _face_cur = lerp(r, _face_start, _face_end);
        if (rng::rand(2) == 1) { _face_phase += _face_cur.WAVE_SPEED; } else { _face_phase -= _face_cur.WAVE_SPEED; }
        
        // select image : mouth position + blink
        tgx::Image<tgx::RGB565> sarcasm_im = sarcasm_bk;

        // new blink code
        if (!blink_inited) 
            {
            blink_inited = true;
            blink_next_ms = rng::rand(BLINK_MIN_MS, BLINK_MAX_MS);
            blink_len = 0;
            blink_em = 0;
            blink_gap_em = 0;
            }

        LipSync::MouthState M = LipSync::mouthState();
        bool speaking = LipSync::speaking_recent();   // blink continue même sans voix
        // Blink juste après une bouche ouverte qui se referme
        if (!blink_active && prevM == LipSync::MouthState::M_OPEN && !speaking) 
            {
            blink_active = true;
            blink_len = (uint32_t)rng::rand(BLINK_LEN_MIN_MS, BLINK_LEN_MAX_MS);
            blink_em = 0;
            blink_gap_em = 0;
            blink_next_ms = (uint32_t)rng::rand(BLINK_MIN_MS, BLINK_MAX_MS);
            }
        // Planification normale
        if (!blink_active) 
            {            
            if (blink_gap_em >= blink_next_ms && !speaking) 
                {
                blink_active = true;
                blink_len = (uint32_t)rng::rand(BLINK_LEN_MIN_MS, BLINK_LEN_MAX_MS);
                blink_em = 0;
                blink_gap_em = 0;
                } 
            else if (speaking) 
                    {                    
                    blink_next_ms += 40;
                    }
                } 
            else 
                {
                if (blink_em >= blink_len) 
                    {
                    blink_active = false;
                    if (!speaking && rng::rand(0, 100) < BLINK_DOUBLE_CHANCE)
                        {
                        blink_next_ms = (uint32_t)rng::rand(120, 220);  // 2e blink très proche
                        } 
                    else
                        {
                        blink_next_ms = (uint32_t)rng::rand(BLINK_MIN_MS, BLINK_MAX_MS);
                        }
                    }
                }
        bool useblink = blink_active || (shuteyes_em < shuteyes_len);
        prevM = M;
        if (M == LipSync::MouthState::M_OPEN) 
            {
            sarcasm_im = useblink ? sarcasm_bk_openmouth_blink : sarcasm_bk_openmouth;
            } 
        else if (M == LipSync::MouthState::M_CLOSE) 
            {
            sarcasm_im = useblink ? sarcasm_bk_closemouth_blink : sarcasm_bk_closemouth;
            } 
        else 
            { // M_NEUTRAL
            sarcasm_im = useblink ? sarcasm_bk_blink : sarcasm_bk;
            }

        

        // draw the UI with the face (or the _src_image if supplied)
        if (_face_cur.op < 1.0f) { im.clear(tgx::RGB565_Black); }        
        
        blendIm(im, (_src_image.isValid() ? _src_image : sarcasm_im), _face_cur, _face_phase);
        
        if (t < _face_len) return; // animation not finished yet

        // animation if finished. check for the next one.

        _face_start = _face_end;
        _face_cur = _face_end;

        switch (_face_state)
            {
            case 0:
            { // invisible : stay invisible
            _face_end = state_invisible;
            _face_len = 10000;
            _face_em = 0;
            break;
            }
            case 1:
            { // visible good : stay visible good
            switch (_face_substate)
                {
                // Main state : "visible good"
                case 0:
                { // choose how long we stay in "good visibility" 
                _face_end = _face_cur;
                _face_substate = 1;
                _face_len = rng::rand(10, 6000);
                _face_em = 0;
                break;
                }

                case 1:
                { // switch to one of the dark (0) hline (1) wigly states (2)
                int next_state;
                for (int k = 0; k < 2; k++)
                    { // choose the next state but reduce the probability of taking the same state again
                    int a = rng::rand(100);
                    if (a < 33) { next_state = 0; } // 35% chance going dark
                    else if (a < 66) { next_state = 1; } // 45% chance hlines
                    else { next_state = 2; } // 20% chance waves
                    if (next_state != _prev_good_state) break; // not the same as last time
                    }
                _prev_good_state = next_state;
                _face_em = 0;
                switch (_prev_good_state)
                    {
                    case 0:
                    { // going dark
                    _face_end = state_visible_dark;
                    _face_end.op = rng::rand(15, 30) / 100.0f;
                    _face_substate = 2;
                    _face_len = rng::rand(1000, 3000);
                    _face_em = 0;
                    break;
                    }
                    case 1:
                    { // going hlines
                    _face_end = state_visible_hlines;
                    _face_end.BITQUANT = rng::rand(5, 10);
                    if (_face_end.BITQUANT > 7) _face_end.BITQUANT = 7;
                    _face_substate = 4;
                    _face_len = rng::rand(500, 1000);
                    _face_em = 0;
                    break;
                    }
                    case 2:
                    { // going waves
                    _face_end = state_visible_waves;
                    _face_end.BITQUANT = rng::rand(0, 10);
                    if (_face_end.BITQUANT > 7) _face_end.BITQUANT = 0;
                    _face_end.WAVE_SPEED = rng::rand(5, 20) / 10.0f;
                    _face_end.WAVE_AMPLITUDE = rng::rand(2, 7);
                    _face_end.WAVE_LENGTH = rng::rand(2, 6);
                    _face_substate = 6;
                    _face_len = rng::rand(200, 2000);
                    _face_em = 0;
                    break;
                    }
                    default: ABORT("Impossible case");
                    }
                break;
                }

                // Going dark
                case 2:
                { // finished going dark, stay a little while like that
                _face_end = _face_cur;
                _face_substate = 3;
                _face_len = rng::rand(10, 1500);
                _face_em = 0;
                break;
                }
                case 3:
                { // finished dark
                _face_end = state_visible_good;
                _face_substate = 0;
                _face_len = rng::rand(1000, 2000);
                _face_em = 0;
                break;
                }

                // going hlines
                case 4:
                { // finished going hline, stay in hline a little
                _face_end = _face_cur;
                _face_substate = 5;
                _face_len = rng::rand(10, 1500);
                _face_em = 0;
                break;
                }
                case 5:
                { // back to good visibility
                _face_end = state_visible_good;
                _face_substate = 0;
                _face_len = rng::rand(100, 1000);
                _face_em = 0;
                break;
                }

                // going waves
                case 6:
                { // finished going waves, stay in waves a little
                _face_end = _face_cur;
                _face_substate = 7;
                _face_len = rng::rand(10, 1500);
                _face_em = 0;
                break;
                }
                case 7:
                { // back to good visibility
                _face_end = state_visible_good;
                _face_substate = 0;
                _face_len = rng::rand(100, 1000);
                _face_em = 0;
                break;
                }

                default: ABORT("Impossible case");
                }
            break;
            }

            case 2:
            { // visible hlines
            _face_end = state_visible_hlines;
            _face_len = 1000;
            _face_em = 0;
            break;
            }
            case 3:
            { // visible waves
            _face_end = state_visible_waves;
            _face_len = 1000;
            _face_em = 0;
            break;
            }
            case 4:
            { // appearing intro
            switch (_face_substate)
                {
                case 0:
                {
                _face_end = { 0.55f, 5, 2.0f, 4.0f, 20.0f };
                _face_substate = 1;
                _face_len = (uint32_t)(1000 * _face_multscale);
                _face_em = 0;
                break;
                }
                case 1:
                {
                _face_end = { 0.45f, 1, 1.0f, 2.0f, 25.0f };
                _face_substate = 2;
                _face_len = (uint32_t)(2000 * _face_multscale);
                _face_em = 0;
                break;
                }
                case 2:
                {
                _face_end = state_visible_good;     // { 0.5f, 0, 1.0f, 2.0f, 30.0f };
                _face_substate = 3; // initial good visibility
                _face_len = (uint32_t)(1000 * _face_multscale);
                _face_em = 0;
                break;
                }
                case 3:
                {
                _face_end = state_visible_good;
                _face_state = 1; // visible good
                _face_substate = 0; // initial good visibility
                _face_len = rng::rand(1000, 3000);
                _face_em = 0;
                break;
                }
                default: ABORT("Impossible case");
                }
            break;
            }
            case 5:
            { //  appearing
            _face_end = state_visible_good;
            _face_state = 1; // visible good
            _face_substate = 0; // initial good visibility
            _face_len = rng::rand(500, 2000);
            _face_em = 0;
            break;
            }
            case 6:
            { // disappearing
            _face_end = state_invisible;
            _face_state = 0; // invisible
            _face_substate = 0;
            _face_len = 10;
            _face_em = 0;
            break;
            }
            case 7:
            { // cube appearing
            _face_end = state_visible_cube;
            _face_state = 9;
            _face_substate = 0;
            _face_len = 1000;
            _face_em = 0;
            break;
            }
            case 8:
            { // cube disappearing
            _face_end = state_invisible;
            _face_state = 0; // invisible
            _face_substate = 0;
            _face_len = 10;
            _face_em = 0;
            break;
            }


            case 9:
            { // visible cube  : stay visible cube
            switch (_face_substate)
                {
                // Main state : "visible good cube"
                case 0:
                { // choose how long we stay in "good visibility" 
                _face_end = _face_cur;
                _face_substate = 1;
                _face_len = rng::rand(4000, 8000);
                _face_em = 0;
                break;
                }
                case 1:
                { // hline or wigly states
                int next_state;
                for (int k = 0; k < 2; k++)
                    { // choose the next state but reduce the probability of taking the same state again
                    int a = rng::rand(100);
                    if (a < 30) { next_state = 0; } // hline
                    else { next_state = 1; } // waves
                    if (next_state != _prev_good_state) break; // not the same as last time
                    }
                _prev_good_state = next_state;
                _face_em = 0;
                switch (_prev_good_state)
                    {
                    case 0:
                    { // going hlines
                    _face_end = state_visible_hlines;
                    _face_end.BITQUANT = rng::rand(4, 10);
                    if (_face_end.BITQUANT > 7) _face_end.BITQUANT = 7;
                    _face_substate = 2;
                    _face_len = rng::rand(500, 1000);
                    break;
                    }
                    case 1:
                    { // going waves
                    _face_end = state_visible_waves;
                    _face_end.BITQUANT = 0;
                    _face_end.WAVE_SPEED = rng::rand(3, 20) / 10.0f;
                    _face_end.WAVE_LENGTH = rng::rand(6, 40);
                    _face_end.WAVE_AMPLITUDE = rng::rand(_face_end.WAVE_LENGTH / 4, (_face_end.WAVE_LENGTH) / 3);
                    _face_substate = 4;
                    _face_len = rng::rand(1000, 2500);
                    break;
                    }
                    default: ABORT("Impossible case");
                    }
                break;
                }

                // going hlines cube
                case 2:
                { // finished going hline cube, stay a little
                _face_end = _face_cur;
                _face_substate = 3;
                _face_len = rng::rand(100, 1500);
                _face_em = 0;
                break;
                }
                case 3:
                { // back to good visibility cube
                _face_end = state_visible_cube;
                _face_substate = 0;
                _face_len = rng::rand(100, 1000);
                _face_em = 0;
                break;
                }

                // going waves cube
                case 4:
                { // finished going waves cube, stay a little
                _face_end = _face_cur;
                _face_substate = 5;
                _face_len = rng::rand(100, 1000);
                _face_em = 0;
                break;
                }
                case 5:
                { // back to good visibility cube
                _face_end = state_visible_cube;
                _face_substate = 0;
                _face_len = rng::rand(1000, 1500);
                _face_em = 0;
                break;
                }
                default: ABORT("Impossible case");
                }
            break;
            }
            case 10:
            { // visible good cube block (no more change)
            _face_end = state_visible_cube_solved;
            _face_len = 10000;
            _face_em = 0;
            break;
            }

            case 11:
            { // dreaming base : stay for a little while
            switch (_face_substate)
                {
                case 0:
                { // stay dreaming a little while
                _face_end = _face_cur;
                _face_substate = 1;
                _face_len = rng::rand(1000, 3000);
                _face_em = 0;
                break;
                }
                case 1:
                { // change the waves parameters
                /*int a = rng::rand(2);
                if (a == 0)
                    { // little waves
                    _face_end.WAVE_LENGTH = rng::rand(10, 70);
                    _face_end.WAVE_AMPLITUDE = rng::rand(2, 5);
                    _face_end.WAVE_SPEED = rng::rand(30, 150) / 100.0f;
                    }
                else*/
                        { // big waves
                        _face_end.WAVE_LENGTH = rng::rand(5, 80);
                        _face_end.WAVE_AMPLITUDE = _face_end.WAVE_LENGTH / 3.0f;
                        _face_end.WAVE_SPEED = rng::rand(10, 60) / 100.0f;
                        }
                        _face_substate = 0;
                        _face_len = rng::rand(2000, 4000);
                        _face_em = 0;
                        break;
                }
                default: ABORT("Impossible case");
                }
            break;
            }

            case 12:
            { // solve replay
            switch (_face_substate)
                {
                case 0:
                { // stay a little while
                _face_end = _face_cur;
                _face_substate = 1;
                _face_len = rng::rand(3000, 7000);
                _face_em = 0;
                break;
                }
                case 1:
                { // add hline for a small time
                //int a = rng::rand(5,8);
                _face_end = _face_start;
                _face_end.BITQUANT = 0; // a; // NO HLINE IS BETTER FINALY...
                _face_substate = 2;
                _face_len = rng::rand(100, 1000);
                _face_em = 0;
                break;
                }
                case 2:
                { // add hline for a small time
                _face_end = state_visible_cube_replay;
                _face_substate = 0;
                _face_len = rng::rand(500, 2000);
                _face_em = 0;
                break;
                }
                default: ABORT("Impossible case");
                }
            break;
            }

            case 13:
                { // visible_dreaming_end 
                _face_end = state_visible_dreaming_end;
                _face_substate = 0;
                _face_len = 10000;
                _face_em = 0;
                break;
                }
                
            default: ABORT("Impossible case");
            }
        }



    void useFace()
        {
        _src_image = tgx::Image<tgx::RGB565>();
        }


    void useImageInsteadOfFace(const tgx::Image<tgx::RGB565> & src_image)
        {
        _src_image = src_image;
        }


    SLOWFUN void appear_intro(float scale)
        {
        useFace();
        _face_multscale = scale;
        _face_start = state_invisible;
        _face_end = { 0.4f, 6, 10.0f, 4.0f, 10.0f };
        _face_state = 4; // appearing long
        _face_substate = 0; // initial state of the appearing long
        _face_len = (uint32_t)(3000 * _face_multscale);
        _face_em = 0;
        }

    SLOWFUN void appear(int ms)
        { // op, quant, speed, ampl, lenght

        // first time is appear_intro()
        static int _called = 0; 
        if (_called == 0)
            {
            _called++;
            appear_intro(1.0f);
            return;
            }

        useFace();
        if (ms <= 0)
            {
            ms = rng::rand(500, 2000);
            }

        // amount of hline
        int q = rng::rand(9);
        if (q > 7) q = 7; // 22% chance strong hline
        if (q < 3) q = 3; // 45% chance no hline

        // amount of wiggling
        int sp = rng::rand(1, 5);
        int length = rng::rand(2, 6);
        int ampl = rng::rand(0, 10);
        if (ampl > 5) ampl = 0;

        if (_face_cur.op < 0.1f)
            {
            // the face is not showing
            _face_start = { 0.0f, q, (float)sp, (float)ampl, (float)length }; // hline          
            //_face_start = { 0.0f, 7, 0.0f, 0.0f, 30.0f }; // tres hline
            //_face_start = { 0.0f, 3, 1.0f, 2.0f, 5.0f };  // smooth avec legere ondulation
            //_face_start = { 0.0f, 6, 1.0f, 2.0f, 30.0f }; // hline + legere ondulation (idem)
            //_face_start = { 0.0f, 3, 0.0f, 0.0f, 30.0f }; // super smooth
            }
        else
            {
            _face_start = _face_cur;
            }

        _face_end = state_visible_good;
        int a = rng::rand(10);
        if (a > 6) a = 0; // 50% chance no hline
        _face_end.BITQUANT = a;

        _face_state = 5; // appearing 
        _face_substate = 0;
        _face_len = ms;
        _face_multscale = 1.0f;
        _face_em = 0;
        }


    SLOWFUN void cubeAppear(tgx::Image<tgx::RGB565> & im, int ms)
        {
        useImageInsteadOfFace(im);
        if (ms <= 0)
            {
            ms = rng::rand(100, 2000);
            }

        // amount of hline
        int q = rng::rand(9);
        if (q > 7) q = 7; // 22% chance strong hline
        if (q < 3) q = 3; // 45% chance no hline

        // amount of wiggling
        int sp = rng::rand(1, 5);
        int length = rng::rand(7, 30);
        int ampl = rng::rand(0, 10);
        if (ampl > length) ampl = length;

        if (_face_cur.op < 0.1f)
            {  // the face is not showing
            _face_start = { 0.0f, q, (float)sp, (float)ampl, (float)length }; // hline          
            } 
        else
            {
            _face_start = _face_cur;
            }
        _face_end = state_visible_cube;
        _face_state = 7; // appearing 
        _face_substate = 0;
        _face_len = ms;
        _face_multscale = 1.0f;
        _face_em = 0;
        }


    SLOWFUN void dreamingAppear(tgx::Image<tgx::RGB565>& im, int ms)
        {
        useImageInsteadOfFace(im);
        if (ms <= 0)
            {
            ms = rng::rand(1000, 3000);
            }

        // amount of wiggling
        float sp = rng::rand(60, 120)/100.0f;
        float length = rng::rand(30, 100);
        float ampl = length / 3;
        if (_face_cur.op < 0.1f)
            {  // the face is not showing
            _face_start = { 0.0f, 3, sp, ampl, length };
            } 
        else
            {
            _face_start = _face_cur;
            }
        _face_end = state_visible_dreaming;
        _face_state = 11; // appearing dreaming
        _face_substate = 0;
        _face_len = ms;
        _face_multscale = 1.0f;
        _face_em = 0;
        }


    SLOWFUN void dreamingEnd(tgx::Image<tgx::RGB565>& im, int ms)
        {
        useImageInsteadOfFace(im);
        if (ms <= 0)
            {
            ms = rng::rand(1000, 3000);
            }

        _face_end = state_visible_dreaming_end;
        _face_state = 13; 
        _face_substate = 0;
        _face_len = ms;
        _face_multscale = 1.0f;
        _face_em = 0;
        }


    



    SLOWFUN void cubeAppearSolved(tgx::Image<tgx::RGB565>& im, int ms)
        {
        useImageInsteadOfFace(im);
        if (ms <= 0)
            {
            ms = rng::rand(1000, 2000);
            }
        _face_end = state_visible_cube_solved;
        _face_state = 10; // appearing 
        _face_substate = 0;
        _face_len = ms;
        _face_multscale = 1.0f;
        _face_em = 0;
        }


    SLOWFUN void cubeAppearReplay(tgx::Image<tgx::RGB565>& im, int ms)
        {
        useImageInsteadOfFace(im);
        if (ms <= 0)
            {
            ms = rng::rand(1000, 2000);
            }
        _face_start = state_invisible; // { 0.0f, 3, 3, 5, 5 };
        _face_end = state_visible_cube_replay;
        _face_state = 12; // appearing 
        _face_substate = 0;
        _face_len = ms;
        _face_multscale = 1.0f;
        _face_em = 0;
        }






    SLOWFUN void disapear(int ms)
        { // op, quant, speed, ampl, lenght
         
        if (ms <= 0)
            {
            ms = rng::rand(1500, 3500); 
            }

        // amount of hline
        int q = rng::rand(12);
        if (q > 7) q = 7;
        if (q < 3) q = 3;

        // amount of wiggling
        int sp = rng::rand(1, 5);
        int length = rng::rand(2,6);
        int ampl = rng::rand(0, 10);
        if (ampl > 5) ampl = 0; 

        _face_end = { 0.0f, q, (float)sp, (float)ampl, (float)length }; // hline          
        //_face_end = { 0.0f, 7, 0.0f, 0.0f, 0.0f }; // hline 
        //_face_end = { 0.0f, 3, 5.0f, 5.0f, 5.0f }; // wigling
        //_face_end = { 0.0f, 3, 0.0f, 0.0f, 0.0f }; // smooth disapearance

        _face_start = _face_cur;
        _face_state = 6;
        _face_substate = 0;
        _face_len = ms;
        _face_multscale = 1.0f;
        _face_em = 0;
        }



    SLOWFUN void cubeDisappear(int ms)
        {
        if (ms <= 0)
            {
            ms = rng::rand(1500, 3500);
            }

        // amount of hline
        int q = rng::rand(12);
        if (q > 7) q = 7;
        if (q < 3) q = 3;

        // amount of wiggling
        int sp = rng::rand(1, 5);
        int length = rng::rand(7, 30);
        int ampl = rng::rand(0, 10);
        if (ampl > length) ampl = length;

        _face_end = { 0.0f, q, (float)sp, (float)ampl, (float)length }; // hline          

        _face_start = _face_cur;
        _face_state = 8;
        _face_substate = 0;
        _face_len = ms;
        _face_multscale = 1.0f;
        _face_em = 0;
        }



    SLOWFUN void dreamingDisappear(int ms)
        {
        if (ms <= 0)
            {
            ms = rng::rand(1500, 3500);
            }

        // amount of wiggling
        const float sp = 0.1f;
        const float length = 5;
        const float ampl = 2;
        _face_end = { 0.0f, 3, sp, ampl, length}; // hline          

        _face_start = _face_cur;
        _face_state = 8;
        _face_substate = 0;
        _face_len = ms;
        _face_multscale = 1.0f;
        _face_em = 0;
        }




    SLOWFUN void disapear_shutdown(int ms)
        { // op, quant, speed, ampl, lenght
        _face_end = { 0.0f, 3, 0, 0, 0 }; // hline          
        _face_start = _face_cur;
        _face_state = 6;
        _face_substate = 0;
        _face_len = ms;
        _face_multscale = 1.0f;
        _face_em = 0;
        }


    float currentFaceOpacity()
        {
        return _face_cur.op;
        }


    bool currentFaceVisible()
        {
        return (_face_cur.op > 0.01f);
        }


    /**
    * Make the face keep its eyes shut for ms milliseconds.
    **/
    SLOWFUN void shuteyes(uint32_t ms)
        {
        shuteyes_len = ms;
        shuteyes_em = 0;
        }






    static int _lastFaceAction = 1; // 0 = disappear, 1 = appear, 2 = disabled
    static int _delayFaceDisapear = -1;
    elapsedMillis _emFaceDisapear;

    SLOWFUN void audioReact(audio_block_t* block)
        {
        // for LypSync and for face animation.         
        if (MAX98357A::isSpeaking())
            {
            if (_lastFaceAction == 0)
                {
                faceAction::appear();
                _lastFaceAction = 1;
                }
            if (block)
                {
                LipSync::lipsync_isr_process_block(block->data, AUDIO_BLOCK_SAMPLES);
                } else
                {
                LipSync::lipsync_isr_process_block(nullptr, 0);
                }
            } 
        else
            {
            LipSync::lipsync_isr_process_block(nullptr, 0);
            if (_lastFaceAction == 1)
                {
                if (_delayFaceDisapear < 0)
                    {
                    _delayFaceDisapear = rng::rand(500, 1500); // delay before disappearing
                    _emFaceDisapear = 0;
                    } 
                else
                    {
                    if (_emFaceDisapear >= (uint32_t)_delayFaceDisapear)
                        {
                        faceAction::disapear();
                        _lastFaceAction = 0;
                        _delayFaceDisapear = -1;
                        }
                    }
                }
            }
        }


    SLOWFUN void audioReacEnable(bool enable)
        {
        if (!enable)
            {
            _lastFaceAction = 2;
            return;
            }
        if (_face_cur.op < 0.01)
            { // the face is not showing. 
            _lastFaceAction = 0;
            }
        else
            { // the face is showing
            _lastFaceAction = 1;
            }
        }




    }


/** end of file */

