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

#include "audioMAX98357A.h"
#include "config.h"
#include "hardware.h"
#include "debug.h"
#include "panic.h"
#include "AudioLed.h"
#include "Memory.h"
#include <Arduino.h>
#include <Audio.h>
#include <SD.h>
#include <espeak-ng_T4.H>
#include <TeensyThreads.h>
#include "teensythread.h"
#include "Console.h"
#include "sarcasm_voice.h"
#include "misc.h"


namespace MAX98357A
    {


    static AudioPlaySdWav           playWav;
    static AudioOutputI2S           audioOutput;
    static AudioMixer4              mixer;
    static AudioAmplifier           amp;
    static AudioEffectFade          fade0;
    static AudioEffectFade          fade1;

    static AudioConnection          patchCord1(playWav, 0, fade0, 0);
    static AudioConnection          patchCord2(playWav, 1, fade1, 0);
    static AudioConnection          patchCord3(fade0, 0, mixer, 0);
    static AudioConnection          patchCord4(fade1, 0, mixer, 1);
    static AudioConnection          patchCord5(espeak, 0, mixer, 2);
    static AudioConnection          patchCord6(mixer, 0, amp, 0); // connect output to the I2S
    static AudioConnection          patchCord7(amp, 0, audioOutput, 0); // connect output to the I2S
    static AudioConnection          patchCord8(mixer, 0, audioLed, 0); // connect output to the led driver for Voice
    


    static void espeakDelay(uint32_t ms)
        { // make sure espaek uses teensythread's delay function
        threads.delay(ms); 
        }



    SLOWFUN void init()
        {
        AudioMemory(AUDIO_MEMORY_BLOCKS); // <- this is a macro that performs static allocation in DMAMEM for a given number of blocks
       
        static int8_t _init_done = 0;
        if (_init_done) return;
        _init_done = 1;
                 
        DEBUG("MAX98357A::init() started")

        pinMode(PIN_MAX98357A_PWR_CTRL, OUTPUT);
        digitalWrite(PIN_MAX98357A_PWR_CTRL, HIGH); // turn on the power        


        if (!(SD.begin(BUILTIN_SDCARD)))
            {
            ABORT("SD card initialization failed!");
            }
        playWav.begin();        
        
        espeak.begin(1); // memory in extmem
        espeak.setDelayFunction(espeakDelay); // set the delay function for espeak using teensythread
        espeak.registerVariant("sarcasm_voice", espeak_ng_data_sarcasm_voice, espeak_ng_data_sarcasm_voice_len);
        espeak.setRate(120);
        //espeak.setVoice("en+f2"); // set the voice to use
        espeak.setVoice("en+sarcasm_voice"); // set the voice to use
        mixer.gain(0, DEFAULT_SOUND_LEVEL_MUSIC); // set the gain for the first channel
        mixer.gain(1, DEFAULT_SOUND_LEVEL_MUSIC); // set the gain for the second channel
        mixer.gain(2, DEFAULT_SOUND_LEVEL_VOICE); // set the gain for the espeak channel
        mixer.gain(3, 0.0f); // set the gain for the third channel to 0 (not used)
        amp.gain(SOUND_LEVEL);
        DEBUG("MAX98357A::init() completed.\n")
        }


    SLOWFUN void disable()
        {
        digitalWrite(PIN_MAX98357A_PWR_CTRL, LOW); // turn on the power        
        }


    SLOWFUN void setAudioLevelMusic(float level)
        {
        fadeInMusic();
        if (level < 0.0f) level = 0.0f;
        if (level > 3.0f) level = 3.0f;
        mixer.gain(0, DEFAULT_SOUND_LEVEL_MUSIC * level); // set the gain for the first channel
        mixer.gain(1, DEFAULT_SOUND_LEVEL_MUSIC * level); // set the gain for the second channel        
        }

    SLOWFUN void fadeInMusic(int ms)
        {
        fade0.fadeIn(ms);
        fade1.fadeIn(ms);
        }

    SLOWFUN void fadeOutMusic(int ms)
        {
        fade0.fadeOut(ms);
        fade1.fadeOut(ms);
        }

    SLOWFUN void setAudioLevelVoice(float level)
        {
        if (level < 0.0f) level = 0.0f;
        if (level > 3.0f) level = 3.0f;
        mixer.gain(2, DEFAULT_SOUND_LEVEL_VOICE * level); // set the gain for the espeak channel
        }

    SLOWFUN void setAudioLevel(float level)
        {
        setAudioLevelMusic(level);
        setAudioLevelVoice(level);
        }


    SLOWFUN void playMusic(const char* filename, bool wait)
        {
        playWav.play(filename);
        if (wait)
            {
            waitEndPlayMusic();
            }
        }

    SLOWFUN uint32_t musicLenghtMs()
        {
        return playWav.lengthMillis();
        }


    SLOWFUN uint32_t musicPositionMs()
        {
        return playWav.positionMillis();
        }



    bool isPlayingMusic()
        {
        return playWav.isPlaying();
        }


    void stopMusic()
        {
        playWav.stop();
        }


    SLOWFUN void waitEndPlayMusic()
        {
        threads.delay(25);
        while (isPlayingMusic())
            {
            threads.delay(1);
            }
        }



    static volatile int _th_speak; // thread id for the speak thread
    static volatile int _is_speaking; 
    static Print * volatile  _speakTextOutput; 

    static void speakThread(void * text)
        {
        espeak.play((const char *)text, _speakTextOutput);
        _is_speaking = 0;
        }


    SLOWFUN void speak(const char* text, Print* textOutput)
        {
        if (_is_speaking) // if we are already speaking, stop the current speak
            {
            stopSpeak();
            }
        _is_speaking = 1; 
        _speakTextOutput = textOutput;
        _th_speak = threads.addThread(speakThread, (void*)text, ESPEAK_STACK_SIZE, Memory::espeak_stack);
        if (_th_speak < 0) { ABORT("MAX98357A::speak() failed to start thread !"); }
        teensythread::speedUp(_th_speak, 5.0f); // thread needs compute time in the beginning but will yield later
        }




    
    SLOWFUN void speakformatted(const char* text, int v1, int v2, int v3, Print* textOutput)
        {
        misc::formatText(Memory::speak_text_buffer, text, v1, v2, v3);
        speak(Memory::speak_text_buffer, textOutput);
        }



    SLOWFUN void stopSpeak()
        {
        if (_is_speaking)
            {            
            while (_is_speaking) 
                { 
                espeak.sendAbortSignal(); // send the abort signal to espeak
                threads.yield();
                }
            threads.wait(_th_speak, 1); // wait for the speak thread to finish
            }        
        }


    bool isSpeaking()
        {
        return (_is_speaking != 0);
        }


    SLOWFUN void waitEndSpeak()
        {
        while (_is_speaking)
            {
            if (getPanicFlag()) { return; }
            threads.delay(1); 
            }
        }

    }



/** end of file */