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

#include "mainLoop.h"

#include "config.h"
#include "Hardware.h"
#include "debug.h"
#include "Memory.h"
#include "PowerCtrl.h"
#include "debug.h" 
#include "panic.h"

#include "display.h"
#include "CraddleMove.h"
#include "Xorshift32.h"
#include "graphics3D.h"
#include "audioMAX98357A.h"
#include "AudioLed.h"
#include "Console.h"
#include "FBSensors.h"
#include "PowerCtrl.h"

#include "sentences.h"
#include "transition_tags.h"
#include "songs.h"
#include "RepeatSolvingAnimation.h"
#include "DreamingAnimation.h"
#include "idleSolveAnimation.h"
#include "BatteryAnimation.h"
#include "solveCube.h"
#include "shutdownSequence.h"









enum class Mood
    {
    MOOD_CURIOUS,
    MOOD_PLAYFUL,
    MOOD_LOGIC,
    MOOD_EXISTENTIAL,
    MOOD_HAUNTING,
    MOOD_DRYLOGIC,
    MOOD_MOCKING
    };

enum class Animation
    {
    ANIM_NONE,
    ANIM_REPLAY,
    ANIM_DREAMING,
    ANIM_IDLESOLVE,
    };



struct SarcasmState
    {

    SarcasmState() : _mood(Mood::MOOD_CURIOUS), _timeMoodChange(-1),
                     _timeInput(-1), _timeCradleMove(-1), _lastInputCode(CODE_NOTHING),
                     _nbSolves(0), _lastSolveDifficulty(0), _timeLastSolve(-1),
                     _nbAnimationsReplay(0), _timeReplay(-1),
                     _nbAnimationsDreaming(0), _timeDreaming(-1),
                     _nbAnimationsIdleSolve(0), _timeIdleSolve(-1),
                     _lastAnimationPlayed(Animation::ANIM_REPLAY), _timeAnimation(-1),
                     _timeBatteryAnimation(-1)
        {
        }


    /*--- MOOD ---*/
    Mood _mood; 
    int  _timeMoodChange; // time in ms since the last mood change.
    int _nbSpeechSameMood; // number of consecutive speeches in the same mood.

    Mood   currentMood() { return _mood; }
    int    timeSinceMoodChange() { return (_timeMoodChange < 0) ? _timeMoodChange : (int)(millis() - _timeMoodChange); }
    void   setMood(Mood m) { if (m != _mood) { _mood = m; _timeMoodChange = millis(); _nbSpeechSameMood = 0; } }
    void   incSpeechSameMood() { _nbSpeechSameMood++; }
    int    nbSpeechSameMood() { return _nbSpeechSameMood; }

    /*--- USER INPUT ---*/

    int _timeInput;  // time in ms since the last user input (touch, cradle or plug/unplug detected).
    int _timeCradleMove; // time since the last cradle move.
    int _lastInputCode;             // last input code (CODE_TOUCHED_SCREEN, CODE_SPINNED, CODE_CHARGING_START, CODE_CHARGING_STOP)

    int    timeSinceInput() { return (_timeInput < 0) ? _timeInput : (int)(millis() -_timeInput); }
    int    timeSinceCradleMove() { return (_timeCradleMove < 0) ? _timeCradleMove : (int)(millis() - _timeCradleMove); }
    int    lastInputCode() { return _lastInputCode; }
    void   setInput(int code)
        {
        if ((code != CODE_NOTHING) && (code != CODE_CHARGING_START) && (code != CODE_CHARGING_STOP))
            {
            _timeInput = millis();
            if (code == CODE_SPINNED) { _timeCradleMove = _timeInput; }
            _lastInputCode = code;
            }
        }

    /*--- SOLVES ---*/

    int _nbSolves;                  // number of solve performed
    int _solveNbMoves;              // number of moves in the last solve
    int _lastSolveDifficulty;       // last solve difficulty (impossible, easy, normal, already solved)
    int _timeLastSolve;             // time in ms since the last solve

    int    nbSolves() { return _nbSolves; }
    int    lastSolveDifficulty() { return _lastSolveDifficulty; }
    int    timeSinceLastSolve() { return (_timeLastSolve < 0) ? _timeLastSolve : (int)(millis() - _timeLastSolve); }
    int    lastSolveNbMoves() { return _solveNbMoves; }
    void   setSolve(int difficulty, int nbmoves) { _nbSolves++; _solveNbMoves = nbmoves; _lastSolveDifficulty = difficulty; _timeLastSolve = millis(); }

    /*--- ANIMATIONS ---*/

    int _nbAnimationsReplay;        // number of replay animations performed
    int _timeReplay; 
    int _nbAnimationsDreaming;      // number of dreaming animations performed
    int _timeDreaming;
    int _nbAnimationsIdleSolve;     // number of idleSolve animations performed
    int _timeIdleSolve;
    Animation _lastAnimationPlayed; // last animation played (replay, dreaming, idleSolve)
    int _timeAnimation;
    int _timeBatteryAnimation;      // time in ms since the last battery animation

    int    nbAnimationsReplay() { return _nbAnimationsReplay; }
    int    nbAnimationsDreaming() { return _nbAnimationsDreaming; }
    int    nbAnimationsIdleSolve() { return _nbAnimationsIdleSolve; }
    Animation lastAnimationPlayed() { return _lastAnimationPlayed; }

    int    timeSinceLastAnimation() { return (_timeAnimation < 0) ? _timeAnimation : (int)(millis() - _timeAnimation); }
    int    timeSinceLastReplay() { return (_timeReplay < 0) ? _timeReplay : (int)(millis() - _timeReplay); }
    int    timeSinceLastDreaming() { return (_timeDreaming < 0) ? _timeDreaming : (int)(millis() - _timeDreaming); }
    int    timeSinceLastIdleSolve() { return (_timeIdleSolve < 0) ? _timeIdleSolve : (int)(millis() - _timeIdleSolve); }
    int    timeSinceLastBatteryAnimation() { return (_timeBatteryAnimation < 0) ? _timeBatteryAnimation : (int)(millis() - _timeBatteryAnimation); }
    void   setAnimation(Animation anim)
        {
        if (anim == Animation::ANIM_NONE) return;
        _lastAnimationPlayed = anim;
        _timeAnimation = millis();
        switch (anim)
            {
            case Animation::ANIM_REPLAY:    _nbAnimationsReplay++;    _timeReplay = _timeAnimation; break;
            case Animation::ANIM_DREAMING:  _nbAnimationsDreaming++;  _timeDreaming = _timeAnimation; break;
            case Animation::ANIM_IDLESOLVE: _nbAnimationsIdleSolve++; _timeIdleSolve = _timeAnimation; break;
            default: break;
            }
        }

    bool replayCubeAvailable() { return (Memory::previous_solve_moves[0] != 0); }

    };



static SarcasmState state; // the main sarcasm state. 


#define MIN_TOUCH_DELAY 100 // minimum delay (ms) after which a touch is considered valid (to avoid false touches right after end of speech).


const char MOOD_CURIOUS_STR[] PROGMEM = "curious";
const char MOOD_PLAYFUL_STR[] PROGMEM = "playful";
const char MOOD_LOGIC_STR[] PROGMEM = "logic";
const char MOOD_EXISTENTIAL_STR[] PROGMEM = "existential";
const char MOOD_HAUNTING_STR[] PROGMEM = "haunting";
const char MOOD_DRYLOGIC_STR[] PROGMEM = "drylogic";
const char MOOD_MOCKING_STR[] PROGMEM = "mocking";
const char* const MOOD_NAMES[] PROGMEM = { MOOD_CURIOUS_STR, MOOD_PLAYFUL_STR, MOOD_LOGIC_STR, MOOD_EXISTENTIAL_STR, MOOD_HAUNTING_STR, MOOD_DRYLOGIC_STR, MOOD_MOCKING_STR };



/**
* Make a solve. 
**/
int mainloop_makeSolve()
    {
    while(1)
        {
        int dif, nbm;
        const int r = solveCube(dif, nbm); // go solve the cube. 
        state.setSolve(dif, nbm); // save the result. 
        if (r != CODE_SPINNED) 
            { 
            if (MAX98357A::isSpeaking()) { ABORT("Impossible 1"); }
            return CODE_SPINNED;  // *** may return r instead but ignore and always return CODE_SPINNED for the time being         
            } 
        state.setInput(CODE_SPINNED); // set the input code to spinned
        }
    }




/**
* Wait for speech to end before returning.
* 
* Returns
* 
*    CODE_NOTHING        : speech completed, no input detected.
*    CODE_SPINNED        : the cradle was spinned. Return immediately, speech may still be going on.
*    CODE_TOUCHED_SCREEN : the screen was touched. Return immediately or not, depending on interrupt_on_touch.
*    CODE_CHARGING_START / CODE_CHARGING_STOP: charging started. This code may only be returned if 'interrupt_on_charging' is set (and then return immediately, speech may be ongoing).
**/
SLOWFUN int mainloop_waitEndSpeech(bool interrupt_on_touch, bool interupt_on_charging)
    {
    const uint32_t lt = Display::touchedTime(); // time of last touch
    while (MAX98357A::isSpeaking())
        {
        if (CraddleMove::status() == 2) { state.setInput(CODE_SPINNED); return mainloop_makeSolve(); }
        if (interupt_on_charging)
            {
            const int st = FBSensors::chargeStatus();
            if ((st == CODE_CHARGING_START) || (st == CODE_CHARGING_STOP)) { state.setInput(st); return st; }
            }
        if (interrupt_on_touch)
            {
            if (Display::touchedTime() > lt + MIN_TOUCH_DELAY) { state.setInput(CODE_TOUCHED_SCREEN); return CODE_TOUCHED_SCREEN; }
            }
        graphics3D::updateAll(0); // redraw the screen
        threads.yield(); 
        }
    if (CraddleMove::status() == CODE_SPINNED) { state.setInput(CODE_SPINNED); return mainloop_makeSolve(); }
    if (Display::touchedTime() > lt + MIN_TOUCH_DELAY) { state.setInput(CODE_TOUCHED_SCREEN); return CODE_TOUCHED_SCREEN; }
    return CODE_NOTHING;
    }



/** Pause for a given number of milliseconds. */
SLOWFUN int mainloop_pause(int ms, bool interrupt_on_touch, bool interupt_on_charging)
    {
    MAX98357A::audioLed.setDriver(MAX98357A::AudioLed::DRIVER_VOICE); // set the audio LED driver to voice mode
    elapsedMillis em = 0;
    const uint32_t lt = Display::touchedTime(); // time of last touch
    while (((int)em) < ms)
        {
        if (CraddleMove::status() == 2) { state.setInput(CODE_SPINNED); return mainloop_makeSolve(); }
        if (interupt_on_charging)
            {
            const int st = FBSensors::chargeStatus();
            if ((st == CODE_CHARGING_START) || (st == CODE_CHARGING_STOP)) { state.setInput(st); return st; }
            }
        if (interrupt_on_touch)
            {
            if (Display::touchedTime() > lt + MIN_TOUCH_DELAY) { state.setInput(CODE_TOUCHED_SCREEN); return CODE_TOUCHED_SCREEN; }
            }
        graphics3D::updateAll(0); // redraw the screen
        threads.yield();
        }
    if (CraddleMove::status() == CODE_SPINNED) { state.setInput(CODE_SPINNED); return mainloop_makeSolve(); }
    if (Display::touchedTime() > lt + MIN_TOUCH_DELAY) { state.setInput(CODE_TOUCHED_SCREEN); return CODE_TOUCHED_SCREEN; }
    return CODE_NOTHING;
    }


/**
* Speak a text and output it on the console then wait for the end of speech and then wait for an additional 
* add_ms milliseconds before returning.
* 
* Returns
* 
*    CODE_NOTHING        : speech completed, no input detected.
*    CODE_SPINNED        : the cradle was spinned. Return immediately, speech may still be going on.
*    CODE_TOUCHED_SCREEN : the screen was touched. Return immediately or not, depending on interrupt_on_touch.
*    CODE_CHARGING_START / CODE_CHARGING_STOP: charging started/stopped.
**/
SLOWFUN int mainloop_speechAnimation(const char* message, int skip_lines, bool interrupt_on_touch, bool interupt_on_charging)
    {
    if (skip_lines > 0)
        {
        MAX98357A::stopSpeak();  // just in case, stop any ongoing speech
        for (int i = 0; i < skip_lines; i++) { Console::console.print("\n"); }
        }
    MAX98357A::audioLed.setDriver(MAX98357A::AudioLed::DRIVER_VOICE); // set the audio LED driver to voice mode
    MAX98357A::setAudioLevelVoice();
    MAX98357A::speak(message); // speak the message
    state.incSpeechSameMood();
    int r = mainloop_waitEndSpeech(interrupt_on_touch, interupt_on_charging);
    if (MAX98357A::isSpeaking())
        { // interrupted...
        MAX98357A::stopSpeak();
        Console::console.print("...\n\n");
        return r;
        }
    Console::console.print("\n\n");
    if (r != CODE_NOTHING)
        { // an input was detected just as the speech ended
        return r;
        }
    return r;
    }


SLOWFUN int mainloop_speechAnimationRandom(int skip_lines, bool interrupt_on_touch, bool interupt_on_charging)
    {
    int ev = 0;
    Mood cm = state.currentMood(); 
    if ((cm != Mood::MOOD_CURIOUS)&&(cm != Mood::MOOD_MOCKING))
        {
        ABORT("impossible: mood " << MOOD_NAMES[(int)cm]);
        }
    if (cm == Mood::MOOD_CURIOUS)
        { 
        const int n_c = sentences::picker_curious.size(); 
        const int n_p = sentences::picker_playful.size();
        const int n_l = sentences::picker_logic.size();
        const int n_e = sentences::picker_existential.size();
        const int n_h = sentences::picker_haunting.size();
        const int tot = n_c + n_p + n_l + n_e + n_h;
        const int a = rng::rand(0, tot);
        if (a < n_c)
            {
            DEBUG("Current mood 'curious': playing picker 'curious'");
            ev = mainloop_speechAnimation(sentences::picker_curious.next_str(), skip_lines, interrupt_on_touch, interupt_on_charging);
            }
        else if (a < n_c + n_p)
            {
            DEBUG("Current mood 'curious': playing picker 'playful'");
            ev = mainloop_speechAnimation(sentences::picker_playful.next_str(), skip_lines, interrupt_on_touch, interupt_on_charging);
            }
        else if (a < n_c + n_p + n_l)
            {
            DEBUG("Current mood 'curious': playing picker 'logic'");
            ev = mainloop_speechAnimation(sentences::picker_logic.next_str(), skip_lines, interrupt_on_touch, interupt_on_charging);
            }
        else if (a < n_c + n_p + n_l + n_e)
            {
            DEBUG("Current mood 'curious': playing picker 'existential'");
            ev = mainloop_speechAnimation(sentences::picker_existential.next_str(), skip_lines, interrupt_on_touch, interupt_on_charging);
            }
        else
            {
            DEBUG("Current mood 'curious': playing picker 'haunting'");
            ev = mainloop_speechAnimation(sentences::picker_haunting.next_str(), skip_lines, interrupt_on_touch, interupt_on_charging);
            }   
        }
    else
        {
        const int n_m = sentences::picker_mocking.size();
        const int n_d = sentences::picker_drylogic.size();
        const int tot = n_m + n_d;
        const int a = rng::rand(0, tot);
        if (a < n_m)
            {
            DEBUG("Current mood 'mocking': playing picker 'mocking'");
            ev = mainloop_speechAnimation(sentences::picker_mocking.next_str(), skip_lines, interrupt_on_touch, interupt_on_charging);
            } 
        else
            {
            DEBUG("Current mood 'mocking': playing picker 'drylogic'");
            ev = mainloop_speechAnimation(sentences::picker_drylogic.next_str(), skip_lines, interrupt_on_touch, interupt_on_charging);
            }
        }
    return ev;
    }



/** Perform a tag animation: 'general tag' **/
SLOWFUN int mainloop_tagAnimation_general(int skip_lines, bool stop_on_craddle_spin, bool stop_on_charge_event)
    {
    if (skip_lines > 0)
        {
        MAX98357A::stopSpeak();  // just in case, stop any ongoing speech
        for (int i = 0; i < skip_lines; i++) { Console::console.print("\n"); }
        }
    int r = transition_tags::play_transition_tag_general(true, stop_on_craddle_spin, stop_on_charge_event);
    if (r != CODE_NOTHING) { state.setInput(r); }
    if (r == CODE_SPINNED) { return mainloop_makeSolve(); }
    return r;
    }


/** Perform a tag animation: 'switching tag' **/
SLOWFUN int mainloop_tagAnimation_switching(int skip_lines, bool stop_on_craddle_spin, bool stop_on_charge_event)
    {
    if (skip_lines > 0)
        {
        MAX98357A::stopSpeak();  // just in case, stop any ongoing speech
        for (int i = 0; i < skip_lines; i++) { Console::console.print("\n"); }
        }
    int r = transition_tags::play_transition_tag_switching(true, stop_on_craddle_spin, stop_on_charge_event);
    if (r != CODE_NOTHING) { state.setInput(r); }
    if (r == CODE_SPINNED) { return mainloop_makeSolve(); }
    return r;
    }


/** Perform a tag animation: 'enter agressive tag' **/
SLOWFUN int mainloop_tagAnimation_enter_aggressive(int skip_lines, bool stop_on_craddle_spin, bool stop_on_charge_event)
    {
    if (skip_lines > 0)
        {
        MAX98357A::stopSpeak();  // just in case, stop any ongoing speech
        for (int i = 0; i < skip_lines; i++) { Console::console.print("\n"); }
        }
    int r = transition_tags::play_transition_tag_enter_aggressive(true, stop_on_craddle_spin, stop_on_charge_event);
    if (r != CODE_NOTHING) { state.setInput(r); }
    if (r == CODE_SPINNED) { return mainloop_makeSolve(); }
    return r;
    }


/** Perform a tag animation: 'exit agressive tag' **/
SLOWFUN int mainloop_tagAnimation_exit_aggressive(int skip_lines, bool stop_on_craddle_spin, bool stop_on_charge_event)
    {
    if (skip_lines > 0)
        {
        MAX98357A::stopSpeak();  // just in case, stop any ongoing speech
        for (int i = 0; i < skip_lines; i++) { Console::console.print("\n"); }
        }
    int r = transition_tags::play_transition_tag_exit_aggressive(true, stop_on_craddle_spin, stop_on_charge_event);
    if (r != CODE_NOTHING) { state.setInput(r); }
    if (r == CODE_SPINNED) { return mainloop_makeSolve(); }
    return r;
    }


/** Perform a dreaming animation **/
int mainloop_dreamingAnimation()
    {
    int r = dreamingAnimation();
    state.setAnimation(Animation::ANIM_DREAMING);
    if (r != CODE_NOTHING) { state.setInput(r); }
    if (r == CODE_SPINNED) { return mainloop_makeSolve(); }
    return r;
    }


/** Perform a dreaming animation **/
int mainloop_idleSolveAnimation()
    {
    int r = idleSolveAnimation();
    state.setAnimation(Animation::ANIM_IDLESOLVE);
    if (r != CODE_NOTHING) { state.setInput(r); }
    if (r == CODE_SPINNED) { return mainloop_makeSolve(); }
    return r;
    }


/** Perform a repeat solving animation **/
int mainloop_repeatSolvingAnimation()
    {
    int r = repeatSolvingAnimation();
    state.setAnimation(Animation::ANIM_REPLAY);
    if (r != CODE_NOTHING) { state.setInput(r); }
    if (r == CODE_SPINNED) { return mainloop_makeSolve(); }
    return r;
    }


/** Perform a battery animation **/
int mainloop_batteryAnimation(int code)
    {
    int r = batteryAnimation(code, false);    
    if (r != CODE_NOTHING) { state.setInput(r); }
    if (r == CODE_SPINNED) { return mainloop_makeSolve(); }
    return r;
    }


/** Perform the shutdown sequence **/
int mainloop_shutdownSequence()
    {
    int r = shutDownSequence();
    state.setInput(r); 
    if (r == CODE_SPINNED) { return mainloop_makeSolve(); }
    return r;
    }










#define MIN_MOOD_SPEECH 2
#define MAX_MOOD_SPEECH 4
#define MOOD_CHANGE_PROB (0.5f)

#define PROB_SKIP_SPEECH (0.3f)

int previous_nb_tags = 0; // number of tags played the previous time. 
bool skipped_speech = true; 
bool never_skipped_speech = true;

#define MIN_WAIT 2000
#define MAX_WAIT 4500

#define MIN_WAIT_SMALL 1000
#define MAX_WAIT_SMALL 2500


#define IDLE_TIME_SHUTDOWN (180*1000) // shutdown after 4 minute or idleness

int nextWait = -1;  // -1 iss long, -2 is short

#define BATTERY_STATUS_PERIOD_MS (60*1000)
elapsedMillis batteryStatus_ms; // time of last battery message
bool battery_normal_shown = false; // true if the normal battery status has been shown at least once.


void mainLoop()
    {
    // greeting speech, and then wait a bit before starting the main loop.
    int ev = mainloop_speechAnimation(sentences::picker_greetings.next_str(), 0, false, false);

    // check battery status
    if (FBSensors::getVoltage() < (VOLTAGE_THRESHOLD_LOW - 0.1f))
        {
        mainloop_batteryAnimation(0); // display critical battery message        
        }
    batteryStatus_ms = BATTERY_STATUS_PERIOD_MS;

    while (1)
        {
        // *** We are at the strat of a new loop ****
        
        // ---------  wait a bit ----------       
        int ms = (nextWait >= 0) ? nextWait : (((nextWait == -1) ? rng::rand(MIN_WAIT, MAX_WAIT) : rng::rand(MIN_WAIT_SMALL, MAX_WAIT_SMALL)));
        ev = mainloop_pause(ms, true, true);
        nextWait = -1; // reset to long wait.
        // check for inactivity shutdown                           
        if (state.timeSinceInput() > IDLE_TIME_SHUTDOWN)
            {
            mainloop_shutdownSequence();
            continue;  // if we return from shutdown, that means SPIN or TOUCHED. Either way, we go back to start of main loop. 
            }

        if (ev == CODE_SPINNED) { nextWait = -2; continue; } // just solve the cube, go back to main loop. 
        else if ((ev == CODE_CHARGING_START)||(ev == CODE_CHARGING_STOP))
            { // battery status change. 
            ev = mainloop_batteryAnimation(ev);
            if (ev == CODE_SPINNED) { nextWait = -2;  continue; } // go back to main loop
            else if (ev == CODE_TOUCHED_SCREEN) { nextWait = 1500; continue; } // go back to main loop
            }
        else if (ev == CODE_TOUCHED_SCREEN)
            { // play a speech
            mainloop_speechAnimationRandom(1, true, true);
            if (ev == CODE_SPINNED) { nextWait = -2; continue; } // go back to main loop
            else if ((ev == CODE_CHARGING_START) || (ev == CODE_CHARGING_STOP))
                { // battery status change. 
                ev = mainloop_batteryAnimation(ev);
                if (ev == CODE_SPINNED) { nextWait = -2; continue; } // go back to main loop
                if (ev == CODE_TOUCHED_SCREEN) { nextWait = 1500; continue; } // go back to main loop
                }
            else if (ev == CODE_TOUCHED_SCREEN) { nextWait = 2000; continue; } // go back to main loop
            }

        // -------- check mood change --------------  
        bool switchmood = false;
        Mood cm = state.currentMood();          // current mood
        int nbsame = state.nbSpeechSameMood();  // number of consecutive speeches in the same mood    
        DEBUG("Current mood is " << MOOD_NAMES[(int)cm] << " since " << state.timeSinceMoodChange() << " ms, with " << nbsame << " speeches in the same mood.");
        if (nbsame > MIN_MOOD_SPEECH)
            { // may change mood
            if ((nbsame > MAX_MOOD_SPEECH) || (rng::unif() < MOOD_CHANGE_PROB))
                { // ok, we change the mood. 
                if (cm == Mood::MOOD_CURIOUS) { state.setMood(Mood::MOOD_MOCKING); } else { state.setMood(Mood::MOOD_MOCKING); }
                DEBUG("Changing mood from " << MOOD_NAMES[(int)cm] << " to " << MOOD_NAMES[(int)state.currentMood()] << " after " << nbsame << " speeches.");
                switchmood = true;
                }
            }

        // --------- play tags ------------------               
        int nbtags = ((previous_nb_tags == 0) ? (rng::rand(1, 3)) : (rng::rand(0, 3))); // choose the number of tags. 
        if ((switchmood) && (nbtags > 1)) { nbtags = rng::rand(2); } // in case of mood change, only 0 or 1 at most. 

        static int nbll = 0; 
        if (nbll < 3)
            {
            nbll++; 
            if (switchmood) { nbtags = 0; }
            if (nbtags > 1) { nbtags = 1; }
            }


        for (int k = 0; k < nbtags; k++)
            { // play the general tags. 
            ev = mainloop_tagAnimation_general(((k == 0) ? 1 : 0), true, true);
            if (ev == CODE_SPINNED) { nextWait = -2; continue; } // go back to main loop
            else if ((ev == CODE_CHARGING_START) || (ev == CODE_CHARGING_STOP))
                { // battery status change. 
                ev = mainloop_batteryAnimation(ev);
                if (ev == CODE_SPINNED) { nextWait = -2;  continue; } // go back to main loop
                if (ev == CODE_TOUCHED_SCREEN)  { nextWait = 500; continue;} // go back to main loop
                }
            else if (ev == CODE_TOUCHED_SCREEN)
                { // play a speech
                mainloop_speechAnimationRandom(1, true, true);
                if (ev == CODE_SPINNED) { nextWait = -2; continue; } // go back to main loop
                else if ((ev == CODE_CHARGING_START) || (ev == CODE_CHARGING_STOP))
                    { // battery status change. 
                    ev = mainloop_batteryAnimation(ev);
                    if (ev == CODE_SPINNED) { nextWait = -2; continue; } // go back to main loop
                    else if (ev == CODE_TOUCHED_SCREEN) { nextWait = 1500; continue; } // go back to main loop
                    }
                else if (ev == CODE_TOUCHED_SCREEN) { nextWait = 500; continue; } // go back to main loop
                }
            }
        if ((nbtags == 0) && (!switchmood))
            { // no tags, just wait a bit
            Console::console.print("\n");
            ev = mainloop_pause(1000, true, true);
            if (ev == CODE_SPINNED) { nextWait = -2; continue; } // go back to main loop
            else if ((ev == CODE_CHARGING_START) || (ev == CODE_CHARGING_STOP))
                { // battery status change. 
                ev = mainloop_batteryAnimation(ev);
                if (ev == CODE_SPINNED) { nextWait = -2;  continue; } // go back to main loop
                if (ev == CODE_TOUCHED_SCREEN)  { nextWait = 500; continue;} // go back to main loop
                }
            else if (ev == CODE_TOUCHED_SCREEN)
                { // play a speech
                mainloop_speechAnimationRandom(1, true, true);
                if (ev == CODE_SPINNED) { nextWait = -2; continue; } // go back to main loop
                else if ((ev == CODE_CHARGING_START) || (ev == CODE_CHARGING_STOP))
                    { // battery status change. 
                    ev = mainloop_batteryAnimation(ev);
                    if (ev == CODE_SPINNED) { nextWait = -2; continue; } // go back to main loop
                    else if (ev != CODE_TOUCHED_SCREEN) { nextWait = 1500; continue; } // go back to main loop
                    }
                else if (ev == CODE_TOUCHED_SCREEN) { nextWait = 500; continue; } // go back to main loop
                }
            }

        if (switchmood)
            {
            // play the switching tag. 
            ev = mainloop_tagAnimation_switching(((nbtags == 0) ? 1 : 0), true, true);
            if (ev == CODE_SPINNED) { nextWait = -2; continue; } // go back to main loop
            else if ((ev == CODE_CHARGING_START) || (ev == CODE_CHARGING_STOP))
                { // battery status change. 
                ev = mainloop_batteryAnimation(ev);
                if (ev == CODE_SPINNED) { nextWait = -2;  continue; } // go back to main loop
                if (ev != CODE_TOUCHED_SCREEN)  { nextWait = 500; continue;} // go back to main loop
                }
            else if (ev == CODE_TOUCHED_SCREEN)
                { // play a speech
                mainloop_speechAnimationRandom(1, true, true);
                if (ev == CODE_SPINNED) { nextWait = -2; continue; } // go back to main loop
                else if ((ev == CODE_CHARGING_START) || (ev == CODE_CHARGING_STOP))
                    { // battery status change. 
                    ev = mainloop_batteryAnimation(ev);
                    if (ev == CODE_SPINNED) { nextWait = -2; continue; } // go back to main loop
                    else if (ev == CODE_TOUCHED_SCREEN) { nextWait = 1500; continue; } // go back to main loop
                    }
                else if (ev == CODE_TOUCHED_SCREEN) { nextWait = 500; continue; } // go back to main loop
                }
            // play the mood change tag
            if (cm == Mood::MOOD_CURIOUS)
                {
                ev = mainloop_tagAnimation_enter_aggressive(0, true, true);
                }
            else
                {
                ev = mainloop_tagAnimation_exit_aggressive(0, true, true);
                }
            if (ev == CODE_SPINNED) { nextWait = -2; continue; } // go back to main loop
            else if ((ev == CODE_CHARGING_START) || (ev == CODE_CHARGING_STOP))
                { // battery status change. 
                ev = mainloop_batteryAnimation(ev);
                if (ev == CODE_SPINNED) { nextWait = -2;  continue; } // go back to main loop
                if (ev != CODE_TOUCHED_SCREEN)  { nextWait = 500; continue;} // go back to main loop
                }
            else if (ev == CODE_TOUCHED_SCREEN)
                { // play a speech
                mainloop_speechAnimationRandom(1, true, true);
                if (ev == CODE_SPINNED) { nextWait = -2; continue; } // go back to main loop
                else if ((ev == CODE_CHARGING_START) || (ev == CODE_CHARGING_STOP))
                    { // battery status change. 
                    ev = mainloop_batteryAnimation(ev);
                    if (ev == CODE_SPINNED) { nextWait = -2; continue; } // go back to main loop
                    else if (ev == CODE_TOUCHED_SCREEN) { nextWait = 1500; continue; } // go back to main loop
                    }
                else if (ev == CODE_TOUCHED_SCREEN) { nextWait = 500; continue; } // go back to main loop
                }
            }

        
        // -------- make a speech --------------
        if ((never_skipped_speech) || ((!skipped_speech) && (nbtags > 0) && (!switchmood) && (rng::unif() < PROB_SKIP_SPEECH)))
            { // skip speech
            DEBUG("skipping Speech after tags");
            skipped_speech = true;        
            }
        else
            { // do not skip
            skipped_speech = false;
            }
        if (!skipped_speech)
            { // make the speech, according to the mood. 
            never_skipped_speech = false;
            mainloop_speechAnimationRandom(1, false, true);
            if (ev == CODE_SPINNED) { continue; } // go back to main loop
            if ((ev == CODE_CHARGING_START) || (ev == CODE_CHARGING_STOP))
                { // battery status change. 
                ev = mainloop_batteryAnimation(ev);
                if (ev == CODE_SPINNED) { nextWait = -2; continue; } // go back to main loop
                if (ev == CODE_TOUCHED_SCREEN) { nextWait = 500; continue; } // go back to main loop
                }
            if (ev == CODE_TOUCHED_SCREEN) { nextWait = 500; continue; } // go back to main loop
            }

        // choose if we play an animation. 
        Animation anim = Animation::ANIM_NONE;
        if (state.replayCubeAvailable())
            { // replay available so we play it. 
            anim = Animation::ANIM_REPLAY;
            nextWait = -2;
            }
        else
            {
            const int nbD = state.nbAnimationsDreaming();
            const int nbI = state.nbAnimationsIdleSolve();
            if (nbI == nbD)
                {
                if (nbI == 0)
                    {
                    anim = Animation::ANIM_IDLESOLVE;
                    }
                else
                    {
                    int j = rng::rand(0, 3);
                    if (j == 0) { anim = Animation::ANIM_DREAMING; } else if (j == 1) { anim = Animation::ANIM_IDLESOLVE; } else { anim = Animation::ANIM_NONE; }
                    }
                }
            else if (nbI < nbD)
                {
                anim = Animation::ANIM_IDLESOLVE;
                nextWait = -2;
                }
            else
                {
                anim = Animation::ANIM_DREAMING;
                nextWait = -2;
                }
            }


        // play animation if anim != 0
        if (anim != Animation::ANIM_NONE)
            {
            // wait a little
            int ms = rng::rand(MIN_WAIT_SMALL, MAX_WAIT_SMALL);
            ev = mainloop_pause(ms, true, true);
            if (ev == CODE_SPINNED) { continue; } // just solve the cube, go back to main loop. 
            if ((ev == CODE_CHARGING_START) || (ev == CODE_CHARGING_STOP))
                { // battery status change. 
                ev = mainloop_batteryAnimation(ev);
                if (ev == CODE_SPINNED) { continue; } // go back to main loop
                if (ev == CODE_TOUCHED_SCREEN) { continue; } // go back to main loop
                }
            // play the animation
            if (anim == Animation::ANIM_REPLAY)
                {
                ev = mainloop_repeatSolvingAnimation();
                }
            else if (anim == Animation::ANIM_DREAMING)
                {
                ev = mainloop_dreamingAnimation();
                }
            else if (anim == Animation::ANIM_IDLESOLVE)
                {
                ev = mainloop_idleSolveAnimation();                        
                }
            else
                {
                ABORT("Impossible animation " << (int)anim);
                }
            if (ev == CODE_SPINNED) { continue; } // just solve the cube, go back to main loop. 
            if ((ev == CODE_CHARGING_START) || (ev == CODE_CHARGING_STOP))
                { // battery status change. 
                ev = mainloop_batteryAnimation(ev);
                if (ev == CODE_SPINNED) { continue; } // go back to main loop
                if (ev == CODE_TOUCHED_SCREEN) { continue; } // go back to main loop
                }

            // make a speech after animation.            
            mainloop_speechAnimationRandom(1, false, true);
            if (ev == CODE_SPINNED) { continue; } // go back to main loop
            else if ((ev == CODE_CHARGING_START) || (ev == CODE_CHARGING_STOP))
                { // battery status change. 
                ev = mainloop_batteryAnimation(ev);
                if (ev == CODE_SPINNED) { continue; } // go back to main loop
                else if (ev == CODE_TOUCHED_SCREEN) { continue; } // go back to main loop
                }
            else if (ev == CODE_TOUCHED_SCREEN) { nextWait = 500; continue; } // go back to main loop
            }


        // check battery 
        if (batteryStatus_ms >= BATTERY_STATUS_PERIOD_MS)
            {
            if ((FBSensors::getVoltage() < (VOLTAGE_THRESHOLD_LOW - 0.1f)) || (battery_normal_shown == false))
                {
                ev = mainloop_batteryAnimation(0); // display critical battery message
                battery_normal_shown = true;
                batteryStatus_ms = 0;
                if (ev == CODE_SPINNED) { nextWait = -2;  continue; } // go back to main loop
                }
            }
        // loop complete
        }
    }







/**
*
*
*


1. wait a bit [4 to 8sec]


2. play 0,1,2 or 3 tags. (change mood according to MC). 
3. immediatly play a speech (90%) or nothing (10%). 

4. 
   - If replay available, wait for 2secondes then play replay. 
   - else, choose between
         - idleSolve (40%)  : in this case, wait 2 seconds before playing
         - dreaming (60%) : in this case wait 2 seconds before playing
         - nothing (10%) : break. 




then either continue (probability 









State: 

- number of solves. 

- time since last touched
- time since last cradle spin
- last solve result. 

MOOD: 

Query
- currentMood();     // the current mood
- timeSinceMoodChange();  // time in milliseconds since the last mood change. 
- setMood();         // change the mood.

IDLE:

- timeSinceInput()      // time in ms since the last user input (touch, cradle or plug/unplug detected). 
- timeSinceCradleMove()  // time since the last cradle move. 
- setInput()


SOLVES:

- nbSolves()             // number of solve performed
- lastSolveDifficulty()  // last solve difficulty (impossible, easy, normal, already solved)
- timeSinceLastSolve()        // time in ms since the last solve
- setSolve()

ANIMATION:

- nbAnimationsReplay()   // number of replay animations performed
- nbAnimationsDreaming() // number of dreaming animations performed
- nbAnimationsIdleSolve() // number of idleSolve animations performed
- lastAnimationPlayed()  // last animation played (replay, dreaming, idleSolve)
- timeSinceLastAnimation() // time in ms since the last animation
- timeSinceLastReplay()    // time in ms since the last replay animation
- timeSinceLastDreaming() // time in ms since the last dreaming animation
- timeSinceLastIdleSolve() // time in ms since the last idleSolve animation
- timeSinceLastBatteryAnimation() // time in ms since the last battery animation
- setAnimation()

- replayedCubeAvailable() // is there an unreplayed cube available ? (i.e. a cube that was solved since the last replay animation)

*/



/** end of file */
