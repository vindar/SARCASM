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
#include "config.h"
#include "hardware.h"
#include "debug.h"
#include "LedStrip.h"
#include "AudioLed.h"
#include <Arduino.h>
#include <Audio.h>
#include <tgx.h>
#include "LipSync.h"
#include "audioMAX98357A.h"
#include "faceAction.h"


namespace MAX98357A
    {

    #define ACTIVITY_LEVEL_SCALER (12500/4)

    static volatile int _minv_audio_block = 0; 
    static volatile int _maxv_audio_block = 0;

    SLOWFUN tgx::iVec2 activityLevel()
        {
        //Serial.printf("act: %d , %d\n", _minv_audio_block, _maxv_audio_block);
        if (_minv_audio_block == 0 && _maxv_audio_block == 0) return tgx::iVec2(-1, -1);
        int ma = 4 + _minv_audio_block / ACTIVITY_LEVEL_SCALER;
        int mb = 4 + _maxv_audio_block / ACTIVITY_LEVEL_SCALER;
        return tgx::iVec2(ma, mb);
        }



    class AudioLedMusic
        {
        public: 

        float brightness = 0.25f;     // overall brightness factor
        float decaySpeed = 0.95f;     // trail persistence
        float colorSpeed = 0.002f;    // hue rotation speed
        float waveFrequency = 4.0f;   // spatial wave frequency
        float flashThreshold = 0.6f;  // peak value to trigger flash
            
        AudioLedMusic() : _em(0), level(0.0f), frameCounter(0) { memset(trail, 0, sizeof(trail)); }

        void create(int32_t peak, tgx::RGB24* leds, float opacity);

        private:

        void hsvToRgb(float h, float s, float v, uint8_t& r, uint8_t& g, uint8_t& b);

        elapsedMicros _em;
        float level;
        uint32_t frameCounter;
        float trail[LED_COUNT];
        };


    SLOWFUN void AudioLedMusic::create(int32_t peak, tgx::RGB24* leds, float opacity)
        {
        //const int us = (int)_em;
        _em = 0;
        if (peak<0) 
            {
            level *= decaySpeed;
            } 
        else 
            {
            float newLevel = 1.2f * peak / 32768.0f;
            level = level * 0.7f + newLevel * 0.3f;
            }

        frameCounter++;

        for (int i = 0; i < LED_COUNT; ++i) 
            {
            float pos = (float)i / (LED_COUNT - 1);
            float wave = sinf(2.0f * 3.14159f * (waveFrequency * pos - frameCounter * 0.01f)) * 0.5f + 0.5f;

            trail[i] *= decaySpeed;
            if (trail[i] < level) trail[i] = level;

            float intensity = trail[i] * wave;

            float hue = fmod(frameCounter * colorSpeed * 360.0f + i * 12.0f, 360.0f);
            float saturation = 1.0f;
            float value = intensity;

            // Bonus flash color on strong peaks
            if ((level > flashThreshold) && ((i % 5) == ((int)(frameCounter % 5)))) 
                {
                hue = fmod(hue + 180.0f, 360.0f);
                value = 1.0f;
                }

            uint8_t r, g, b;
            hsvToRgb(hue, saturation, value, r, g, b);
            if (opacity < 1.0f)
                {
                leds[i].blend(tgx::RGB24((int)(r * brightness), (int)(g * brightness), (int)(b * brightness)), opacity);
                }
            else
                {
                leds[i] = tgx::RGB24((int)(r * brightness), (int)(g * brightness), (int)(b * brightness));
                }
            }
        }


    SLOWFUN void AudioLedMusic::hsvToRgb(float h, float s, float v, uint8_t& r, uint8_t& g, uint8_t& b)
        {
        int i = int(h / 60.0f) % 6;
        float f = (h / 60.0f) - i;
        float p = v * (1.0f - s);
        float q = v * (1.0f - f * s);
        float t = v * (1.0f - (1.0f - f) * s);
        float rf, gf, bf;
        switch (i) {
            case 0: rf = v; gf = t; bf = p; break;
            case 1: rf = q; gf = v; bf = p; break;
            case 2: rf = p; gf = v; bf = t; break;
            case 3: rf = p; gf = q; bf = v; break;
            case 4: rf = t; gf = p; bf = v; break;
            default: rf = v; gf = p; bf = q; break;
            }
        r = (uint8_t)(rf * 255.0f);
        g = (uint8_t)(gf * 255.0f);
        b = (uint8_t)(bf * 255.0f);
        }





    /*

    class AudioLedVoice
        {

        public:


        float voiceBrightnessFactor = 0.15f;
        float voiceBoostFactor = 2.7f;             // Boost pendant la voix
        float idleBreathPeriod = 4.0f;             // Durée d'un cycle de respiration (en secondes)
        float idleBreathAmplitude = 0.9f;          // Amplitude de la respiration (0.0 - 1.0)


        AudioLedVoice() : _em(0), voiceLevel(0.0f), idleBlend(1.0f), idleTime(0.0f) {}

        void create(int32_t peak, tgx::RGB24* leds, float opacity);


        private:

        elapsedMicros _em;
        float voiceLevel;
        float idleBlend;
        float idleTime;
        };




    SLOWFUN void AudioLedVoice::create(int32_t peak, tgx::RGB24* leds, float opacity)
        {
        const int us = (int)_em;
        _em = 0;
        idleTime += us / 1000000.0f;

        bool voiceDetected = false;

        if (peak < 0) 
            {
            voiceLevel *= 0.90f;
            } 
        else 
            {
            voiceDetected = true;
            float newLevel = 1.0f * peak / 32768.0f;
            if (newLevel > 1.0f) newLevel = 1.0f;
            voiceLevel = voiceLevel * 0.7f + newLevel * 0.3f;            
            }

        if (voiceDetected) 
            {
            idleBlend = 0.0f;
            idleTime = 0.0f;
            } else {
            float delta = us / 800000.0f;
            idleBlend = constrain(idleBlend + delta, 0.0f, 1.0f);
            }

        const float baseR = 65, baseG = 135, baseB = 113;
        const float activeR = 0, activeG = 255, activeB = 100;

        float breathFactor = 1.0f;
        if (!voiceDetected && idleBlend >= 1.0f) 
            {
            float phase = fmod(idleTime, idleBreathPeriod) / idleBreathPeriod;
            float s = sinf(phase * 2.0f * 3.14159f) * 0.5f + 0.5f;
            breathFactor = 1.0f - idleBreathAmplitude + idleBreathAmplitude * s;
            }

        const int mid = LED_COUNT / 2;
        float yTarget = voiceLevel;

        for (int i = 0; i < LED_COUNT; ++i) {
            int sym = (i <= mid) ? i : (LED_COUNT - 1 - i);
            float y = (float)sym / mid;
            float dist = fabs(y - yTarget);

            float window = 0.12f;
            float intensity = constrain(1.0f - dist / window, 0.0f, 1.0f);
            intensity = intensity * intensity * (3 - 2 * intensity);
            intensity *= voiceLevel;

            float r, g, b;

            if (intensity > 0.01f) 
                {
                float cR = baseR * (1 - y) + activeR * y;
                float cG = baseG * (1 - y) + activeG * y;
                float cB = baseB * (1 - y) + activeB * y;
                r = cR * intensity * voiceBoostFactor;
                g = cG * intensity * voiceBoostFactor;
                b = cB * intensity * voiceBoostFactor;
                } 
            else 
                {
                float threshold = y;
                float blend = constrain((idleBlend - threshold) * 4.0f, 0.0f, 1.0f);
                r = (baseR * blend) * (breathFactor * 0.8f);
                g = (baseG * blend) * (breathFactor * 0.8f);
                b = (baseB * blend) * (breathFactor * 0.8f);
                }

            r = min(r, 255.0f);
            g = min(g, 255.0f);
            b = min(b, 255.0f);

            if (opacity < 1.0f) 
                {
                leds[i].blend(tgx::RGB24((int)(r * voiceBrightnessFactor), (int)(g * voiceBrightnessFactor), (int)(b * voiceBrightnessFactor)), opacity);
                } 
            else 
                {
                leds[i] = tgx::RGB24((int)(r * voiceBrightnessFactor), (int)(g * voiceBrightnessFactor), (int)(b * voiceBrightnessFactor));
                }
            }
        }


        */



class AudioLedVoice {
public:
    float voiceBrightnessFactor = 0.15f;  // garder assez haut pour éviter les paliers
    float voiceBoostFactor = 2.7f;

    float idleBreathPeriod = 4.0f;
    float idleBreathAmplitude = 0.9f;   // un peu plus doux
    float idleBreathGamma = 1.4f;   // courbe moins “plate” en haut

    // Teinte / gain idle (diminue l énergie sans écraser la dynamique 8 bits)
    float idleGlobalGain = 0.35f;  // remplace idleBrightnessFactor très bas
    

    float idleTintR = DEFAULT_VOICE_TINT_R;
    float idleTintG = DEFAULT_VOICE_TINT_G;
    float idleTintB = DEFAULT_VOICE_TINT_B;
    uint32_t tint_period = 0;


    AudioLedVoice() : _em(0), voiceLevel(0.0f), idleBlend(1.0f), idleTime(0.0f), _frame(0)
        {
        setVoiceLedTint(DEFAULT_VOICE_TINT_R, DEFAULT_VOICE_TINT_G, DEFAULT_VOICE_TINT_B);
        }

    SLOWFUN void create(int32_t peak, tgx::RGB24* leds, float opacity);



    SLOWFUN void setVoiceLedTint(float R, float G, float B)
        {
        idleTintR = R;
        idleTintG = G;
        idleTintB = B;
        tint_period = 0;
        }

    SLOWFUN void setVoiceLedTintAuto(uint32_t period)
        {
        tint_period = period;
        }


    SLOWFUN tgx::RGBf getCurrentTint()
        {
        return tgx::RGBf(idleTintR, idleTintG, idleTintB);
        }


private:

    elapsedMicros _em;
    float voiceLevel;
    float idleBlend;
    float idleTime;

    // Compteur pour le dithering temporel (pas besoin de RNG)
    uint16_t _frame;

    // Quantification avec dithering temporel (16 sous-niveaux)
    inline uint8_t quantizeWithDither(float v255, int ledIndex, float brightnessFactor) {
        if (v255 <= 0.0f) return 0;
        if (v255 > 255.0f) v255 = 255.0f;

        float scaled = v255 * brightnessFactor; // 0..255
        int   base = (int)scaled;             // plancher
        float frac = scaled - base;           // [0..1)

        // Seuil pseudo-aléatoire déterministe dépendant de l index LED et du frame
        // distribue la fraction dans le temps sans scintillement visible.
        uint8_t threshold = (uint8_t)((ledIndex * 13 + _frame * 7) & 15); // 0..15
        int bump = ((frac * 16.0f) > threshold) ? 1 : 0;

        int q = base + bump;
        if (q > 255) q = 255;
        return (uint8_t)q;
        }
    };



tgx::RGBf hsvToRgb(float h, float s, float v)
    {
    int i = int(h / 60.0f) % 6;
    float f = (h / 60.0f) - i;
    float p = v * (1.0f - s);
    float q = v * (1.0f - f * s);
    float t = v * (1.0f - (1.0f - f) * s);
    float rf, gf, bf;
    switch (i) {
        case 0: rf = v; gf = t; bf = p; break;
        case 1: rf = q; gf = v; bf = p; break;
        case 2: rf = p; gf = v; bf = t; break;
        case 3: rf = p; gf = q; bf = v; break;
        case 4: rf = t; gf = p; bf = v; break;
        default: rf = v; gf = p; bf = q; break;
        }
    return tgx::RGBf(rf, gf, bf);
    }



SLOWFUN void AudioLedVoice::create(int32_t peak, tgx::RGB24* leds, float opacity) {
    const int us = (int)_em;
    _em = 0;
    idleTime += us / 1000000.0f;
    _frame++; // pour le dithering


    if (tint_period > 0)
        { // recompute tint
        float ff = 360*(((float)(millis() % tint_period)) / ((float)tint_period));
        tgx::RGBf col = hsvToRgb(ff, 1.0f, 1.0f);
        idleTintR = col.R;
        idleTintG = col.G;
        idleTintB = col.B;
        }

    bool voiceDetected = false;

    if (peak < 0) {
        voiceLevel *= 0.95f;
        } else {
        voiceDetected = true;
        float newLevel = 1.0f * peak / 32768.0f;
        if (newLevel > 1.0f) newLevel = 1.0f;
        voiceLevel = voiceLevel * 0.7f + newLevel * 0.3f;
        }

    if (voiceDetected) {
        idleBlend = 0.0f;
        idleTime = 0.0f;
        } else {
        float delta = us / 800000.0f;
        idleBlend = constrain(idleBlend + delta, 0.0f, 1.0f);
        }

    float baseR = 65, baseG = 135, baseB = 113;
    float activeR = 0, activeG = 255, activeB = 100;
    if (tint_period > 0)
        {
        baseR = 200 * idleTintR;
        baseG = 200 * idleTintG;
        baseB = 200 * idleTintB;
        activeR = 255;
        activeG = 255;
        activeB = 255;
        }



    float breathFactor = 1.0f;
    if (!voiceDetected && idleBlend >= 1.0f) {
        float phase = fmod(idleTime, idleBreathPeriod) / idleBreathPeriod;
        float s = sinf(phase * 2.0f * 3.14159f) * 0.5f + 0.5f; // [0..1]
        float bf = 1.0f - idleBreathAmplitude + idleBreathAmplitude * s; // [1-A..1]
        breathFactor = powf(bf, idleBreathGamma);
        }

    // brightness global commun (évite la réduction à 16 niveaux)
    const float brightnessFactor = voiceBrightnessFactor;

    const int mid = LED_COUNT / 2;
    float yTarget = voiceLevel;

    for (int i = 0; i < LED_COUNT; ++i) {
        int sym = (i <= mid) ? i : (LED_COUNT - 1 - i);
        float y = (float)sym / mid;
        float dist = fabs(y - yTarget);

        float window = 0.12f;
        float intensity = constrain(1.0f - dist / window, 0.0f, 1.0f);
        intensity = intensity * intensity * (3 - 2 * intensity);
        intensity *= voiceLevel;

        float r, g, b;

        if (intensity > 0.01f) {
            // bande active voix
            float cR = baseR * (1 - y) + activeR * y;
            float cG = baseG * (1 - y) + activeG * y;
            float cB = baseB * (1 - y) + activeB * y;
            r = cR * intensity * voiceBoostFactor;
            g = cG * intensity * voiceBoostFactor;
            b = cB * intensity * voiceBoostFactor;
            } else {
            // fond idle — fondu + teinte + gain
            float threshold = y;
            float blend = constrain((idleBlend - threshold) * 4.0f, 0.0f, 1.0f);
            // smoothstep pour éviter des marches pendant le retour à l idle
            blend = blend * blend * (3 - 2 * blend);

            r = baseR * blend * breathFactor * idleTintR * idleGlobalGain;
            g = baseG * blend * breathFactor * idleTintG * idleGlobalGain;
            b = baseB * blend * breathFactor * idleTintB * idleGlobalGain;
            }

        // Quantification + dithering (limite les paliers de bas niveau)
        const uint8_t R = quantizeWithDither(r, i, brightnessFactor);
        const uint8_t G = quantizeWithDither(g, i + 17, brightnessFactor); // décalage pour éviter corrélations
        const uint8_t B = quantizeWithDither(b, i + 33, brightnessFactor);

        const tgx::RGB24 out(R, G, B);

        if (opacity < 1.0f) {
            leds[i].blend(out, opacity);
            } else {
            leds[i] = out;
            }
        }
    }

























    static AudioLedMusic audioLedMusic; // local instance for music LED effect
    static AudioLedVoice audioLedVoice; // local instance for voice LED effect


    SLOWFUN void AudioLed::setDriver(int driver)
        {
        noInterrupts();
        _drivertype = driver;
        interrupts();
        }


    SLOWFUN void AudioLed::setVoiceLedTint(float R, float G, float B)
        {
        audioLedVoice.setVoiceLedTint(R, G, B);
        }


    SLOWFUN void AudioLed::setVoiceLedTintAuto(uint32_t period)
        {
        audioLedVoice.setVoiceLedTintAuto(period);
        }


    SLOWFUN tgx::RGBf AudioLed::getCurrentTint()
        {
        return audioLedVoice.getCurrentTint();
        }




    SLOWFUN void AudioLed::update()
        {
        audio_block_t* block = receiveReadOnly();

        faceAction::audioReact(block);

        int32_t peak = 0;
        if (!block)
            {
            _minv_audio_block = 0;
            //_maxv_audio_block = 0;
            peak = -1;
            }
        else
            {
            int minv = 100000; 
            int maxv = -100000; // min and max values in the block
            for (int i = 0; i < AUDIO_BLOCK_SAMPLES; ++i)
                {
                if (block->data[i] < minv) minv = block->data[i]; // find the minimum value
                if (block->data[i] > maxv) maxv = block->data[i]; // find the maximum value
                int32_t s = abs(block->data[i]);
                if (s > peak) peak = s;
                }
            _minv_audio_block = minv; // store the minimum value
            _maxv_audio_block = maxv; // store the maximum value
            release(block);
            }

        if (_drivertype == DRIVER_NONE)
            {// no driver selected
            _minv_audio_block = 0;
            _maxv_audio_block = 0;
            return;
            }

        tgx::RGB24 leds[LED_COUNT]; // array to hold the LED colors
        memset(leds, 0, sizeof(leds)); // clear the led colors

        float opacity = 1.0f; // default opacity
        if (_drivertype == DRIVER_VOICE)
            {
            if (peak >= 0) peak = (int32_t)(peak * AUDIOLED_VOICE_BRIGHTNESS);
            audioLedVoice.create(peak, leds, opacity);
            }
        else
            {
            if (peak >= 0) peak = (int32_t)(peak * AUDIOLED_MUSIC_BRIGHTNESS);
            audioLedMusic.create(peak, leds, opacity);
            }

        for (int i = 0; i < LED_COUNT; ++i)
            {
            LedStrip::setLedAudio(i, leds[i].R, leds[i].G, leds[i].B, 0);
            }
        LedStrip::showAudio();
        }


    AudioLed audioLed; // global instance for audio LED effect

    }


/** end of file */


 
  

   







