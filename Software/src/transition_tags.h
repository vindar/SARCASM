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

#include "config.h"
#include "debug.h"
#include <Arduino.h>
#include <TeensyThreads.h>
#include <tgx.h>
#include "indexpicker.h"


namespace transition_tags
    {

    // used by idleSolveAnimation.cpp   
    extern IndexPicker<NVS_ADR_TAG_IDLE_SOLVE_LEN>  picker_tag_idle_solve;
    const int tag_idle_solve_size = 20;
    extern const char* const tag_idle_solve[tag_idle_solve_size];
    extern const char* const tag_idle_solve_end[tag_idle_solve_size];


    // used by dreamingAnimation.cpp
    extern IndexPicker<NVS_ADR_TAG_DREAMING_LEN>  picker_tag_dreaming;
    const int tag_dreaming_size = 12;
    extern const char* const tag_dreaming[tag_dreaming_size];
    extern const char* const tag_dreaming_end[tag_dreaming_size];



    struct TagTransition 
        {
        char tag_start[21];
        char tag_end[21];
        uint8_t duration_seconds;
        uint8_t start_sound_id;
        uint8_t activity_sound_id;
        uint8_t end_sound_id;
        uint16_t fade_ms;
        float volume;
        };


    /**
    * Play a given transition tag. 
    * 
    * Returns: 
    *     CODE_NOTHING (0) if the transition was played successfully,
    *     CODE_TOUCHED_SCREEN if the transition was aborted by user pressing the screen (if allowed).
    *     CODE_SPINNED if the transition was aborted by the user by spinning the craddle (if allowed).
    *     CODE_CHARGING_START if the transition was aborted by the user start charging the machine (if allowed)
    *     CODE_CHARGING_STOP if the transition was aborted by the user stop charging the machine (if allowed)
    *     
    * return the cursor just below the tag. 
    **/
    int play_transition_tag(const TagTransition & tag, bool stop_on_touch, bool stop_on_craddle_spin, bool stop_on_charge_event);
   
    /**
    * Play a 'general' transition tag at random preventing repetition.
    **/
    int play_transition_tag_general(bool stop_on_touch, bool stop_on_craddle_spin, bool stop_on_charge_event);


    /**
    * Play a 'mood switching' transition tag at random preventing repetition.
    **/
    int play_transition_tag_switching(bool stop_on_touch, bool stop_on_craddle_spin, bool stop_on_charge_event);


    /**
    * Play a 'exiting aggressive mood' transition tag at random preventing repetition.
    **/
    int play_transition_tag_exit_aggressive(bool stop_on_touch, bool stop_on_craddle_spin, bool stop_on_charge_event);


    /**
    * Play a 'entering aggressive mood' transition tag at random preventing repetition.
    **/
    int play_transition_tag_enter_aggressive(bool stop_on_touch, bool stop_on_craddle_spin, bool stop_on_charge_event);


    /**
    * Reset the transition tag picker.
    * Optionally make so that the tags picked at next startup will be determinisitc
    **/
    SLOWFUN void resetTags(bool deterministic_at_startup);

    }
/** end of file */


