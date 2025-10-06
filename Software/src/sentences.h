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

#include <Arduino.h>
#include "indexpicker.h"

namespace sentences
    {

    extern const char solution_found_str[]; // "Solution found in [] moves."

    extern const char remaining_moves_str[]; // "Moves remaining:\n"

    /**
    * The index picker objects to choose a sentence at random without too much repetition
    **/

    extern IndexPicker<NVS_ADR_GREETING_LEN>               picker_greetings;

    extern IndexPicker<NVS_ADR_CURIOUS_LEN>                picker_curious;
    extern IndexPicker<NVS_ADR_PLAYFUL_LEN>                picker_playful;
    extern IndexPicker<NVS_ADR_LOGIC_LEN>                  picker_logic;
    extern IndexPicker<NVS_ADR_EXISTENTIAL_LEN>            picker_existential;
    extern IndexPicker<NVS_ADR_HAUNTING_LEN>               picker_haunting;

    extern IndexPicker<NVS_ADR_DRYLOGIC_LEN>               picker_drylogic;
    extern IndexPicker<NVS_ADR_MOCKING_LEN>                picker_mocking;

    extern IndexPicker<NVS_ADR_VICTORY_NOCUBE_LEN>         picker_victory_nocube;
    extern IndexPicker<NVS_ADR_VICTORY_INVALID_LEN>        picker_victory_invalid;
    extern IndexPicker<NVS_ADR_VICTORY_ALREADYSOLVED_LEN>  picker_victory_alreadysolved;
    extern IndexPicker<NVS_ADR_VICTORY_NORMAL_LEN>         picker_victory_normal;

    extern IndexPicker<NVS_ADR_RES_TRIVIAL_LEN>            picker_res_trivial;
    extern IndexPicker<NVS_ADR_RES_TRIVIAL_TAIL_LEN>       picker_res_trivial_tail;
    extern IndexPicker<NVS_ADR_RES_MODERATE_LEN>           picker_res_moderate;
    extern IndexPicker<NVS_ADR_RES_MODERATE_TAIL_LEN>      picker_res_moderate_tail;

    extern IndexPicker<NVS_ADR_SOLVE_START_LEN>            picker_solve_start;

    extern IndexPicker<NVS_ADR_RES_MUSIC_ABORT_LEN>        picker_res_music_abort;

    extern IndexPicker<NVS_ADR_SHUTDOWN_ABORTED_LEN>       picker_shutdown_aborted;       

    extern IndexPicker<NVS_ADR_POWER_ON_LEN>               picker_power_on;
    extern IndexPicker<NVS_ADR_POWER_OFF_LEN>              picker_power_off;
    extern IndexPicker<NVS_ADR_BATTERY_CRITICAL_LEN>       picker_battery_critical;
    extern IndexPicker<NVS_ADR_BATTERY_OK_LEN>             picker_battery_ok;
    extern IndexPicker<NVS_ADR_BATTERY_CHARGING_LEN>       picker_battery_charging;

    extern IndexPicker<NVS_ADR_REPLAYCUBE_LEN>              picker_replaycube;


    /**
    * Reset all sentence pickers 
    * Optionally make so that the sentence picked at next startup will be determinisitc
    **/
    SLOWFUN void resetSentences(bool deterministic_at_startup);


    }


/** end of file */


