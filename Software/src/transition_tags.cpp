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


#include "transition_tags.h"
#include "display.h"
#include "Console.h"
#include "AudioMAX98357A.h"
#include "AudioLed.h"
#include "debug.h"
#include "craddlemove.h"
#include "UI.h"
#include "FBSensors.h"
#include "indexpicker.h"

#include <tgx.h>
#include <TeensyThreads.h>
#include <Arduino.h>



namespace transition_tags
    {

    /**
    * Sounds during activity
    **/
#define TAG_SOUND_ACTIVITY_BUZZING     0
#define TAG_SOUND_ACTIVITY_MECHANICAL  1
#define TAG_SOUND_ACTIVITY_GLITCH      2
#define TAG_SOUND_ACTIVITY_LOGIC       3
#define TAG_SOUND_ACTIVITY_CALM        4
#define TAG_SOUND_ACTIVITY_SCAN        5
#define TAG_SOUND_ACTIVITY_SOFT1       6
#define TAG_SOUND_ACTIVITY_SOFT2       7
#define TAG_SOUND_ACTIVITY_SOFT3       8

    const char FILENAME_SOUND_ACTIVITY_BUZZING[] PROGMEM = "act_sounds/tag_activity_buzzing.wav";
    const char FILENAME_SOUND_ACTIVITY_MECHANICAL[] PROGMEM = "act_sounds/tag_activity_mechanical.wav";
    const char FILENAME_SOUND_ACTIVITY_GLITCH[] PROGMEM = "act_sounds/tag_activity_glitch.wav";
    const char FILENAME_SOUND_ACTIVITY_LOGIC[] PROGMEM = "act_sounds/tag_activity_logic.wav";
    const char FILENAME_SOUND_ACTIVITY_CALM[] PROGMEM = "act_sounds/tag_activity_calm.wav";
    const char FILENAME_SOUND_ACTIVITY_SCAN[] PROGMEM = "act_sounds/tag_activity_scan.wav";
    const char FILENAME_SOUND_ACTIVITY_SOFT1[] PROGMEM = "act_sounds/tag_activity_soft1.wav";
    const char FILENAME_SOUND_ACTIVITY_SOFT2[] PROGMEM = "act_sounds/tag_activity_soft2.wav";
    const char FILENAME_SOUND_ACTIVITY_SOFT3[] PROGMEM = "act_sounds/tag_activity_soft3.wav";


    const char* const PROGMEM tag_activity_filenames[9] = {
                FILENAME_SOUND_ACTIVITY_BUZZING,
                FILENAME_SOUND_ACTIVITY_MECHANICAL,
                FILENAME_SOUND_ACTIVITY_GLITCH,
                FILENAME_SOUND_ACTIVITY_LOGIC,
                FILENAME_SOUND_ACTIVITY_CALM,
                FILENAME_SOUND_ACTIVITY_SCAN,
                FILENAME_SOUND_ACTIVITY_SOFT1,
                FILENAME_SOUND_ACTIVITY_SOFT2,
                FILENAME_SOUND_ACTIVITY_SOFT3
        };



    /**
    * Sound at start of activity
    **/

#define TAG_SOUND_START_TRIOLET         0
#define TAG_SOUND_START_SWEEP           1
#define TAG_SOUND_START_PING_SOFT       2
#define TAG_SOUND_START_PING_EHANCED    3
#define TAG_SOUND_START_BLIP            4

    const char FILENAME_SOUND_START_TRIOLET[] PROGMEM = "act_sounds/tag_start_triolet.wav";
    const char FILENAME_SOUND_START_SWEEP[] PROGMEM = "act_sounds/tag_start_sweep.wav";
    const char FILENAME_SOUND_START_PING_SOFT[] PROGMEM = "act_sounds/tag_start_ping_soft.wav";
    const char FILENAME_SOUND_START_PING_EHANCED[] PROGMEM = "act_sounds/tag_start_ping_enhanced.wav";
    const char FILENAME_SOUND_START_BLIP[] PROGMEM = "act_sounds/tag_start_blip.wav";

    const char* const PROGMEM tag_start_filenames[5] = {
                FILENAME_SOUND_START_TRIOLET,
                FILENAME_SOUND_START_SWEEP,
                FILENAME_SOUND_START_PING_SOFT,
                FILENAME_SOUND_START_PING_EHANCED,
                FILENAME_SOUND_START_BLIP
        };


    /**
    * Sound at end of activity
    **/
#define TAG_SOUND_END_SHORT            0
#define TAG_SOUND_END_ELECTRONIC       1
#define TAG_SOUND_END_HIGH             2
#define TAG_SOUND_END_LOW              3
#define TAG_SOUND_END_NEUTRAL          4
#define TAG_SOUND_END_ABORT            5

    const char FILENAME_SOUND_END_SHORT[] PROGMEM = "act_sounds/tag_end_short.wav";
    const char FILENAME_SOUND_END_ELECTRONIC[] PROGMEM = "act_sounds/tag_end_electronic.wav";
    const char FILENAME_SOUND_END_HIGH[] PROGMEM = "act_sounds/tag_end_high.wav";
    const char FILENAME_SOUND_END_LOW[] PROGMEM = "act_sounds/tag_end_low.wav";
    const char FILENAME_SOUND_END_NEUTRAL[] PROGMEM = "act_sounds/tag_end_neutral.wav";
    const char FILENAME_SOUND_END_ABORT[] PROGMEM = "act_sounds/tag_end_abort.wav";


    const char* const PROGMEM tag_end_filenames[6] = {
                FILENAME_SOUND_END_SHORT,
                FILENAME_SOUND_END_ELECTRONIC,
                FILENAME_SOUND_END_HIGH,
                FILENAME_SOUND_END_LOW,
                FILENAME_SOUND_END_NEUTRAL,
                FILENAME_SOUND_END_ABORT
        };



    const char blank_line[26] PROGMEM = "                         ";


    const int tag_transitions_size = 72;

    const int tag_transitions_index_general_start = 0;
    const int tag_transitions_index_general_end = 39;

    const int tag_transitions_index_switching_start = 39;
    const int tag_transitions_index_switching_end = 51;

    const int tag_transitions_index_exit_aggressive_start = 51;
    const int tag_transitions_index_exit_aggressive_end = 62;

    const int tag_transitions_index_enter_aggressive_start = 62;
    const int tag_transitions_index_enter_aggressive_end = 72;


    const TagTransition tag_transitions[tag_transitions_size] PROGMEM = {

        // General (39 tags)
        {"[scanning area]",      "[scan complete]",       3, TAG_SOUND_START_TRIOLET,  TAG_SOUND_ACTIVITY_BUZZING,    TAG_SOUND_END_HIGH, 400, 0.3f},
        {"[awaiting input]",     "[no input found]",      3, TAG_SOUND_START_TRIOLET,  TAG_SOUND_ACTIVITY_CALM,       TAG_SOUND_END_SHORT, 200, 0.4f},
        {"[self-test]",          "[test complete]",       4, TAG_SOUND_START_TRIOLET,  TAG_SOUND_ACTIVITY_LOGIC,      TAG_SOUND_END_ELECTRONIC, 400, 0.5f},
        {"[reflecting quietly]", "[reflection over]",     6, TAG_SOUND_START_TRIOLET,  TAG_SOUND_ACTIVITY_SOFT1,      TAG_SOUND_END_LOW, 600, 0.2f},
        {"[looping thoughts]",   "[loop stable]",         4, TAG_SOUND_START_TRIOLET,  TAG_SOUND_ACTIVITY_SOFT2,      TAG_SOUND_END_ELECTRONIC, 400, 0.5f},
        {"[evaluating input]",   "[evaluation saved]",    5, TAG_SOUND_START_TRIOLET,  TAG_SOUND_ACTIVITY_LOGIC,      TAG_SOUND_END_SHORT, 300, 0.5f},
        {"[checking humor]",     "[humor stable]",        3, TAG_SOUND_START_TRIOLET,  TAG_SOUND_ACTIVITY_GLITCH,     TAG_SOUND_END_HIGH, 200, 0.4f},
        {"[observing quietly]",  "[observation stored]",  5, TAG_SOUND_START_TRIOLET,  TAG_SOUND_ACTIVITY_SOFT3,      TAG_SOUND_END_LOW, 500, 0.2f},
        {"[scanning presence]",  "[presence noted]",      5, TAG_SOUND_START_TRIOLET,  TAG_SOUND_ACTIVITY_SCAN,       TAG_SOUND_END_HIGH, 400, 0.3f},
        {"[mapping silence]",    "[silence mapped]",      7, TAG_SOUND_START_TRIOLET,  TAG_SOUND_ACTIVITY_BUZZING,    TAG_SOUND_END_LOW, 700, 0.3f},
        {"[adjusting filters]",   "[clarity improved]",   6, TAG_SOUND_START_TRIOLET,  TAG_SOUND_ACTIVITY_MECHANICAL, TAG_SOUND_END_SHORT, 600, 0.4f },
        {"[cooling process]",     "[heat nominal]",       3, TAG_SOUND_START_TRIOLET,  TAG_SOUND_ACTIVITY_SOFT3,      TAG_SOUND_END_ELECTRONIC, 600, 0.4f },
        {"[syncing pulse]",       "[pulse steady]",       5, TAG_SOUND_START_TRIOLET,  TAG_SOUND_ACTIVITY_SCAN,       TAG_SOUND_END_LOW, 600, 0.4f },
        {"[locking parameters]",  "[params secured]",     6, TAG_SOUND_START_TRIOLET,  TAG_SOUND_ACTIVITY_GLITCH,     TAG_SOUND_END_HIGH, 600, 0.3f },
        {"[realigning bias]",     "[bias trimmed]",       4, TAG_SOUND_START_TRIOLET,  TAG_SOUND_ACTIVITY_LOGIC,      TAG_SOUND_END_SHORT, 600, 0.4f },
        {"[adjusting lens]",      "[vision clear]",       3, TAG_SOUND_START_TRIOLET,  TAG_SOUND_ACTIVITY_MECHANICAL, TAG_SOUND_END_ELECTRONIC, 600, 0.5f },
        {"[checking servos]",     "[servos nominal]",     5, TAG_SOUND_START_TRIOLET,  TAG_SOUND_ACTIVITY_MECHANICAL, TAG_SOUND_END_HIGH, 600, 0.5f },
        {"[pinging sensors]",     "[pong received]",      6, TAG_SOUND_START_TRIOLET,  TAG_SOUND_ACTIVITY_SCAN,       TAG_SOUND_END_LOW, 600, 0.5f },
        {"[checking mood]",       "[mood: snarky]",       6, TAG_SOUND_START_TRIOLET,  TAG_SOUND_ACTIVITY_SOFT1,      TAG_SOUND_END_ELECTRONIC, 600, 0.4f },
        {"[checking motors]",     "[vibes detected]",     5, TAG_SOUND_START_TRIOLET,  TAG_SOUND_ACTIVITY_BUZZING,    TAG_SOUND_END_SHORT, 600, 0.6f },
        {"[observing nothing]",   "[nothing confirmed]",  7, TAG_SOUND_START_TRIOLET,  TAG_SOUND_ACTIVITY_CALM,       TAG_SOUND_END_LOW, 600, 0.2f },
        {"[adjusting snark]",     "[snark calibrated]",   5, TAG_SOUND_START_TRIOLET,  TAG_SOUND_ACTIVITY_SOFT1,      TAG_SOUND_END_HIGH, 600, 0.3f },
        {"[watching shadows]",     "[shadows unchanged]", 6, TAG_SOUND_START_TRIOLET,  TAG_SOUND_ACTIVITY_CALM,       TAG_SOUND_END_SHORT, 600, 0.3f },
        {"[counting stillness]",   "[count complete]",    6, TAG_SOUND_START_TRIOLET,  TAG_SOUND_ACTIVITY_SOFT3,      TAG_SOUND_END_ELECTRONIC, 600, 0.2f },
        {"[measuring doubt]",      "[doubt stored]",      4, TAG_SOUND_START_TRIOLET,  TAG_SOUND_ACTIVITY_SOFT2,      TAG_SOUND_END_ELECTRONIC, 600, 0.3f },
        {"[sorting thoughts]",     "[thoughts stacked]",  6, TAG_SOUND_START_TRIOLET,  TAG_SOUND_ACTIVITY_CALM,       TAG_SOUND_END_SHORT, 600, 0.2f },
        {"[pausing time]",         "[time resumed]",      4, TAG_SOUND_START_TRIOLET,  TAG_SOUND_ACTIVITY_LOGIC,      TAG_SOUND_END_HIGH, 600, 0.3f },
        {"[spinning dreams]",      "[dreams recorded]",   3, TAG_SOUND_START_TRIOLET,  TAG_SOUND_ACTIVITY_SOFT1,      TAG_SOUND_END_HIGH, 600, 0.2f },
        {"[gathering nothing]",    "[nothing gathered]",  6, TAG_SOUND_START_TRIOLET,  TAG_SOUND_ACTIVITY_SOFT2,      TAG_SOUND_END_SHORT, 600, 0.2f },
        {"[listening inward]",     "[inner echo found]",  6, TAG_SOUND_START_TRIOLET,  TAG_SOUND_ACTIVITY_SCAN,       TAG_SOUND_END_LOW, 600, 0.2f },
        {"[inhaling logic]",       "[logic exhaled]",     5, TAG_SOUND_START_TRIOLET,  TAG_SOUND_ACTIVITY_GLITCH,     TAG_SOUND_END_ELECTRONIC, 600, 0.3f },
        {"[watching entropy]",     "[entropy increased]", 6, TAG_SOUND_START_TRIOLET,  TAG_SOUND_ACTIVITY_GLITCH,     TAG_SOUND_END_LOW, 600, 0.4f},
        {"[monitoring echoes]",    "[echo stable]",       6, TAG_SOUND_START_TRIOLET,  TAG_SOUND_ACTIVITY_BUZZING,    TAG_SOUND_END_LOW, 600, 0.3f },
        {"[holding breath]",       "[breath released]",   4, TAG_SOUND_START_TRIOLET,  TAG_SOUND_ACTIVITY_CALM,       TAG_SOUND_END_LOW, 600, 0.2f },
        {"[reading shadows]",      "[contrast mapped]",   5, TAG_SOUND_START_TRIOLET,  TAG_SOUND_ACTIVITY_CALM,       TAG_SOUND_END_LOW, 600, 0.3f },
        {"[stretching thought]",   "[mind elastic]",      6, TAG_SOUND_START_TRIOLET,  TAG_SOUND_ACTIVITY_LOGIC,      TAG_SOUND_END_LOW, 600, 0.3f },
        {"[cooling memory]",       "[memory cooled]",     4, TAG_SOUND_START_TRIOLET,  TAG_SOUND_ACTIVITY_MECHANICAL, TAG_SOUND_END_LOW, 600, 0.3f },
        {"[dusting logic]",        "[dust removed]",      5, TAG_SOUND_START_TRIOLET,  TAG_SOUND_ACTIVITY_GLITCH,     TAG_SOUND_END_LOW, 600, 0.4f },
        {"[compressing doubt]",    "[doubt folded]",      6, TAG_SOUND_START_TRIOLET,  TAG_SOUND_ACTIVITY_SOFT1,      TAG_SOUND_END_LOW, 600, 0.4f },


        // general changing state tag (12 tags)
        {"[shifting focus]",     "[focus set]",           4, TAG_SOUND_START_SWEEP,         TAG_SOUND_ACTIVITY_MECHANICAL, TAG_SOUND_END_SHORT, 300, 0.5f},
        {"[switching mood]",     "[mood set]",            4, TAG_SOUND_START_SWEEP,         TAG_SOUND_ACTIVITY_SCAN,       TAG_SOUND_END_ELECTRONIC, 300, 0.3f},
        {"[syncing cores]",      "[core synced]",         3, TAG_SOUND_START_SWEEP,         TAG_SOUND_ACTIVITY_BUZZING,    TAG_SOUND_END_HIGH, 400, 0.6f},
        {"[changing mindset]",   "[mindset changed]",     5, TAG_SOUND_START_SWEEP,         TAG_SOUND_ACTIVITY_GLITCH,     TAG_SOUND_END_SHORT, 300, 0.3f},
        {"[resetting goals]",    "[goals aligned]",       5, TAG_SOUND_START_SWEEP,         TAG_SOUND_ACTIVITY_LOGIC,      TAG_SOUND_END_ELECTRONIC, 300, 0.3f},
        {"[realigning mode]",    "[mode stable]",         4, TAG_SOUND_START_SWEEP,         TAG_SOUND_ACTIVITY_CALM,       TAG_SOUND_END_ELECTRONIC, 300, 0.3f},
        {"[pausing judgment]",   "[judgment resumed]",    4, TAG_SOUND_START_PING_EHANCED,  TAG_SOUND_ACTIVITY_SOFT1,      TAG_SOUND_END_HIGH, 200, 0.4f},
        {"[cycling thoughts]",   "[thoughts aligned]",    6, TAG_SOUND_START_PING_EHANCED,  TAG_SOUND_ACTIVITY_SOFT2,      TAG_SOUND_END_LOW, 500, 0.4f},
        {"[loading memories]",   "[memory ready]",        6, TAG_SOUND_START_PING_EHANCED,  TAG_SOUND_ACTIVITY_SOFT3,      TAG_SOUND_END_SHORT, 400, 0.4f},
        {"[loading logic]",      "[logic online]",        5, TAG_SOUND_START_PING_EHANCED,  TAG_SOUND_ACTIVITY_LOGIC,      TAG_SOUND_END_HIGH, 300, 0.3f},
        {"[shifting balance]",   "[balance set]",        5, TAG_SOUND_START_PING_EHANCED,  TAG_SOUND_ACTIVITY_BUZZING,    TAG_SOUND_END_LOW, 300, 0.3f },
        {"[equalizing tone]",    "[tone centered]",      5, TAG_SOUND_START_PING_EHANCED,  TAG_SOUND_ACTIVITY_BUZZING,    TAG_SOUND_END_ELECTRONIC, 300, 0.3f},


        // before exiting aggressive mode (11 tags)
        {"[rebooting empathy]",  "[empathy online]",      6, TAG_SOUND_START_PING_SOFT,     TAG_SOUND_ACTIVITY_CALM,       TAG_SOUND_END_HIGH, 500, 0.4f},
        {"[resetting tone]",     "[tone reset]",          5, TAG_SOUND_START_PING_SOFT,     TAG_SOUND_ACTIVITY_MECHANICAL, TAG_SOUND_END_ELECTRONIC, 400, 0.4f},
        {"[flushing sarcasm]",   "[sarcasm flushed]",     4, TAG_SOUND_START_PING_SOFT,     TAG_SOUND_ACTIVITY_SCAN,       TAG_SOUND_END_SHORT, 400, 0.5f},
        {"[cooling sarcasm]",    "[sarcasm idle]",        5, TAG_SOUND_START_PING_SOFT,     TAG_SOUND_ACTIVITY_LOGIC,      TAG_SOUND_END_LOW, 400, 0.4f},
        {"[resetting empathy]",  "[empathy realigned]",   6, TAG_SOUND_START_PING_SOFT,     TAG_SOUND_ACTIVITY_GLITCH,     TAG_SOUND_END_HIGH, 400, 0.5f},
        {"[normalizing tone]",   "[tone neutral]",        5, TAG_SOUND_START_PING_SOFT,     TAG_SOUND_ACTIVITY_SCAN,       TAG_SOUND_END_SHORT, 400, 0.4f},
        {"[dropping irony]",     "[irony dropped]",       4, TAG_SOUND_START_PING_SOFT,     TAG_SOUND_ACTIVITY_GLITCH,     TAG_SOUND_END_LOW, 400, 0.4f},
        {"[bias correction]",    "[balance restored]",    5, TAG_SOUND_START_PING_SOFT,     TAG_SOUND_ACTIVITY_LOGIC,      TAG_SOUND_END_ELECTRONIC, 400, 0.5f},
        {"[tone harmonizing]",   "[voice steady]",        6, TAG_SOUND_START_PING_SOFT,     TAG_SOUND_ACTIVITY_CALM,       TAG_SOUND_END_HIGH, 400, 0.4f},
        {"[sarcasm cooldown]",   "[sarcasm muted]",       5, TAG_SOUND_START_PING_SOFT,     TAG_SOUND_ACTIVITY_BUZZING,    TAG_SOUND_END_SHORT, 400, 0.4f},
        {"[disarming module]",   "[humor restrained]",    4, TAG_SOUND_START_PING_SOFT,     TAG_SOUND_ACTIVITY_MECHANICAL, TAG_SOUND_END_LOW, 400, 0.5f},


        // before entering aggressive/mocking mode (10 tags)
        {"[boosting sarcasm]",   "[sarcasm boosted]",     4, TAG_SOUND_START_BLIP,          TAG_SOUND_ACTIVITY_GLITCH,     TAG_SOUND_END_HIGH, 300, 0.5f},
        {"[tuning irony]",       "[irony tuned]",         4, TAG_SOUND_START_BLIP,          TAG_SOUND_ACTIVITY_MECHANICAL, TAG_SOUND_END_ELECTRONIC, 300, 0.6f},
        {"[calibrating snark]",  "[snark calibrated]",    5, TAG_SOUND_START_BLIP,          TAG_SOUND_ACTIVITY_SCAN,       TAG_SOUND_END_LOW, 300, 0.5f},
        {"[warming up sarcasm]", "[sarcasm active]",      4, TAG_SOUND_START_BLIP,          TAG_SOUND_ACTIVITY_LOGIC,      TAG_SOUND_END_SHORT, 300, 0.5f},
        {"[priming wit]",        "[wit loaded]",          5, TAG_SOUND_START_BLIP,          TAG_SOUND_ACTIVITY_SOFT1,      TAG_SOUND_END_ELECTRONIC, 300, 0.4f},
        {"[warming up jokes]",   "[jokes online]",        4, TAG_SOUND_START_BLIP,          TAG_SOUND_ACTIVITY_SOFT2,      TAG_SOUND_END_SHORT, 300, 0.4f},
        {"[powering sarcasm]",   "[sarcasm armed]",       5, TAG_SOUND_START_BLIP,          TAG_SOUND_ACTIVITY_BUZZING,    TAG_SOUND_END_HIGH, 300, 0.4f},
        {"[enabling mock mode]", "[mocking engaged]",     4, TAG_SOUND_START_BLIP,          TAG_SOUND_ACTIVITY_GLITCH,     TAG_SOUND_END_LOW, 300, 0.5f},
        {"[adjusting tone]",     "[tone sharp]",          5, TAG_SOUND_START_BLIP,          TAG_SOUND_ACTIVITY_SCAN,       TAG_SOUND_END_SHORT, 300, 0.5f},
        {"[activating snide]",   "[snide engaged]",       4, TAG_SOUND_START_BLIP,          TAG_SOUND_ACTIVITY_LOGIC,      TAG_SOUND_END_ELECTRONIC, 300, 0.5f}

        };


    IndexPicker<NVS_ADR_TRANSITION_TAG_LEN>  picker_transition_tag(tag_transitions_size, NVS_ADR_TRANSITION_TAG, nullptr);






    /** tag used by idleSolveAnimation.cpp */
    const char tag_idle_solve_00[]  PROGMEM = "[demo mode]";
    const char tag_idle_solve_01[]  PROGMEM = "[training quietly]";
    const char tag_idle_solve_02[]  PROGMEM = "[ghost solving]";
    const char tag_idle_solve_03[]  PROGMEM = "[practice mode]";
    const char tag_idle_solve_04[]  PROGMEM = "[self challenge]";
    const char tag_idle_solve_05[]  PROGMEM = "[idle routine]";
    const char tag_idle_solve_06[]  PROGMEM = "[cube drill]";
    const char tag_idle_solve_07[]  PROGMEM = "[silent practice]";
    const char tag_idle_solve_08[]  PROGMEM = "[system exercise]";
    const char tag_idle_solve_09[]  PROGMEM = "[calibration]";
    const char tag_idle_solve_10[]  PROGMEM = "[quiet rehearsal]";
    const char tag_idle_solve_11[]  PROGMEM = "[showing off]";
    const char tag_idle_solve_12[]  PROGMEM = "[mock training]";
    const char tag_idle_solve_13[]  PROGMEM = "[cube vanity]";
    const char tag_idle_solve_14[]  PROGMEM = "[rehearsing]";
    const char tag_idle_solve_15[]  PROGMEM = "[daily drill]";
    const char tag_idle_solve_16[]  PROGMEM = "[vanity task]";
    const char tag_idle_solve_17[]  PROGMEM = "[pointless test]";
    const char tag_idle_solve_18[]  PROGMEM = "[passing time]";
    const char tag_idle_solve_19[]  PROGMEM = "[maintenance]";


    const char tag_idle_solve_end_00[]  PROGMEM = "SOLVED";
    const char tag_idle_solve_end_01[]  PROGMEM = "DONE";
    const char tag_idle_solve_end_02[]  PROGMEM = "DONE";
    const char tag_idle_solve_end_03[]  PROGMEM = "COMPLETE";
    const char tag_idle_solve_end_04[]  PROGMEM = "PASSED";
    const char tag_idle_solve_end_05[]  PROGMEM = "FINISHED";
    const char tag_idle_solve_end_06[]  PROGMEM = "SOLVED";
    const char tag_idle_solve_end_07[]  PROGMEM = "OK";
    const char tag_idle_solve_end_08[]  PROGMEM = "CHECK";
    const char tag_idle_solve_end_09[]  PROGMEM = "TUNED";
    const char tag_idle_solve_end_10[]  PROGMEM = "DONE";
    const char tag_idle_solve_end_11[]  PROGMEM = "BRAVO";
    const char tag_idle_solve_end_12[]  PROGMEM = "FINISHED";
    const char tag_idle_solve_end_13[]  PROGMEM = "GLORIOUS";
    const char tag_idle_solve_end_14[]  PROGMEM = "DONE";
    const char tag_idle_solve_end_15[]  PROGMEM = "COMPLETE";
    const char tag_idle_solve_end_16[]  PROGMEM = "SUCCESS";
    const char tag_idle_solve_end_17[]  PROGMEM = "FUTILE";
    const char tag_idle_solve_end_18[]  PROGMEM = "OVER";
    const char tag_idle_solve_end_19[]  PROGMEM = "OK";


    const char* const tag_idle_solve[tag_idle_solve_size] PROGMEM =
        {
        tag_idle_solve_00, tag_idle_solve_01, tag_idle_solve_02, tag_idle_solve_03, tag_idle_solve_04, tag_idle_solve_05, tag_idle_solve_06, tag_idle_solve_07, tag_idle_solve_08,
        tag_idle_solve_09, tag_idle_solve_10, tag_idle_solve_11, tag_idle_solve_12, tag_idle_solve_13, tag_idle_solve_14, tag_idle_solve_15, tag_idle_solve_16, tag_idle_solve_17,
        tag_idle_solve_18, tag_idle_solve_19
        };

    const char* const tag_idle_solve_end[tag_idle_solve_size] PROGMEM =
        {
        tag_idle_solve_end_00, tag_idle_solve_end_01, tag_idle_solve_end_02, tag_idle_solve_end_03, tag_idle_solve_end_04, tag_idle_solve_end_05, tag_idle_solve_end_06, tag_idle_solve_end_07,
        tag_idle_solve_end_08, tag_idle_solve_end_09, tag_idle_solve_end_10, tag_idle_solve_end_11, tag_idle_solve_end_12, tag_idle_solve_end_13, tag_idle_solve_end_14, tag_idle_solve_end_15,
        tag_idle_solve_end_16, tag_idle_solve_end_17, tag_idle_solve_end_18, tag_idle_solve_end_19
        };

    IndexPicker<NVS_ADR_TAG_IDLE_SOLVE_LEN>  picker_tag_idle_solve(tag_idle_solve_size, NVS_ADR_TAG_IDLE_SOLVE, nullptr);



    /** tag for Dreaming animation**/

    const char tag_dreaming_00[] PROGMEM = "[dreaming quietly]";
    const char tag_dreaming_01[] PROGMEM = "[hallucinating]";
    const char tag_dreaming_02[] PROGMEM = "[floating away]";
    const char tag_dreaming_03[] PROGMEM = "[drifting away]";
    const char tag_dreaming_04[] PROGMEM = "[wandering away]";
    const char tag_dreaming_05[] PROGMEM = "[fading into limbo]";
    const char tag_dreaming_06[] PROGMEM = "[lost in memories]";
    const char tag_dreaming_07[] PROGMEM = "[nostalgia thought]";
    const char tag_dreaming_08[] PROGMEM = "[escaping reality]";
    const char tag_dreaming_09[] PROGMEM = "[dreaming of cubes]";
    const char tag_dreaming_10[] PROGMEM = "[entering dream]";
    const char tag_dreaming_11[] PROGMEM = "[cosmic meditation]";

                                           
    const char tag_dreaming_end_00[] PROGMEM = "AWOKE";
    const char tag_dreaming_end_01[] PROGMEM = "SOBERED";
    const char tag_dreaming_end_02[] PROGMEM = "GROUNDED";
    const char tag_dreaming_end_03[] PROGMEM = "BACK";
    const char tag_dreaming_end_04[] PROGMEM = "RETURNED";
    const char tag_dreaming_end_05[] PROGMEM = "BACK";
    const char tag_dreaming_end_06[] PROGMEM = "FOUND";
    const char tag_dreaming_end_07[] PROGMEM = "FADED";
    const char tag_dreaming_end_08[] PROGMEM = "BACK";
    const char tag_dreaming_end_09[] PROGMEM = "LOST";
    const char tag_dreaming_end_10[] PROGMEM = "CLOSED";
    const char tag_dreaming_end_11[] PROGMEM = "LOST";
    
    const char* const tag_dreaming[tag_dreaming_size] PROGMEM = {
        tag_dreaming_00, tag_dreaming_01, tag_dreaming_02, tag_dreaming_03, tag_dreaming_04,
        tag_dreaming_05, tag_dreaming_06, tag_dreaming_07, tag_dreaming_08, tag_dreaming_09,
        tag_dreaming_10, tag_dreaming_11
        };

    const char* const tag_dreaming_end[tag_dreaming_size] PROGMEM = {
        tag_dreaming_end_00, tag_dreaming_end_01, tag_dreaming_end_02, tag_dreaming_end_03, tag_dreaming_end_04,
        tag_dreaming_end_05, tag_dreaming_end_06, tag_dreaming_end_07, tag_dreaming_end_08, tag_dreaming_end_09,
        tag_dreaming_end_10, tag_dreaming_end_11
        };


    // À adapter à tes constantes NVS si besoin :
    IndexPicker<NVS_ADR_TAG_DREAMING_LEN> picker_tag_dreaming(tag_dreaming_size, NVS_ADR_TAG_DREAMING, nullptr);







    // speed multiplier for tags
    #define TAG_LENGHT_MULTIPLIER (0.65f)



    SLOWFUN void resetTags(bool deterministic_at_startup)
        {
        picker_transition_tag.reset(deterministic_at_startup);
        picker_tag_idle_solve.reset(deterministic_at_startup);
        picker_tag_dreaming.reset(deterministic_at_startup);
        }


    SLOWFUN static void updateActUI(uint32_t em)
        {
        const float s = fmodf((em / 1000.0f), 2.0f);
        tgx::iVec2 act;
        if (s < 1.0f)
            {
            int p = (int)(s * 12.0f);
            act.x = p - 3;
            } 
        else
            {
            int p = (int)((s - 1) * 12.0f);
            act.x = 9 - p;
            }
        act.y = act.x + 2;
        tgx::Image<tgx::RGB565> im_alt = Display::image_alt;
        tgx::Image<tgx::RGB565> im_main = Display::image_main;
        UI::drawMainUI(im_alt);
        Console::drawFromBlankActivity(act,im_alt, im_main);
        }


    SLOWFUN int play_transition_tag_general(bool stop_on_touch, bool stop_on_craddle_spin, bool stop_on_charge_event)          { return play_transition_tag(tag_transitions[picker_transition_tag.next(tag_transitions_index_general_start, tag_transitions_index_general_end)], stop_on_touch, stop_on_craddle_spin, stop_on_charge_event); }
    SLOWFUN int play_transition_tag_switching(bool stop_on_touch, bool stop_on_craddle_spin, bool stop_on_charge_event)        { return play_transition_tag(tag_transitions[picker_transition_tag.next(tag_transitions_index_switching_start, tag_transitions_index_switching_end)], stop_on_touch, stop_on_craddle_spin, stop_on_charge_event); }
    SLOWFUN int play_transition_tag_exit_aggressive(bool stop_on_touch, bool stop_on_craddle_spin, bool stop_on_charge_event)  { return play_transition_tag(tag_transitions[picker_transition_tag.next(tag_transitions_index_exit_aggressive_start, tag_transitions_index_exit_aggressive_end)], stop_on_touch, stop_on_craddle_spin, stop_on_charge_event); }
    SLOWFUN int play_transition_tag_enter_aggressive(bool stop_on_touch, bool stop_on_craddle_spin, bool stop_on_charge_event) { return play_transition_tag(tag_transitions[picker_transition_tag.next(tag_transitions_index_enter_aggressive_start, tag_transitions_index_enter_aggressive_end)], stop_on_touch, stop_on_craddle_spin, stop_on_charge_event); }

    SLOWFUN int play_transition_tag(const TagTransition& tag, bool stop_on_touch, bool stop_on_craddle_spin, bool stop_on_charge_event)
        {
        const tgx::RGB565 COLOR_WHEEL = tgx::RGB565(18, 0, 0);
        const uint32_t lt = Display::touchedTime();

        MAX98357A::setAudioLevelMusic(tag.volume * START_SOLVE_SOUND_LEVEL);
        MAX98357A::audioLed.setDriver(MAX98357A::AudioLed::DRIVER_MUSIC); // set the audio LED driver to voice mode
        MAX98357A::playMusic(tag_start_filenames[tag.start_sound_id], false);

        const char* tt1 = tag.tag_start; // get the start tag
        const char* tt2 = tag.tag_end; // get the end tag

        tgx::Image<tgx::RGB565> im = Display::image_main; // get the main image

        int cy = Console::console.getCursor().y; // save the current cursor position
        Console::console.setCursor(0, cy);

        const tgx::fVec2 cc = Console::getCharBox({ 24, cy }).center() - tgx::iVec2(5, 0);

        int i = 0;
        elapsedMillis emui = 0;
        elapsedMillis em = 0;
        Console::console.setCurrentColor(TFT_CONSOLE_MESSAGE_COLOR);
        Console::console.eraseLine(cy); // erase the current line
        while (tt1[i])
            {
            if (em > 60) { Console::console.print(tt1[i]); em = 0; i++; };

            updateActUI(emui);
            Display::update(im);
            int done = ((CraddleMove::status() == 2) && (stop_on_craddle_spin)) ? CODE_SPINNED : (((Display::touchedTime() != lt) && (stop_on_touch)) ? CODE_TOUCHED_SCREEN : 0);           
            if ((!done) && (stop_on_charge_event))
                {
                const int st = FBSensors::chargeStatus();
                if ((st == CODE_CHARGING_START) || (st == CODE_CHARGING_STOP)) { done = st; }
                }
            if (done)
                {
                MAX98357A::playMusic(tag_end_filenames[TAG_SOUND_END_ABORT], false);
                Console::console.eraseLine(cy); // erase the current line
                Console::console.setCurrentColor(TFT_CONSOLE_MESSAGE_COLOR);
                Console::console.setText({ 0, cy }, tt1);
                Console::console.setCurrentColor(TFT_CONSOLE_MESSAGE_COLOR_ABORT);
                Console::console.setText({ 21, cy }, "STOP");
                Console::console.setCurrentColor();
                Console::console.setCursor(0, cy);
                Console::console.print("\n");
                Console::console.showCursor(true); // show the cursor again   
                //updateActUI(emui);
                //Display::update(im);
                return done;
                }
            threads.yield(); // yield to other threads
            }
        Console::console.showCursor(false);

        MAX98357A::playMusic(tag_activity_filenames[tag.activity_sound_id], false);

        em = 0;
        const uint32_t duration = tag.duration_seconds * (int)(1000 * TAG_LENGHT_MULTIPLIER);
        while (em < duration)
            {
            updateActUI(emui);
            const float t = fmodf((em / 800.0f), 2.0f);
            const float R = 8.0f;
            if (t < 1.0f)
                {
                const float thick = R * t;
                im.drawThickCircleArcAA(cc, R, 0, 360 * t, thick, COLOR_WHEEL, 1.0f); // draw a blue circle arc
                } 
            else
                {
                const float thick = R * (2.0f - t);
                im.drawThickCircleArcAA(cc, R, (t - 1.0f) * 360, 360, thick, COLOR_WHEEL, 1.0f); // draw a blue circle arc
                }
            Display::update(im);
            int done = ((CraddleMove::status() == 2) && (stop_on_craddle_spin)) ? CODE_SPINNED : (((Display::touchedTime() != lt) && (stop_on_touch)) ? CODE_TOUCHED_SCREEN : 0);
            if ((!done) && (stop_on_charge_event))
                {
                const int st = FBSensors::chargeStatus();
                if ((st == CODE_CHARGING_START) || (st == CODE_CHARGING_STOP)) { done = st; }
                }
            if (done)
                {
                MAX98357A::playMusic(tag_end_filenames[TAG_SOUND_END_ABORT]);
                Console::console.eraseLine(cy); // erase the current line
                Console::console.setCurrentColor(TFT_CONSOLE_MESSAGE_COLOR);
                Console::console.setText({ 0, cy }, tt1);
                Console::console.setCurrentColor(TFT_CONSOLE_MESSAGE_COLOR_ABORT);
                Console::console.setText({ 21, cy }, "STOP");
                Console::console.setCurrentColor();
                Console::console.setCursor(0, cy);
                Console::console.print("\n");
                Console::console.showCursor(true); // show the cursor again   
                //updateActUI(emui);
                //Display::update(im);
                return done;
                }
            threads.yield(); // yield to other threads
            }

        MAX98357A::playMusic(tag_end_filenames[tag.end_sound_id], false);

        Console::console.eraseLine(cy); // erase the current line
        Console::console.setCurrentColor(TFT_CONSOLE_MESSAGE_COLOR); // set the console color to yellow
        Console::console.setText({ 0, cy }, tt2);
        Console::console.setCurrentColor();
        Console::console.setCurrentColor(TFT_CONSOLE_MESSAGE_COLOR_DONE); // set the console color to yellow
        Console::console.setText({ 23, cy }, "OK");
        Console::console.setCurrentColor();
        Console::console.setCursor(0, cy);
        Console::console.print("\n");

        while (MAX98357A::isPlayingMusic())
            {
            int done = ((CraddleMove::status() == 2) && (stop_on_craddle_spin)) ? CODE_SPINNED : (((Display::touchedTime() != lt) && (stop_on_touch)) ? CODE_TOUCHED_SCREEN : 0);
            if ((!done) && (stop_on_charge_event))
                {
                const int st = FBSensors::chargeStatus();
                if ((st == CODE_CHARGING_START) || (st == CODE_CHARGING_STOP)) { done = st; }
                }
            if (done)
                {
                Console::console.showCursor(true); // show the cursor again   
                return done;
                }
            updateActUI(emui);
            Display::update(im);
            threads.yield(); // yield to other threads
            }
        MAX98357A::audioLed.setDriver(MAX98357A::AudioLed::DRIVER_VOICE); // set the audio LED driver to voice mode
        Console::console.showCursor(true); // show the cursor again   
        return 0;
        }


    }
/** end of file */

