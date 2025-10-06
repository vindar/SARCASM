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

namespace songs
    {

    // The index picker object to choose a song during solve at random without too much repetition
    extern IndexPicker<NVS_ADR_SONG_SOLVE_LEN>  picker_song_solve;

    /** Return the filename of a song during (for solve) given its index */
    SLOWFUN const char* song_solve_filename(uint8_t index);

    /** Return the intro sentence of a song (for solve)  given its index */
    SLOWFUN const char* song_solve_intro_sentence(uint8_t index);



    // The index picker object to choose a song for ending at random without too much repetition
    extern IndexPicker<NVS_ADR_SONG_ENDING_LEN>  picker_song_ending;

    /** Return the filename of a song during (for ending) given its index */
    SLOWFUN const char* song_ending_filename(uint8_t index);

    /** Return the intro sentence of a song (for ending)  given its index */
    SLOWFUN const char* song_ending_intro_sentence(uint8_t index);



    // indexPicker for songs for the dreaming animation
    extern IndexPicker<NVS_ADR_SONG_DREAMING_LEN>  picker_song_dreaming;

    // indexPicker for song for the idlesolve animation
    extern IndexPicker<NVS_ADR_SONG_IDLESOLVE_LEN>  picker_song_idlesolve;




    /**
    * Reset the song picker.
    * Optionally make so that the songs picked at next startup will be determinisitc
    **/
    SLOWFUN void resetSongs(bool deterministic_at_startup);
    }

/** end of file */




