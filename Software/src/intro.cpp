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



#include "config.h"
#include "Hardware.h"
#include "display.h"
#include "sarcasm_bk.h" // background image 
#include "intro_face.h" // subliminal face image
#include "intro_rubik.h"// rubik cube image
#include "audioMAX98357A.h"
#include "LedStrip.h"
#include "xorshift32.h"
#include "UI.h"

#include <math.h>
#include <Arduino.h>
#include <tgx.h>
#include <TeensyThreads.h>

#include "FBSensors.h"


#define LX 320
#define LY 240


#define SARCASM_BK_OPACITY 0.4f
#define ROTATION_TURNS 1
#define WAVE_SPEED 2.5
#define WAVE_AMPLITUDE 20
#define WAVE_FREQ (1.0f / 40.0f)

#define SUBLIMINAL_THRESHOLD_HIGH 120
#define SUBLIMINAL_THRESHOLD_LOW 50
#define DURATION_INTRO_MS 3800
#define PART2_RATIO 0.65f
#define STATIC_NOISE_FADE_SPEED  3
#define STATIC_NOISE_MIN 0

#define DURATION_SHOWCMD_PART1_MS 300
#define DURATION_SHOWCMD_PART2_MS 300
#define DURATION_SHOWCMD_PART3_MS 300



SLOWFUN static void updateLedColor(int ms, float brightnessFactor = 0.15f) {
    auto smoothstep = [](float edge0, float edge1, float x) -> float {
        x = constrain((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
        return x * x * (3 - 2 * x);
        };

    auto randomByte = []() -> uint8_t {
        return ((rng::rand()) >> 24);
        };

    auto lerp = [](float a, float b, float t) -> float {
        return a + (b - a) * t;
        };

    const int mid = LED_COUNT / 2;

    // PHASE 1 : 0 - 1900 ms - erratic random flashing
    if (ms < 1900) {
        float progress = smoothstep(300, 1900, ms);
        for (int i = 0; i < LED_COUNT; ++i) {
            if ((rng::rand(100)) < (5 + progress * 40)) {
                float fade = (rng::rand(100)) / 100.0f;
                uint8_t choice = randomByte() % 4;
                uint8_t r = 0, g = 0, b = 0;
                switch (choice) {
                    case 0: r = 255; break;
                    case 1: r = 255; g = 64; break;
                    case 2: r = 255; g = 200; break;
                    case 3: r = g = b = 220; break;
                    }
                r = (uint8_t)(r * fade * brightnessFactor);
                g = (uint8_t)(g * fade * brightnessFactor);
                b = (uint8_t)(b * fade * brightnessFactor);
                LedStrip::setLed(i, r, g, b, 0);
                } else {
                LedStrip::setLed(i, 0, 0, 0, 0);
                }
            }
        }

    // PHASE 2 : 1900 - 2500 ms - warm wave transition
    else if (ms < 2500) {
        float t = smoothstep(1900, 2500, ms);
        float phase = fmod(ms * 0.003f, 1.0f);
        for (int i = 0; i < LED_COUNT; ++i) {
            float x = (float)i / (LED_COUNT - 1);
            float wave = sinf(2 * 3.14159f * (x - phase)) * 0.5f + 0.5f;
            uint8_t r = (uint8_t)(lerp(255, 80, wave) * t * brightnessFactor);
            uint8_t g = (uint8_t)(lerp(0, 64, wave) * t * brightnessFactor);
            uint8_t b = 0;
            LedStrip::setLed(i, r, g, b, 0);
            }
        }

    // PHASE 3 : 2500 - 3700 ms - tight wave, only one or two LEDs lit
    else if (ms < 3700) {
        float t = smoothstep(2500, 3700, ms);
        float progress = (float)(ms - 2500) / (3700 - 2500);
        float head1 = progress * (LED_COUNT - 1);
        float head2 = (1.0f - progress) * (LED_COUNT - 1);

        for (int i = 0; i < LED_COUNT; ++i) {
            float intensity = 0.0f;
            float dist1 = fabs(i - head1);
            if (dist1 <= 1.5f) {
                intensity += 1.0f - dist1 / 1.5f;
                }
            float dist2 = fabs(i - head2);
            if (dist2 <= 1.5f) {
                intensity += 1.0f - dist2 / 1.5f;
                }
            intensity = constrain(intensity, 0.0f, 1.0f) * t;

            if (intensity > 0.01f) {
                float posRatio = (float)i / (LED_COUNT - 1);
                uint8_t r = (uint8_t)(255 * (1.0f - posRatio) * intensity * brightnessFactor);
                uint8_t g = (uint8_t)(64 * posRatio * intensity * brightnessFactor);
                uint8_t b = (uint8_t)(255 * posRatio * intensity * brightnessFactor);
                LedStrip::setLed(i, r, g, b, 0);
                } else {
                LedStrip::setLed(i, 0, 0, 0, 0);
                }
            }
        }

    // PHASE 4 : 3700 - 4500 ms - pulsating centered blue
    else if (ms < 4500) {
        float t = smoothstep(3700, 4500, ms);
        float pulse = sinf(ms * 0.006f) * 0.5f + 0.5f;
        for (int i = 0; i < LED_COUNT; ++i) {
            float dist = fabs((float)(i - mid)) / mid;
            float attenuation = 1.0f - dist * 0.6f;
            float intensity = pulse * attenuation * t * brightnessFactor;
            uint8_t r = 0;
            uint8_t g = (uint8_t)(160 * intensity);
            uint8_t b = (uint8_t)(255 * intensity);
            LedStrip::setLed(i, r, g, b, 0);
            }
        }

    // PHASE 5 : 4500 - 5100 ms - fade out from center to edges
    else if (ms < 5000) {
        float t = smoothstep(4500, 5000, ms);
        for (int i = 0; i < LED_COUNT; ++i) {
            float dist = fabs((float)(i - mid)) / mid;
            float fade = 1.0f - smoothstep(0.0f, 1.0f, dist * t);
            uint8_t r = 0;
            uint8_t g = (uint8_t)(100 * fade * brightnessFactor);
            uint8_t b = (uint8_t)(200 * fade * brightnessFactor);
            LedStrip::setLed(i, r, g, b, 0);
            }
        }

    // PHASE 6 : 5100 - 5300 ms - full black (silent pause)
    else if (ms < 5200) {
        for (int i = 0; i < LED_COUNT; ++i) {
            LedStrip::setLed(i, 0, 0, 0, 0);
            }
        }

    /*
    // PHASE 7 : 5300 - 5600 ms - symmetric bottom-up activation
    else if (ms < 5600) {
        float t = smoothstep(5200, 5600, ms);
        for (int i = 0; i < LED_COUNT; ++i) {
            int symIndex = (i <= mid) ? i : (LED_COUNT - 1 - i);
            float threshold = (float)symIndex / mid;
            float ledProgress = constrain((t - threshold) * 3.0f, 0.0f, 1.0f);
            uint8_t r = (uint8_t)(65 * ledProgress * 0.3f * brightnessFactor);
            uint8_t g = (uint8_t)(135 * ledProgress * 0.3f * brightnessFactor);
            uint8_t b = (uint8_t)(113 * ledProgress * 0.3f * brightnessFactor);
            LedStrip::setLed(i, r, g, b, 0);
            }
        }
    */

    // PHASE 8 : after
    else {
        // You can place idle animation here if needed
        }
    }


// copy src to dst with a wave deformation
SLOWFUN static void copyWave(uint16_t* dest, const uint16_t* src)
    {
    static int phase = 0;
    phase += WAVE_SPEED;
    int o = 0;
    for (int j = 1; j < LY - 1; j++) // avoid j=0 and j = LY-1 to prevent overflow. 
        {
        memcpy(dest + LX * j, src + LX * j + o, (LX * 2));
        o = sin((j + phase) * WAVE_FREQ) * WAVE_AMPLITUDE;
        }
    }



/**
* Played in a teensy thread. Uses around 1500 bytes of stack.
**/
SLOWFUN void playIntro()
    {
    
    MAX98357A::playMusic("intro_full.wav");
    Display::backlight(true);
    tgx::Image<tgx::RGB565> im = Display::image_main;

    tgx::Image<tgx::RGB565> im_rubik = Display::image_zbuffer;    
    im_rubik.blit(intro_rubik, { 0,0 }); // copy the rubik cube image into the zbuffer in DMAMEM for speedup.

    tgx::Image<tgx::RGB565> im_face = Display::image_alt;
    im_face.blit(intro_face, { 0,0 }); // copy the subliminal face image into the alternate framebuffer in DMAMEM for speedup.
   
    elapsedMillis emled = 0;
    elapsedMillis em = 0;

    bool cont = true;
    while (cont)
        {
        threads.yield();
        updateLedColor((int)(emled));
        LedStrip::show();
        float r = ((float)((int)em))/DURATION_INTRO_MS;
        float u = (r - PART2_RATIO) / (1.0f - PART2_RATIO);
        if (r >= 1.0f) { r = 1.0f; cont = false; }

        // draw the wave deformed rubik cube image with the subliminal face image on top
        im.clear(tgx::RGB565_Black);
        copyWave((uint16_t*)im.data(), (uint16_t*)im_rubik.data());
        int a = (rng::rand()) & 0xFF;
        if (a > SUBLIMINAL_THRESHOLD_HIGH) a = SUBLIMINAL_THRESHOLD_HIGH; // minimum alpha
        if (a > SUBLIMINAL_THRESHOLD_LOW) { im.blit(im_face, { 0,0 }, a / 255.0f); }

        // add static noise
        uint32_t noiselvl = (uint32_t)(255*(1.0f - r*STATIC_NOISE_FADE_SPEED));
        if (noiselvl < STATIC_NOISE_MIN) noiselvl = STATIC_NOISE_MIN;
        for (int j = 0; j < LY; j++)
            {
            for (int i = 0; i < LX; i++)
                {
                uint32_t a = rng::rand();
                if ((a & 0xFF) < noiselvl)
                    {
                    uint16_t col = (uint16_t)((a >> 8) & 0xFFFF);
                    im(i, j) = (tgx::RGB565)col;
                    }
                if (u > 0)
                    { // remove intro image as we progress in the second part
                    if ((rng::rand() & 0xFF) < r * 256) im(i, j) = tgx::RGB565_Black;
                    }
                }
            }

        // draw the console image inceasing in size in art 2
        if (u > 0)
            {
            im.blitScaledRotated(sarcasm_bk, { 160,120 }, { 160,120 }, u, ROTATION_TURNS*(1 - u)*(1-u) * 360,1.0f - r);
            }

        updateLedColor((int)(emled));
        LedStrip::show();
        Display::update(); // update the display with the current image
        }
    //im.blit(sarcasm_bk, { 0,0 }, 0.0f);
    //Display::update(); // update the display with the final image
    updateLedColor((int)(emled));
    LedStrip::show();

    UI::drawMainUI(im_face);
    im.blit(im_face, { 0,0 });

    UI::drawUIactivity(im, { -1, -1 } ); // clear activity
    UI::drawUIvoltage(im, FBSensors::getVoltage(), FBSensors::getChargingCurrent());

    updateLedColor((int)(emled));
    LedStrip::show();

    em = 0;
    while (em < DURATION_SHOWCMD_PART1_MS)
        {
        int a = (int)(em*LX/(2*DURATION_SHOWCMD_PART1_MS));
        Display::updateRegion({ max(LX/2 - a, 0),min(LX/2 + a, 319), 0, 18}, true);
        updateLedColor((int)(emled));
        LedStrip::show();
        }
    Display::updateRegion({ 0,319, 0, 18 }, false);
    updateLedColor((int)(emled));
    LedStrip::show();

    em = 0; 
    while (em < DURATION_SHOWCMD_PART2_MS)
        {
        int a = (int)(em * LX / (2 * DURATION_SHOWCMD_PART1_MS));
        Display::updateRegion({ max(LX / 2 - a, 0),min(LX / 2 + a, 319), 16, 24 }, false);
        Display::updateRegion({ max(LX / 2 - a, 0),min(LX / 2 + a, 319), 233, 239 }, true);
        updateLedColor((int)(emled));
        LedStrip::show();
        }

    em = 0;
    while (em < DURATION_SHOWCMD_PART3_MS)
        {
        int a = (int)(em * 110 / (DURATION_SHOWCMD_PART1_MS));
        Display::updateRegion({0,6, max(130 - a, 0),min(130 + a, 240) }, false);
        Display::updateRegion({313, 319, max(130 - a, 0),min(130 + a, 239) }, true);
        updateLedColor((int)(emled));
        LedStrip::show();
        }

    Display::update(); 
    while ((int)emled < 5200)
        {
        updateLedColor((int)(emled));
        LedStrip::show();
        }

    }


/** end of file */