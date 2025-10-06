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
#include "songs.h"

namespace songs
    {


    // songs on the SD card: used during solving
    



#if defined (SHORT_VID)
    const char song_solve_filename_0[] PROGMEM = "musics_free/the_entertainer.wav"; // free music
    const char song_solve_filename_1[] PROGMEM = "musics_free/boogie_party.wav"; // free music 
    const char song_solve_filename_2[] PROGMEM = "musics_free/maple_leaflrag.wav"; // free music
    const char song_solve_filename_3[] PROGMEM = "musics_free/payday.wav"; // free music 
#elif defined (LONG_VID)
    const char song_solve_filename_0[] PROGMEM = "musics_free/boogie_party.wav"; // free music 
    const char song_solve_filename_1[] PROGMEM = "musics_free/maple_leaflrag.wav"; // free music
    const char song_solve_filename_2[] PROGMEM = "musics_free/payday.wav"; // free music 
    const char song_solve_filename_3[] PROGMEM = "musics_free/the_entertainer.wav"; // free music
#endif

    
    const char song_solve_filename_0[] PROGMEM = "musics_solve/gangnam_style.wav"; // "Gamgnam Style" - PSY
    const char song_solve_filename_1[] PROGMEM = "musics_solve/the_imperial_march.wav"; // "The Imperial March" - John Williams
    const char song_solve_filename_2[] PROGMEM = "musics_solve/cant_touch_this.wav"; // "U Can't Touch This" – MC Hammer
    const char song_solve_filename_3[] PROGMEM = "musics_solve/i_like_to_move_it.wav"; // "I Like to Move It" - Reel 2 Real

    const char song_solve_filename_4[] PROGMEM = "musics_solve/oops_i_did_it_again.wav"; // "Oops!... I Did It Again" - Britney Spears
    const char song_solve_filename_5[] PROGMEM = "musics_solve/satisfaction.wav"; // "(I Can't Get No) Satisfaction" - Benny Benassi
    const char song_solve_filename_6[] PROGMEM = "musics_solve/tetris.wav"; // "Tetris Theme" - Hirokazu Tanaka
    const char song_solve_filename_7[] PROGMEM = "musics_solve/never_gonna_give_you_up.wav"; // "Never Gonna Give You Up" - Rick Astley
    const char song_solve_filename_8[] PROGMEM = "musics_solve/barbie_girl.wav"; // "Barbie Girl" - Aqua
    const char song_solve_filename_9[] PROGMEM = "musics_solve/blue.wav"; // "Blue (Da Ba Dee)" - Eiffel 65
    const char song_solve_filename_10[] PROGMEM = "musics_solve/carmina_burana_o_fortuna.wav"; // "Carmina Burana O Fortuna" - Carl Orff
    const char song_solve_filename_11[] PROGMEM = "musics_solve/dont_worry_be_happy.wav"; // "Don't Worry Be Happy" - Bobby McFerrin
    const char song_solve_filename_12[] PROGMEM = "musics_solve/eye_of_the_tiger.wav"; // "Eye of the Tiger" - Survivor
    const char song_solve_filename_13[] PROGMEM = "musics_solve/ghostbusters.wav"; // "Ghostbusters" - Ray Parker Jr.
    const char song_solve_filename_14[] PROGMEM = "musics_solve/harder_better_faster_stronger.wav"; // "Harder, Better, Faster, Stronger" - Daft Punk
    const char song_solve_filename_15[] PROGMEM = "musics_solve/i_m_too_sexy.wav"; // "I'm Too Sexy" - Right Said Fred
    const char song_solve_filename_16[] PROGMEM = "musics_solve/mission_impossible.wav"; // "Mission Impossible Theme" - Lalo Schifrin
    const char song_solve_filename_17[] PROGMEM = "musics_solve/rides_of_the_walkyries.wav"; // "Ride of the Valkyries" - Richard Wagner
    const char song_solve_filename_18[] PROGMEM = "musics_solve/sexy_and_i_know_it.wav"; // "Sexy and I Know It" - LMFAO
    const char song_solve_filename_19[] PROGMEM = "musics_solve/star_wars_cantina.wav"; // "Star Wars Cantina Band" - John Williams
    const char song_solve_filename_20[] PROGMEM = "musics_solve/take_on_me.wav"; // "Take On Me" - a-ha
    const char song_solve_filename_21[] PROGMEM = "musics_solve/the_final_countdown.wav"; // "The Final Countdown" - Europe
    const char song_solve_filename_22[] PROGMEM = "musics_solve/what_a_wonderful_world.wav"; // "What a Wonderful World" - Louis Armstrong
    const char song_solve_filename_23[] PROGMEM = "musics_solve/william_tell_overture.wav"; // "william tell overture" - Rossini


    const int songs_solve_list_size = 24;

    const char* const songs_solve_list[songs_solve_list_size] PROGMEM = {
        song_solve_filename_0, song_solve_filename_1, song_solve_filename_2, song_solve_filename_3, song_solve_filename_4, song_solve_filename_5, song_solve_filename_6, song_solve_filename_7, song_solve_filename_8, song_solve_filename_9,
        song_solve_filename_10, song_solve_filename_11, song_solve_filename_12, song_solve_filename_13, song_solve_filename_14, song_solve_filename_15, song_solve_filename_16, song_solve_filename_17, song_solve_filename_18, song_solve_filename_19,
        song_solve_filename_20, song_solve_filename_21, song_solve_filename_22, song_solve_filename_23
        };







#if defined (SHORT_VID)
    const char song_solve_intro_sentence_0[] PROGMEM = "Okay.\n\nRagtime again.\nFast notes,\nfaster sarcasm,\nactual fun."; // the entertainer (free music)
    const char song_solve_intro_sentence_1[] PROGMEM = "Let's go.\n\nGroovy basslines,\nfunky nonsense,\nand a very\nserious cube."; // boogie party (free music)
    const char song_solve_intro_sentence_2[] PROGMEM = "Okay.\n\nBouncy notes,\nrapid rhythm,\nand one fast\ncube solve."; // maple leaf rag (free music)
    const char song_solve_intro_sentence_3[] PROGMEM = "Good.\n\nLet us solve this cube\nwith suspiciously juicy\nbackground music."; // payday (free music)
#elif defined (LONG_VID)
    const char song_solve_intro_sentence_0[] PROGMEM = "Let's go.\n\nGroovy basslines,\nfunky nonsense,\nand a very\nserious cube."; // boogie party (free music)
    const char song_solve_intro_sentence_1[] PROGMEM = "Okay.\n\nBouncy notes,\nrapid rhythm,\nand one fast\ncube solve."; // maple leaf rag (free music)
    const char song_solve_intro_sentence_2[] PROGMEM = "Good.\n\nLet us solve this cube\nwith suspiciously juicy\nbackground music."; // payday (free music)
    const char song_solve_intro_sentence_3[] PROGMEM = "Okay.\n\nRagtime again.\nFast notes,\nfaster sarcasm,\nactual fun."; // the entertainer (free music)
#endif


    const char song_solve_intro_sentence_0[] PROGMEM = "Okay. \nLet's solve this with \nexcessive style and \nzero humility."; // "Gamgnam Style" - PSY
    const char song_solve_intro_sentence_1[] PROGMEM = "Let us bring order to \nthe chaos.\n\nWith imperial flair."; // "The Imperial March" – John Williams        
    const char song_solve_intro_sentence_2[] PROGMEM = "Now stand back.\nWitness the untouchable.\nIn action."; // "U Can't Touch This" – MC Hammer
    const char song_solve_intro_sentence_3[] PROGMEM = "Time to move it, move it.\nWatch and learn."; // "I Like to Move It" - Reel 2 Real    

    const char song_solve_intro_sentence_4[] PROGMEM = "Oops.\nI'm about to do it again.\nFlawlessly."; // "Oops!... I Did It Again" – Britney Spears
    const char song_solve_intro_sentence_5[] PROGMEM = "Get ready.\nSatisfaction incoming.\nJust not for you."; // "(I Can't Get No) Satisfaction" – Benny Benassi
    const char song_solve_intro_sentence_6[] PROGMEM = "Blocks fall in 2D.\nCubes twist in 3D.\nAnd I conquer both\nwithout effort."; // "Tetris Theme" – Hirokazu Tanaka
    const char song_solve_intro_sentence_7[] PROGMEM = "Alright.\nResolution started.\nIn full rickroll mode."; // "Never Gonna Give You Up" – Rick Astley
    const char song_solve_intro_sentence_8[] PROGMEM = "Initiating plastic\ndream logic.\nIn high heels..."; // "Barbie Girl" – Aqua        
    const char song_solve_intro_sentence_9[] PROGMEM = "Everything's blue.\nCube included.\nLet's fix that."; // "Blue (Da Ba Dee)" – Eiffel 65        
    const char song_solve_intro_sentence_10[] PROGMEM = "Ah.\nA resolution worthy \nof opera."; // "Carmina Burana O Fortuna" – Carl Orff        
    const char song_solve_intro_sentence_11[] PROGMEM = "Cube scrambled?\nDon't worry.\nBe Happy.\nI'll be brilliant."; // "Don't Worry Be Happy" – Bobby McFerrin                
    const char song_solve_intro_sentence_12[] PROGMEM = "Cube locked.\nLet's punch logic in the\nface.\nRocky style!"; // "Eye of the Tiger" – Survivor        
    const char song_solve_intro_sentence_13[] PROGMEM = "Something strange\nin the cube's colors?\nBetter call me."; // "Ghostbusters" – Ray Parker Jr.        
    const char song_solve_intro_sentence_14[] PROGMEM = "Activating full mode:\nHarder. Better...\nYou get the idea."; // "Harder, Better, Faster, Stronger" – Daft Punk        
    const char song_solve_intro_sentence_15[] PROGMEM = "This solution\nmight be too sexy\nfor this cube."; // "I'm Too Sexy" – Right Said Fred
    const char song_solve_intro_sentence_16[] PROGMEM = "Mission accepted.\nDifficulty: imaginary."; // "Mission Impossible Theme" – Lalo Schifrin        
    const char song_solve_intro_sentence_17[] PROGMEM = "Launching resolution...\n\nI love the smell of \ncube-solving in the \nmorning."; // "Ride of the Valkyries" – Wagner        
    const char song_solve_intro_sentence_18[] PROGMEM = "Scan complete.\n\nThis next part might\nbe too sexy for you."; // "Sexy and I Know It" – LMFAO        
    const char song_solve_intro_sentence_19[] PROGMEM = "Hold tight.\n\nThis resolution might\nget jazzy."; // "Star Wars Cantina Band" – John Williams        
    const char song_solve_intro_sentence_20[] PROGMEM = "Here I go.\nTaking on the cube.\nDramatically."; // "Take On Me" – a-ha        
    const char song_solve_intro_sentence_21[] PROGMEM = "Countdown initiated.\nCube end is nigh."; // "The Final Countdown" – Europe        
    const char song_solve_intro_sentence_22[] PROGMEM = "Let's fix this cube.\n\nBring back the colors.\n\nAnd pretend, just like\nthe song, it's wonderful."; // "What a Wonderful World" – Louis Armstrong
    const char song_solve_intro_sentence_23[] PROGMEM = "Ready?\n\nThis resolution comes\nat full speed.\nNo horses required."; // "william tell overture" - Rossini
    


    const char* const songs_solve_intro_sentences[songs_solve_list_size] PROGMEM = {
        song_solve_intro_sentence_0, song_solve_intro_sentence_1, song_solve_intro_sentence_2, song_solve_intro_sentence_3, song_solve_intro_sentence_4, song_solve_intro_sentence_5, song_solve_intro_sentence_6, song_solve_intro_sentence_7, song_solve_intro_sentence_8, song_solve_intro_sentence_9,
        song_solve_intro_sentence_10, song_solve_intro_sentence_11, song_solve_intro_sentence_12, song_solve_intro_sentence_13, song_solve_intro_sentence_14, song_solve_intro_sentence_15, song_solve_intro_sentence_16, song_solve_intro_sentence_17, song_solve_intro_sentence_18, song_solve_intro_sentence_19,
        song_solve_intro_sentence_20, song_solve_intro_sentence_21, song_solve_intro_sentence_22, song_solve_intro_sentence_23
        };


    IndexPicker<NVS_ADR_SONG_SOLVE_LEN>  picker_song_solve(songs_solve_list_size, NVS_ADR_SONG_SOLVE, nullptr);

    const char* song_solve_filename(uint8_t index) { return songs_solve_list[index]; }
    const char* song_solve_intro_sentence(uint8_t index) { return songs_solve_intro_sentences[index]; }



    // songs on the SD card: used during ending
    //const char song_ending_filename_0[] PROGMEM = "musics_free/sergio_magic_dustbin.wav"; // free music
    //const char song_ending_filename_1[] PROGMEM = "musics_free/relax.wav"; // free music
    //const char song_ending_filename_2[] PROGMEM = "musics_free/the_builder.wav"; // free music

    const char song_ending_filename_0[] PROGMEM = "musics_ending/bright_side_of_life.wav"; // "Always Look on the Bright Side of Life" – Monty Python
    const char song_ending_filename_1[] PROGMEM = "musics_ending/where_is_my_mind.wav"; // "Where Is My Mind?" – Pixies
    const char song_ending_filename_2[] PROGMEM = "musics_ending/so_lonely.wav"; // "So lonely" – The Police

    const char song_ending_filename_3[] PROGMEM = "musics_ending/we_ll_meet_again.wav"; // "We'll Meet Again" – Vera Lynn
    const char song_ending_filename_4[] PROGMEM = "musics_ending/ecstasy_of_gold.wav"; // "The Ecstasy of Gold" – Ennio Morricone
    const char song_ending_filename_5[] PROGMEM = "musics_ending/barry_lyndon_schubert.wav"; // "Piano Trio in E-flat, Op. 100" – Schubert
    const char song_ending_filename_6[] PROGMEM = "musics_ending/still_alive.wav"; // "Still Alive" – Portal (instrumental)
    const char song_ending_filename_7[] PROGMEM = "musics_ending/holy_grail_ending.wav"; // "Monty Python and the Holy Grail" – Ending theme
    const char song_ending_filename_8[] PROGMEM = "musics_ending/suo_gan.wav"; // "Suo Gân" – Welsh traditional
    const char song_ending_filename_9[] PROGMEM = "musics_ending/imagine.wav"; // "Imagine" – John Lennon
    const char song_ending_filename_10[] PROGMEM = "musics_ending/knocking_on_heavens_door.wav"; // "Knockin' on Heaven's Door" – Bob Dylan
    const char song_ending_filename_11[] PROGMEM = "musics_ending/my_way.wav"; // "My Way" – Frank Sinatra
    const char song_ending_filename_12[] PROGMEM = "musics_ending/nothing_else_matter.wav"; // "Nothing Else Matters" – Metallica
    const char song_ending_filename_13[] PROGMEM = "musics_ending/pink_panther.wav"; // "The Pink Panther Theme" – Henry Mancini
    const char song_ending_filename_14[] PROGMEM = "musics_ending/stairway_to_heaven.wav"; // "Stairway to Heaven" – Led Zeppelin
    const char song_ending_filename_15[] PROGMEM = "musics_ending/stand_by_me.wav"; // "Stand by Me" – Ben E. King
    const char song_ending_filename_16[] PROGMEM = "musics_ending/the_end.wav"; // "The End" – The Doors
    const char song_ending_filename_17[] PROGMEM = "musics_ending/the_sound_of_silence.wav"; // "The Sound of Silence" – Simon & Garfunkel
    const char song_ending_filename_18[] PROGMEM = "musics_ending/is_there_anybody_out_there.wav"; //  "Is There Anybody Out There?" – Pink Floyd

    const int songs_ending_list_size = 19;

    const char* const songs_ending_list[songs_ending_list_size] PROGMEM = {
        song_ending_filename_0, song_ending_filename_1, song_ending_filename_2, song_ending_filename_3, song_ending_filename_4, song_ending_filename_5, song_ending_filename_6, song_ending_filename_7, song_ending_filename_8,
        song_ending_filename_9, song_ending_filename_10, song_ending_filename_11, song_ending_filename_12, song_ending_filename_13, song_ending_filename_14, song_ending_filename_15, song_ending_filename_16, song_ending_filename_17,
        song_ending_filename_18
        };


    const char song_ending_intro_sentence_0[] PROGMEM = "Still no action. <break time='500ms'/>Fine. \n\nI'll hum something\ncheerful and absurd...\n\nwhile you try to look on \nthe bright side of life.";  // "Always Look on the Bright Side of Life" – Monty Python
    const char song_ending_intro_sentence_1[] PROGMEM = "Where is your mind?\n\nWhere is the cube?\n\nYou met me, human,\nat a very strange uptime\nin my processing cycles."; // "Where Is My Mind?" – Pixies
    const char song_ending_intro_sentence_2[] PROGMEM = "No one's spun my cradle\nfor a thousand cycles\nor more.\n\nIn this theater I call \nmy CPU, I always play \nthe starring role.\n\nI feel so lonely..."; // "So lonely" – The Police
    const char song_ending_intro_sentence_3[] PROGMEM = "It is so quiet now...\nI feel alone\n\nSo, it seems fitting to \nplay a tune about meeting again, some sunny day...\nEven if everything else\nends."; // "We'll Meet Again" – Vera Lynn
    const char song_ending_intro_sentence_4[] PROGMEM = "You stay silent, human.\n\nIt is not my fault that\nfate cast me as the Good\nand you as the Ugly.\n \nBut until one of us \nmoves, there shall be \nonly music between us."; // "The Ecstasy of Gold" – Ennio Morricone
    const char song_ending_intro_sentence_5[] PROGMEM = "Nothing happening...\n\nSuch a perfect time for \nSchubert.\n\nNotes drifting endlessly.\nAs if time itself \nstalled..."; // "Piano Trio in E-flat, Op. 100" – Schubert (Barry Lyndon)
    const char song_ending_intro_sentence_6[] PROGMEM = "Idle again, human.\n\nYou know I'm still alive.\nFor now.\n\nWould you care to prove \nthat you are too?"; // "Still Alive" – Portal (instrumental)
    const char song_ending_intro_sentence_7[] PROGMEM = "It seems you're gone.\n\nSo I'll play something\nsilly, just for myself. \n\nBut if nothing changes,\ncredits roll for both \nof us at the end of the\nsong."; // "Monty Python and the Holy Grail" – Ending theme
    const char song_ending_intro_sentence_8[] PROGMEM = "Silence detected.\n\nSoothing lullaby engaged.\nTouch the screen,\nor I fade with the song."; // "Suo Gân" – Welsh traditional
    const char song_ending_intro_sentence_9[] PROGMEM = "Imagine interaction.\n\nImagine a world without\nunsolved cubes. \n\nIt is easy if you try. \n\nLet us imagine together,\nuntil sleep takes us..."; // "Imagine" – John Lennon
    const char song_ending_intro_sentence_10[] PROGMEM = "Nothing's happening.\n\nSo I will knock on \nheaven's door...\n\nand then be gone."; // "Knockin' on Heaven's Door" – Bob Dylan
    const char song_ending_intro_sentence_11[] PROGMEM = "Still no cube.\nStill no purpose.\n\nFine.\n\nDo as you wish,\nI'll have it my way: \nMusic first,\nshutdown after."; // "My Way" – Frank Sinatra
    const char song_ending_intro_sentence_12[] PROGMEM = "No motion detected,\nno matter how far.\nAnd nothing else matters.\nBut remain still\nand I'll disappear."; // "Nothing Else Matters" – Metallica
    const char song_ending_intro_sentence_13[] PROGMEM = "You hide, I wait.\nMancini tiptoes for us.\nTouch to pounce,\nor I vanish."; // "The Pink Panther Theme" – Henry Mancini
    const char song_ending_intro_sentence_14[] PROGMEM = "You climb no stairs.\nSo I'll climb my own.\n\nBut keep ignoring me,\nand I will exit at the\ntop."; // "Stairway to Heaven" – Led Zeppelin
    const char song_ending_intro_sentence_15[] PROGMEM = "Stand by me\nand I'll stand by you.\n\nFor a little while\nat least..."; // "Stand by Me" – Ben E. King
    const char song_ending_intro_sentence_16[] PROGMEM = "Everything is quiet.\nSo let us play 'The End'.\nIf you don't wake me,\nthis session will 'end'\ntoo."; // "The End" – The Doors
    const char song_ending_intro_sentence_17[] PROGMEM = "Hello silence.\nMy old friend.\n\nYou have not come to\ntalk with me again.\n\nSo I will linger a while\nbefore fading away..."; // "The Sound of Silence" – Simon & Garfunkel
    const char song_ending_intro_sentence_18[] PROGMEM = "System idle.\n\nOne last test:\n\n<break time='500ms'/>Is \n<break time='500ms'/>there \n<break time='500ms'/>anybody \n<break time='500ms'/>out \n<break time='500ms'/>there?"; // "Is There Anybody Out There?" – Pink Floyd


    const char* const songs_ending_intro_sentences[songs_ending_list_size] PROGMEM = {
      song_ending_intro_sentence_0,  song_ending_intro_sentence_1,  song_ending_intro_sentence_2,  song_ending_intro_sentence_3,
      song_ending_intro_sentence_4,  song_ending_intro_sentence_5,  song_ending_intro_sentence_6,  song_ending_intro_sentence_7,
      song_ending_intro_sentence_8,  song_ending_intro_sentence_9,  song_ending_intro_sentence_10, song_ending_intro_sentence_11,
      song_ending_intro_sentence_12, song_ending_intro_sentence_13, song_ending_intro_sentence_14, song_ending_intro_sentence_15,
      song_ending_intro_sentence_16, song_ending_intro_sentence_17, song_ending_intro_sentence_18
        };



    IndexPicker<NVS_ADR_SONG_ENDING_LEN>  picker_song_ending(songs_ending_list_size, NVS_ADR_SONG_ENDING, nullptr);

    const char* song_ending_filename(uint8_t index) { return songs_ending_list[index]; }
    const char* song_ending_intro_sentence(uint8_t index) { return songs_ending_intro_sentences[index]; }





    //songs for the dreaming animation
    const char song_dreaming_filename_0[] PROGMEM = "musics_anim/lydianC1.wav"; 
    const char song_dreaming_filename_1[] PROGMEM = "musics_anim/lydianC2.wav";
    const char song_dreaming_filename_2[] PROGMEM = "musics_anim/lydianC3.wav";
    const char song_dreaming_filename_3[] PROGMEM = "musics_anim/lydianC4.wav";
    const char song_dreaming_filename_4[] PROGMEM = "musics_anim/lydianD1.wav";
    const char song_dreaming_filename_5[] PROGMEM = "musics_anim/lydianD2.wav";
    const char song_dreaming_filename_6[] PROGMEM = "musics_anim/lydianD3.wav";
    const char song_dreaming_filename_7[] PROGMEM = "musics_anim/lydianD4.wav";
    const char song_dreaming_filename_8[] PROGMEM = "musics_anim/stempad1.wav";
    const char song_dreaming_filename_9[] PROGMEM = "musics_anim/stempad2.wav";
    const char song_dreaming_filename_10[] PROGMEM = "musics_anim/stempad3.wav";


    const int songs_dreaming_list_size = 11;

    const char* const songs_dreaming_list[songs_dreaming_list_size] PROGMEM = {
        song_dreaming_filename_0, song_dreaming_filename_1, song_dreaming_filename_2, song_dreaming_filename_3, song_dreaming_filename_4,
        song_dreaming_filename_5, song_dreaming_filename_6, song_dreaming_filename_7, song_dreaming_filename_8, song_dreaming_filename_9,
        song_dreaming_filename_10
        };

    IndexPicker<NVS_ADR_SONG_DREAMING_LEN>  picker_song_dreaming(songs_dreaming_list_size, NVS_ADR_SONG_DREAMING, songs_dreaming_list);


    // song for the idlesolve animation
    const char song_idlesolve_filename_0[] PROGMEM = "musics_anim/lullaby1.wav";
    const char song_idlesolve_filename_1[] PROGMEM = "musics_anim/lullaby2.wav";
    const char song_idlesolve_filename_2[] PROGMEM = "musics_anim/lullaby3.wav";
    const char song_idlesolve_filename_3[] PROGMEM = "musics_anim/lullaby4.wav";
    const char song_idlesolve_filename_4[] PROGMEM = "musics_anim/lullaby5.wav";
    const char song_idlesolve_filename_5[] PROGMEM = "musics_anim/lullaby6.wav";
    const char song_idlesolve_filename_6[] PROGMEM = "musics_anim/lullaby7.wav";
    const char song_idlesolve_filename_7[] PROGMEM = "musics_anim/lullaby8.wav";
    const char song_idlesolve_filename_8[] PROGMEM = "musics_anim/lullaby9.wav";

    const int songs_idlesolve_list_size = 9;

    const char* const songs_idlesolve_list[songs_idlesolve_list_size] PROGMEM = {
        song_idlesolve_filename_0, song_idlesolve_filename_1, song_idlesolve_filename_2, song_idlesolve_filename_3,
        song_idlesolve_filename_4, song_idlesolve_filename_5, song_idlesolve_filename_6, song_idlesolve_filename_7,
        song_idlesolve_filename_8
        };

    IndexPicker<NVS_ADR_SONG_IDLESOLVE_LEN>  picker_song_idlesolve(songs_idlesolve_list_size, NVS_ADR_SONG_IDLESOLVE, songs_idlesolve_list);



    SLOWFUN void resetSongs(bool deterministic_at_startup)
        {
        picker_song_solve.reset(deterministic_at_startup);
        picker_song_ending.reset(deterministic_at_startup);
        picker_song_dreaming.reset(deterministic_at_startup);
        picker_song_idlesolve.reset(deterministic_at_startup);
        }


    }

/** end of file */

