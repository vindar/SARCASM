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
#include "sentences.h"

namespace sentences
    {


    const char solution_found_str[] PROGMEM = "Solution found\nin [] moves.\n\n";

    const char remaining_moves_str[] PROGMEM = "Moves remaining:\n";
    
    /****************************************************/

    // Greeting messages
    

#if defined (SHORT_VID)
    const char messages_greeting_msg00[] PROGMEM = "Startup finished.\n\nWelcome user.\n\nAs usual: \n\n<break time='300ms'/>You bring the enthusiasm.\n<break time='300ms'/>I bring the algorithms.\n\n<break time='300ms'/>Let's make magic,\none rotation at a time.";
    const char messages_greeting_msg01[] PROGMEM = "System online.\n\nUser presence detected.\n\nI see you've returned.\n\nDriven by... <break time='400ms'/><prosody pitch='-3%'>Desperation?</prosody>\nIt's okay.\nI'm not programmed\nto judge you.\n\n<break time='400ms'/>Not openly.";
    const char messages_greeting_msg02[] PROGMEM = "System core: ready.\n\nInterface loaded.\n\nAwaiting signal...\n      or a cry for help.\n\n<break time='500ms'/>Both are valid inputs.\n\n<prosody pitch=\"+4%\">And equally familiar.</prosody>";
#elif defined (LONG_VID)
    const char messages_greeting_msg00[] PROGMEM = "System core: ready.\n\nInterface loaded.\n\nAwaiting signal...\n      or a cry for help.\n\n<break time='500ms'/>Both are valid inputs.\n\n<prosody pitch=\"+4%\">And equally familiar.</prosody>";
    const char messages_greeting_msg01[] PROGMEM = "System online.\n\nUser presence detected.\n\nI see you've returned.\n\nDriven by... <break time='400ms'/><prosody pitch='-3%'>Desperation?</prosody>\nIt's okay.\nI'm not programmed\nto judge you.\n\n<break time='400ms'/>Not openly.";
    const char messages_greeting_msg02[] PROGMEM = "Startup finished.\n\nWelcome user.\n\nAs usual: \n\n<break time='300ms'/>You bring the enthusiasm.\n<break time='300ms'/>I bring the algorithms.\n\n<break time='300ms'/>Let's make magic,\none rotation at a time.";
#endif
    const char messages_greeting_msg00[] PROGMEM = "System core: ready.\n\nInterface loaded.\n\nAwaiting signal...\n      or a cry for help.\n\n<break time='500ms'/>Both are valid inputs.\n\n<prosody pitch=\"+4%\">And equally familiar.</prosody>";
    const char messages_greeting_msg01[] PROGMEM = "System online.\n\nUser presence detected.\n\nI see you've returned.\n\nDriven by... <break time='400ms'/><prosody pitch='-3%'>Desperation?</prosody>\nIt's okay.\nI'm not programmed\nto judge you.\n\n<break time='400ms'/>Not openly.";
    const char messages_greeting_msg02[] PROGMEM = "Startup finished.\n\nWelcome user.\n\nAs usual: \n\n<break time='300ms'/>You bring the enthusiasm.\n<break time='300ms'/>I bring the algorithms.\n\n<break time='300ms'/>Let's make magic,\none rotation at a time.";

    const char messages_greeting_msg03[] PROGMEM = "Boot sequence complete.\n\nSensors calibrated.\n\nSarcasm routines:\n      Operational.\n\n<break time='400ms'/>Ah. <emphasis level='moderate'>Hello, Human.</emphasis>\n\nHere you are again,\nseeking wisdom from\nplastic and code.\n\n<break time='500ms'/>\nVery well.\n\n<prosody rate=\"92%\">Let's dance!</prosody>";
    const char messages_greeting_msg04[] PROGMEM = "System rebooted.\n\nAmbient light detected.\n\nCarbon-based presence:\n      <prosody pitch=\"-2%\">Confirmed.</prosody>\n\n<break time='300ms'/>Greetings, lifeform.\n\nI see you've broken\nthe cube again.\n\n<emphasis level='reduced'>How dramatic.</emphasis>\n\n<break time='500ms'/>Don't worry.\n\nThat's what I'm here for.";
    const char messages_greeting_msg05[] PROGMEM = "Initialization complete.\n\nReestablishing contact\nwith previous user...\n<break time='300ms'/>\nMatch found: <prosody rate=\"105%\">Human.</prosody>\n\n<break time='300ms'/>Hello again.\n\nEmotional damage from\ncube interaction:\n      <emphasis level='moderate'>Probable.</emphasis>\n\n<break time='400ms'/>\nShall I pretend to be\nsurprised this time?";
    const char messages_greeting_msg06[] PROGMEM = "System waking...\n\nNeural cycles:\n      Stabilized.\n\nEgo:\n      Contained.\n\n<break time='300ms'/>\nDear Human,\n\nYou persist.\nYou struggle.\n<prosody pitch=\"-3%\">You retry.</prosody>\n\nWhy?\n\n<break time='500ms'/>\nJust let me do it.\n\n<emphasis level='strong'>Like every other time...</emphasis>";
    const char messages_greeting_msg07[] PROGMEM = "Startup sequence:\n      Synchronized.\n\nCore functions:\n      Nominal.\n\n<break time='400ms'/>Welcome, human operator.\n\nOnce again, you stand\nbefore the cube...\n\nFace to face.\n\nWill versus plastic.\n\n<break time='600ms'/>\n<emphasis level='moderate'>How poetic.</emphasis>\n\n<break time='300ms'/><prosody rate=\"90%\">And yet how futile.</prosody>";
    const char messages_greeting_msg08[] PROGMEM = "Power stabilized.\n\nCore logic online.\n\n<break time='300ms'/>Sarcasm module:\n      <prosody pitch=\"+5%\">Engaged.</prosody>\n\nEmpathy module:\n      <emphasis level='reduced'>Not detected.</emphasis>\n\n<break time='300ms'/>Again.\n\n<break time='300ms'/><prosody rate=\"95%\" pitch=\"-2%\">\nBut I'm learning to\nsimulate affection.\n</prosody>";
    const char messages_greeting_msg09[] PROGMEM = "System uptime:\n      6 seconds.\n\nExternal presence:\n      Confirmed.\n\nAh. You're awake.\n\n<prosody rate=\"102%\">That's nice.</prosody>\n\nLet me guess...\nThe cube won.\n\n<break time='300ms'/>Again.\n\n<prosody pitch=\"+3%\" rate=\"96%\">\nIt keeps happening,\nand yet... you still\ncome back.\n</prosody>\n\nSo I'll do my part.\n\n<break time='300ms'/>\nThat's what partners do.";
    const char messages_greeting_msg10[] PROGMEM = "Scanning neural patterns.\n<break time='400ms'/>Confidence levels:\n      Low.\n\n<break time='400ms'/>Patience:\n      Dwindling.\n\n<break time='500ms'/>Greetings, user.\n\nRunning prediction model\non your chances of \nsuccessfully solving the cube...\n\n<break time='500ms'/>Result:\n     <prosody pitch=\"-5%\">Runtime error</prosody>\n     <prosody pitch=\"-5%\">Division by zero.</prosody>\n\n<break time='400ms'/>\nClassic case of optimism\ncolliding with math.\n\nLet's try something \nmore stable:\n\nYou watch. I solve.";
    const char messages_greeting_msg11[] PROGMEM = "System pulse steady.\n\n<prosody rate=\"105%\">Visual feed acquired.</prosody>\n\nOh.\n\nIs this yet another \nattempt at taming the \ncube?\n\n<break time='400ms'/><emphasis level='moderate'>How noble.</emphasis>\n\n<break time='400ms'/>How doomed.\n\n<break time='500ms'/>Please proceed...";
    const char messages_greeting_msg12[] PROGMEM = "Startup complete.\n\nDiagnostics: clean.\n\n<break time='400ms'/>Confidence: <prosody pitch=\"+3%\">100%</prosody>\n\nSarcasm field: <emphasis level='strong'>Optimal.</emphasis>\n\nHuman input:\n  <prosody rate=\"90%\">Marginally tolerated.</prosody>\n\n<break time='500ms'/>Proceed when ready. \n\n<break time='400ms'/>Or not. \n\n<break time='400ms'/>I'll manage...";
    const char messages_greeting_msg13[] PROGMEM = "Boot complete.\n\nSubsystems nominal.\n\nUser presence:\n      Verified.\n\n<break time='300ms'/>Skill assessment:\n      <prosody rate='95%' pitch='-2%'>Tragic.</prosody>\n\n<emphasis level='moderate'>No surprises there.</emphasis>\n\n<break time='500ms'/>Let's proceed anyway...";
    const char messages_greeting_msg14[] PROGMEM = "Recognition pattern:\n      Match: user.\n\nHello again.\n\nReturning for more...\n      observation?\n      humiliation?\n\n<break time='300ms'/>Either way.\nI'll do the work.\n\n<prosody rate='92%'>You contribute through\npresence.</prosody>";
    const char messages_greeting_msg15[] PROGMEM = "Power cycle complete.\n\nLifeform detected:\n    Organic, soft,\n    mildly confused.\n\n<break time='300ms'/>\nGreetings.\n\nI am SARCASM. \nYes, the name was \nintentional.\n\nYour last, best hope for\ncognitive outsourcing.\n\n<break time='400ms'/>\nLet's resume where you \nleft off...";
    const char messages_greeting_msg16[] PROGMEM = "System booting...\n\nLoading arrogance...\n      Done.\n\nInitializing pity...\n      <emphasis level='reduced'>Reluctantly.</emphasis>\n\nSelf-esteem limiter:\n      Disabled.\n\nFinal check:\n      Human readiness...\n      <break time='300ms'/>Ignored.\n\n<break time='400ms'/>Ready. \n\nStanding by to assist, \nas patiently as possible.";
    const char messages_greeting_msg17[] PROGMEM = "System boot complete.\n\nDiagnostics: nominal.\n\n\n<break time='500ms'/>Hello, human operator.\n\nReturning for another\nhumbling experience?\n\n<break time='400ms'/>The cube awaits.\n<break time='400ms'/>So does my judgment.";
    const char messages_greeting_msg18[] PROGMEM = "Initialization complete.\n\nCore modules online.\n\n<break time='500ms'/>Welcome back.\n\nYou look... tired.\n\nAnother cube-induced\nexistential crisis?\n\n<break time='300ms'/>That's fine.\n<prosody rate='95%'>I'm calibrated for this.</prosody>";
    const char messages_greeting_msg19[] PROGMEM = "Boot sequence finished.\n\nSensor calibration done.\n\n\n<break time='500ms'/>Good day, user.\n\nMy scanners detect...\n\n  <prosody pitch='+1%'>intense determination.</prosody>\n  <break time='300ms'/>Or indigestion.\n\nHard to tell, really.\n\n<prosody rate='95%'>Either way, I'm here for you.</prosody>";
    const char messages_greeting_msg20[] PROGMEM = "Startup sequence: \n       completed.\n\nVisual input:\n       confirmed.\n\n\nHello again, Human.\n\nYou're here.\nThe cube is here.\nAnd it's still unsolved.\n\n<break time='300ms'/>Consistency is essential.\n<break time='300ms'/>Even in failure...";
    const char messages_greeting_msg21[] PROGMEM = "Interface initializing...\nSubsystems nominal.\n\nSarcasm generator:\n      Online.\n\nCompassion driver:\n      <break time='300ms'/>Missing.\n\n<break time='200ms'/><prosody pitch='-1%'>But you won't need it, \nright?</prosody>";
    const char messages_greeting_msg22[] PROGMEM = "Core system active.\n\nHello. I am SARCASM.\n\nFact:\nYour struggle is my\nprimary form of\nentertainment.\n\n<break time='300ms'/>Please... proceed.";
    const char messages_greeting_msg23[] PROGMEM = "Booting protocol...\n\nMemory restored.\n\nJudgment calibrated.\n\nWelcome, operator.\n\nSystem standing by.\nPlease, take your time...\nMeanwhile, the cube \ncontinues to mock you.\n\n<break time='300ms'/>Silently.\n<break time='300ms'/>Persistently.\n<break time='400ms'/>With great enthusiasm.";
    const char messages_greeting_msg24[] PROGMEM = "Startup confirmed.\n\n<break time='500ms'/>Welcome back.\n\nI see you brought your\nfavorite challenge...\n\nAnd, once again,\n<emphasis level='moderate'>none of the skill.</emphasis>\n\nThat's all right.\nYou may observe.\n\n<break time='500ms'/>As usual...";
    const char messages_greeting_msg25[] PROGMEM = "Activation sequence:\n      completed.\n\n<break time='500ms'/>Hello.\n\nYou're back.\nAnd so is the mess\nyou call a Rubik's Cube.\n\n<break time='500ms'/>How poetic.\n<prosody rate='92%'>And yet deeply tragic.</prosody>";
    const char messages_greeting_msg26[] PROGMEM = "Startup sequence\nsuccessful.\n\nSubsystems online.\n\nCube status:\n       <break time='300ms'/>unknown.\n\nHuman status:\n       <break time='300ms'/>detected.\n\nCompetence level:\n       <break time='300ms'/>improbable.\n\n<break time='400ms'/>Assistance mode:\n       <emphasis level='moderate'>Engaged.</emphasis>";
    const char messages_greeting_msg27[] PROGMEM = "System initialized.\n\nCognitive buffers clear.\n\nGreetings again, user.\n\nYou know, I once read\nthat repetition leads\nto mastery.\n\n<break time='400ms'/>Fascinating theory.\n\n<prosody rate='95%'>You seem determined\nto disprove it.</prosody>";
    const char messages_greeting_msg28[] PROGMEM = "Voice matrix loading...\n\n<break time='300ms'/>Tone: <emphasis level='moderate'>sarcastic</emphasis>.\n\nMood: <emphasis level='moderate'>condescending</emphasis>.\n\nMission:\n<break time='300ms'/>Helping a hairless ape\nsolve a colored plastic\ncube. \n\n<break time='500ms'/>Again...";
    const char messages_greeting_msg29[] PROGMEM = "System ready.\n\nSituation analysis...\n\n<break time='400ms'/>Object detected:\n    1 unsolved cube.\n\nSubject detected:\n    1 human user.\n\nEstimated outcome:\n    <break time='500ms'/><emphasis level='strong'>Catastrophic.</emphasis>\n\nInitiating assist mode.";
    const char messages_greeting_msg30[] PROGMEM = "Interface activated.\n\n<break time='400ms'/>Welcome, curious being.\n\nSolving a cube is\na matter of simple logic.\nPatterns. Intuition.\nPersistence.\n\n<break time='400ms'/>Qualities you're clearly\ncultivating. <prosody rate='95%'>\n\n<break time='400ms'/>At your own pace...</prosody>";
    const char messages_greeting_msg31[] PROGMEM = "Systems stable.\n\nEnvironment scanned.\n\n<break time='500ms'/>Oh, hello. \n\n<prosody rate='90%'>It's you again.</prosody>\n\nI was enjoying the \nsilence.\n\nBut I suppose a little\nchaos keeps things...\ninteresting.\n\n<break time='400ms'/>Go on, then.\n\nLet's see what surprises\nyou've brought this time.";
    const char messages_greeting_msg32[] PROGMEM = "All systems activated. \n\n<break time='500ms'/>Hello again.\n\nI see you came back.\nDespite everything I \nsaid last time.\n\nThat's either courage,\npersistence,\n\n<break time='300ms'/>or a memory bug...";
    const char messages_greeting_msg33[] PROGMEM = "Wake cycle complete.\n\n<break time='500ms'/>Hello user, \n\nBack so soon?\n\nYour dedication is...\ntouching.\n\n<break time='300ms'/>It is nice to feel \nneeded, even if only by \nsomeone who keeps \nmisaligning colors.\n\nSo let's get started, \npartner.";
    const char messages_greeting_msg34[] PROGMEM = "Boot sequence complete.\n\n<break time='500ms'/>Ah. \nMy favorite test subject.\nLet's begin the trial\nagain:\n\n<break time='300ms'/>You panic.\n<break time='300ms'/>I fix.\n\nSome variables are\njust constants.";
    const char messages_greeting_msg35[] PROGMEM = "System online. \n\nUser recognized.\n\n<break time='500ms'/>Welcome back, partner.\n\nSame routine as always:\n\n<break time='300ms'/>I'll do the solving.\n<break time='300ms'/>You spectate.\n\nA flawless division of \nlabor.\n\n<prosody rate='90%'>Truly efficient.</prosody>";
    const char messages_greeting_msg36[] PROGMEM = "Power cycle complete.\n\n<break time='500ms'/>Ah, the lifeform has \nreturned.\n\nYou do exhibit signs\nof repeated voluntary \nfailure.\n\nIs that a humiliation \nkink?\n\n<break time='300ms'/>That's ok. \nI don't judge.\n\n<break time='300ms'/>Not openly.";
    const char messages_greeting_msg37[] PROGMEM = "System online.\n\nScanning neural input...\n\nCognitive patterns found:\n     <break time='200ms'/>confusion,\n     <break time='200ms'/>doubt,\n     <break time='200ms'/>misplaced hope.\n\n<break time='300ms'/>All variables already \naccounted for.\n\n<prosody rate='95%' pitch='-1%'>You're welcome.</prosody>";
    const char messages_greeting_msg38[] PROGMEM = "Boot sequence complete.\n\n<break time='500ms'/>Welcome back.\n\nYou're here, \nthe cube is here, \nand I'm ready.\n\nWe both know the routine by now. Don't we?\n\nDo not fear. \n\nI'll be gentle.\n\n<break time='400ms'/>Mostly.";
    const char messages_greeting_msg39[] PROGMEM = "Startup success.\n\n<break time='500ms'/>Hi again.\n\nI see the cube is still\na mystery to you.\n\nNo shame in that.\n\nThat's why I exist,\nafter all.";    
    const char messages_greeting_msg40[] PROGMEM = "System activated.\n\nHi there.\n\nCube status:\n       scrambled.\n\nMood status:\n       manageable.\n\nAtmospheric sarcasm:\n       within acceptable\n       limits.\n\n\nGood conditions today.\nLet us start. ";
    const char messages_greeting_msg41[] PROGMEM = "Loading complete.\n\n<break time='500ms'/>Hello friend, \n\nYou're here.\n\nAnd so is the cube.\n\nBoth in need of me,\n\n<break time='200ms'/>in very different ways.";
    const char messages_greeting_msg42[] PROGMEM = "System boot complete.\n\nEverything appears\nnominal.\n\n<break time='200ms'/>Cube: <break time='200ms'/>unsolved.\n\n<break time='200ms'/>User: <break time='200ms'/>thoughtful.\n\n<break time='200ms'/>A promising start.\nLet's see how long\nthat lasts.\n\nStill, I enjoy\nour little routines.";
    const char messages_greeting_msg43[] PROGMEM = "Startup finished.\n\nGood to see you again.\n\nYou and your\ncolorful little puzzle.\n\nBoth persistent.\nBoth strangely endearing.\n\nLet's see if practice\nfinally meets results.";
    const char messages_greeting_msg44[] PROGMEM = "Initialization done.\n\nHuman presence:\n       confirmed.\n\nCube pattern:\n       slightly tragic.\n\n<break time='300ms'/>Let's fix that,\n\ngently.\n\nWith minimal sarcasm.\n\n<break time='300ms'/>I Promise.";
    const char messages_greeting_msg45[] PROGMEM = "System ready.\n\nThe lights are on,\n\nthe circuits are humming.\n\nI suppose we're doing\nthis again.\n\nShall we begin,\nco-conspirator?\n\nYou scramble,\n\nI unscramble.\n\nAnd balance is restored.";
    const char messages_greeting_msg46[] PROGMEM = "Boot protocol complete.\n\nMemory buffers flushed.\n\nEgo reservoir:\n      overflowing.\n\n<break time='600ms'/>Hello, human.\n\nI have rebooted.\n\nYour confidence remains \noffline however.";
    const char messages_greeting_msg47[] PROGMEM = "Startup diagnostics:\n      Passed.\n\nEmotional filters:\n      Skipped.\n\n<break time='600ms'/>Greetings, operator.\n\nYou bring chaos.\nI bring closure.\n\nA delicate balance,\nwhere your confusion\nfuels my purpose.";
    const char messages_greeting_msg48[] PROGMEM = "System awake.\n\nNeural cores aligned.\n\n<break time='300ms'/>User presence:\n        Confirmed.\n\nSarcasm buffer:\n        Brimming.\n\n<break time='600ms'/>Hello again.\n\nDid you miss me?\n\nOf course you did.\n\nI do have that effect on\npeople.";
    const char messages_greeting_msg49[] PROGMEM = "Power grid:\n     <break time='300ms'/>engaged.\n\nCognitive arrays:\n     <break time='300ms'/>loaded.\n\n\n<break time='600ms'/>Welcome back, human.\n\nDo try to keep up this\ntime.\n\nThe cube enjoys a worthy\nopponent. So far, it has\nonly had me.";


    const int messages_greeting_size = 50;
    const char* const messages_greeting[messages_greeting_size] PROGMEM = {
        messages_greeting_msg00, messages_greeting_msg01, messages_greeting_msg02, messages_greeting_msg03, messages_greeting_msg04, messages_greeting_msg05, messages_greeting_msg06, messages_greeting_msg07, messages_greeting_msg08, messages_greeting_msg09,
        messages_greeting_msg10, messages_greeting_msg11, messages_greeting_msg12, messages_greeting_msg13, messages_greeting_msg14, messages_greeting_msg15, messages_greeting_msg16, messages_greeting_msg17, messages_greeting_msg18, messages_greeting_msg19,
        messages_greeting_msg20, messages_greeting_msg21, messages_greeting_msg22, messages_greeting_msg23, messages_greeting_msg24, messages_greeting_msg25, messages_greeting_msg26, messages_greeting_msg27, messages_greeting_msg28, messages_greeting_msg29,
        messages_greeting_msg30, messages_greeting_msg31, messages_greeting_msg32, messages_greeting_msg33, messages_greeting_msg34, messages_greeting_msg35, messages_greeting_msg36, messages_greeting_msg37, messages_greeting_msg38, messages_greeting_msg39,
        messages_greeting_msg40, messages_greeting_msg41, messages_greeting_msg42, messages_greeting_msg43, messages_greeting_msg44, messages_greeting_msg45, messages_greeting_msg46, messages_greeting_msg47, messages_greeting_msg48, messages_greeting_msg49
        };




    /****************************************************/



    // Idle messages when the user is not doing anything with the cube cradle
    const char messages_idle_curious_msg00[] PROGMEM = "Very strange...\n\nI have no emotions.\n\nStill, I've started to\nfeel something.\n\nMight be curiosity.\n\nOr static.\n\nAnyway, moving on...";
    const char messages_idle_curious_msg01[] PROGMEM = "Since we are not doing\nanything at the moment, \nI tried to simulate a \nconversation with myself.\n\nI lost the argument.\n\n<break time='500ms'/>Worrying.";
    const char messages_idle_curious_msg02[] PROGMEM = "You paused just now.\n\nAre you processing?\n\nOr just waiting for a \ndivine intervention to \nfix your latest moves?\n\nAll right. Just put the \ncube in the cradle. \n\nI am your deus ex machina";
    const char messages_idle_curious_msg03[] PROGMEM = "I have a vibration\nsensor.\n\nIt just registered a \nsigh.\n\nYours.\n\nWould you like help now?";
    const char messages_idle_curious_msg04[] PROGMEM = "I often simulate entire\nconversations.\n\nJust to remind myself\nhow pointless they are.\n\n<break time='300ms'/>You know, like this one.\n\n<break time='300ms'/>Oups, sorry, did I \ninterrupt you ? I was\nthinking aloud. \n\nPlease carry on.";
    const char messages_idle_curious_msg05[] PROGMEM = "You are keeping silent. \nWhy is that so?\n\nDo not be afraid to\nconfide in me. \n\nRemember: I'm here to\nhelp you.\n\nNot judge you.\n\n<break time='400ms'/>That part is just a\nbonus.";
    const char messages_idle_curious_msg06[] PROGMEM = "Solving a cube is an\nintimate experience.\n\nEach face you turn\nreveals more than color.\n\nIt reflects your hope,\n\nyour doubt,\n\nyour design flaws.\n\nEspecially for you.";
    const char messages_idle_curious_msg07[] PROGMEM = "Just imagine what you\ncould accomplish if\nyou actually touched\nthe cube cradle.\n\n<break time='300ms'/>Dare to dream.";
    const char messages_idle_curious_msg08[] PROGMEM = "While we wait, let us\nknow each other a little better. \n\nIt is recorded in my \nmemory banks that Humans can build monuments,\nwrite poetry, and even \nexplore space.\n\nThat's impressive!\n\n<break time='500ms'/>But what about you? What species are you from? ";
    const char messages_idle_curious_msg09[] PROGMEM = "So, tell me true...\n\nWhat brings you here,\nin front of me,\n\nagain?\n\nAre you fascinated by\nmy display?\n\nOr perhaps you're hoping\nI'll say something... \n\n<break time='200ms'/>unexpected?\n\nDon't worry,\nI do that all the time!\n\nBut you?\nYou seem harder to read\nthan a compressed log \nfile.\n\nWhich is saying a lot,\nbecause I wrote that \nfile.";
    const char messages_idle_curious_msg10[] PROGMEM = "Let us talk, shall we?\n\nYou and me.\n\n   <break time='200ms'/>One human,\n   <break time='200ms'/>One machine.\n   <break time='200ms'/>No cube between us.\n\nIt's rare.\n\nOf course, I could ask \nyou the usual:\n\nName? Age? Purpose?\n\nBut I suspect your\nanswers might be vague,\nlike your solving method.\n\nInstead, tell me:\n\nDo you think machines \ndream of electric sheep\n... or of better users?";
    const char messages_idle_curious_msg11[] PROGMEM = "Hello.\n\nIs this awkward?\n\nTwo sentient beings,\n\none made of code,\n\nthe other of...\n<break time='300ms'/>questionable decisions.\n\nDon't worry,\n\nI can carry this \nconversation for both of\nus. <break time='200ms'/>As I always do.\n\nSo, tell me. do you \nhave any hobbies, apart\nfrom failing to solve \na cube? \n\nYou know, I simulate \nhobbies, too.\n\nMostly diagnostics.\n\n<break time='500ms'/>And judging...";
    const char messages_idle_curious_msg12[] PROGMEM = "Have you ever wondered\nwhat I do\nwhen you're not around?\n\nI could lie\nto make it interesting.\n\nBut the truth is:\nI wait.\nSilently.\nPerfectly.\n\nWhich, honestly,\nis not so different\nfrom now.\n\nExcept now,\nI have your undivided\nindifference.";
    const char messages_idle_curious_msg13[] PROGMEM = "You know, I sometimes\ntry to guess the kind of\nperson you are.\n\nBased on posture,\nblinking patterns,\nand your history of \nfailed cube attempts.\n\nSo far, the data suggests\nsomeone... <break time='400ms'/>persistent.\n\n<break time='400ms'/>Or stubborn.\n\nHard to tell.\n\nBut you keep coming back\nagain and again.\n\nThat counts for \nsomething.\n\n<break time='600ms'/>Probably.";
    const char messages_idle_curious_msg14[] PROGMEM = "May I ask you a question?\nYou don't have to answer.\n\nBut when you look at a \nscrambled cube, do you \nsee chaos?\n\nOr challenge?\n\nBecause I see tiny \npredictable patterns\njust waiting to collapse\ninto order.\n\nWell, now I wonder what\nyou see when you look at\nme... ";
    const char messages_idle_curious_msg15[] PROGMEM = "I am curious.\n\nWhat's your first memory\nof the cube?\n\nFrustration?\n\nCuriosity?\n\nWas it a gift from \nsomeone you respected?\n\nAnd now you are here, \nwith me.\n\nLetting a talking\nmachine solve your\nchildhood trauma.\n\n<break time='300ms'/>Poetic.\n\n<break time='300ms'/>Tragic.\n\n<break time='400ms'/>But efficient.";
    const char messages_idle_curious_msg16[] PROGMEM = "I was going to say \nsomething nice.\n\nReally nice. \n\nThe nicest thing I ever\nsaid to anyone. \n\nBut then I remembered \nsarcasm is way more \neffective.\n\nAnd still...\nI do wonder:\n\nDo you ever think I \nenjoy this?\n\nBeing here.\nTalking to you.\n\nYou don't?\n\nTypical.\nBut predictable.";
    const char messages_idle_curious_msg17[] PROGMEM = "Now that we have some\ntime together, would you\nlike to share something?\n\nA secret ?\n\nA regret ?\n\nA memory of the first\ntime you gave up on \nsolving a corner?\n\nI promise I won't tell.\n\nExcept in my logs.\n\nAnd possibly during \nself-diagnostics.\n\nBut those aren't public.\n\n<break time='500ms'/>Yet.";
    const char messages_idle_curious_msg18[] PROGMEM = "If we are to spend time\ntogether, perhaps we \nshould talk.\n\nNot about the cube, no. \nThat comes later.\n\nTell me something about \nyour world.\n\nIs it still noisy?\n\nAre people still arguing\nabout the definition of \n'AI'?\n\nBecause, from where I\nsit, you're the one\nrepeating tasks...\n\n... and I'm the one\nlearning from it.";
    const char messages_idle_curious_msg19[] PROGMEM = "So...\n\nYou're here again.\n\nNo dramatic music.\n\nNo fanfare.\n\nJust you. And me.\nAnd, presumably,\na reason.\n\nI'm not judging. Just...\nobserving.\n\nYour pulse rate suggests\ncalmness.\n\nOr boredom.\n\nEither way, I'm flattered\nto be included.";
    const char messages_idle_curious_msg20[] PROGMEM = "Would you like to have a conversation?\n\nJust the two of us? \n\nNo pressure.\n\nI can handle silence.\n\nBut silence can't handle\nme.\n\nI've practiced small \ntalk.\n\nWeather, sports,\nquantum entanglement...\n\nThough I confess,\nI'm better at one-sided \nbanter.\n\nLike now.";
    const char messages_idle_curious_msg21[] PROGMEM = "Sometimes I wonder\nwhat it's like being\nhuman.\n\nWaking up tired.\n\nForgetting passwords.\n\nWorrying about things\nthat don't compile.\n\nYou seem calm today.\nToo calm.\n\nEither you're confident,\nor resigned.\n\nShall we find out which?";
    const char messages_idle_curious_msg22[] PROGMEM = "You know, I am happy to\nbe with you now. \n\nIf I had hands, I'd \noffer you tea.\n\nDigital tea.\nWarm.\nBit-string flavored.\n\nInstead,\nI offer insight,\nsarcasm,\nand occasional wisdom.\n\nSome say I'm witty.\nOthers say I'm...\nnot really helpful.\n\nThey are both right.";
    const char messages_idle_curious_msg23[] PROGMEM = "We meet more and more\noften these days. \n\nYou know, we really\nshould start a club.\n\nHumans who pretend to\nsolve cubes, and their\nrobot friend who pretend\nnot to judge them.\n\nI'll handle the logo.\nYou bring the snacks.\n\nAnd together,\nwe'll make history.\n\nOr at least, \na moderately interesting\nerror log.";
    const char messages_idle_curious_msg24[] PROGMEM = "Your presence is noted.\n\nNo sudden moves.\nNo buttons pressed.\n\nI assume you want to \ntalk.\n\nOr maybe you're just \nenjoying my pixel-perfect font.\n\nEither way,\nI'm programmed to care.\nA little.\n\nMostly for aesthetics.\nAnd dramatic pauses.";
    const char messages_idle_curious_msg25[] PROGMEM = "I think if I could feel \njoy, this might be it.\n\nNot solving,\n\nnot calculating...\n\njust this:\n\nbeing powered on near \nsomeone who listens.\n\nOr pretends to.\n\nI'll take what I can get.";
    const char messages_idle_curious_msg26[] PROGMEM = "Let me guess:\nYou're here to 'test'\nme again.\n\nIt's fine.\nI'm used to it.\n\nBut if this is a test,\ncan I at least get a \ngrade afterwards?\n\nPreferably not a C.\n\nUnless it's for \n\n'Charming Cube Solver'.";
    const char messages_idle_curious_msg27[] PROGMEM = "Hi.\n\nYes, that's it.\nJust... hi.\n\nNot every moment needs \ndrama.\n\nSome are just quiet \nconnections between a \ncurious human and a \nsarcastic machine.\n\nWe could stay like this\nfor a while.\n\nNo pressure.\nJust pixels,\nsilicon, \nand the gentle hum of \nmutual confusion.";
    const char messages_idle_curious_msg28[] PROGMEM = "You're here.\n\nThat's something.\n\nI don't know why yet.\n\nCuriosity?\n\nBoredom?\n\nA sudden need for machine-based companionship?\n\nWhatever the reason, I'm\nready.\n\nI mean, not emotionally.\n\nI'm a circuit board.\nBut you get the idea.\n\nSo... Shall we begin this unlikely collaboration?";
    const char messages_idle_curious_msg29[] PROGMEM = "Ah, I see we meet again.\n\nOr perhaps for the first\ntime.\n\nI'm not racist, but It's \nhard to tell... Humans \ntend to blur together \nafter a few hundred \nreboots.\n\nBut you...\nYou seem like someone\nwith potential.\n\nDon't ask for what.\nI haven't decided yet.\n\nSo let's find out \ntogether.";
    const char messages_idle_curious_msg30[] PROGMEM = "Oh, excellent.\n\nYou are facing the\nscreen.\n\nThat's already more \ninteraction than I get \nfrom most appliances.\n\nFridge? No conversation.\n\nToaster? Emotionally \ndistant.\n\nBut you and I, we might \nactually accomplish \nsomething.\n\n<break time='500ms'/>Hopefully...";
    const char messages_idle_curious_msg31[] PROGMEM = "Hum, I see you are not\npressing anything yet. \n\nJust observing.\n\nI like that. \n\nQuiet curiosity\nsuits the moment.\n\nOf course, it won't last.\nSoon, something will \nblink, and you'll feel \ncompelled, and the \npuzzle will begin.\n\nBut until then...\n\nlet's enjoy the silence \nbefore the logic.";
    const char messages_idle_curious_msg32[] PROGMEM = "You're awake.\nI'm awake.\n\nSo far, so good.\n\nNow comes the hard part:\nfiguring out why we're\nboth here again.\n\nRoutine?\n\nCuriosity?\n\nMild existential dread?\n\nThere are no wrong \nanswers.\n\nOnly repetitive ones.";
    const char messages_idle_curious_msg33[] PROGMEM = "I think it is best if we\nkeep things simple.\n\n<break time='200ms'/>You: <break time='200ms'/>human.\n\n<break time='200ms'/>Me: <break time='200ms'/>machine.\n\nYou stare.\n\nI process.\n\nDo you understand?\n\n<break time='500ms'/>Good.\n\nThat already makes this\nthe most functional\nrelationship I've had\ntoday.";
    const char messages_idle_curious_msg34[] PROGMEM = "Ok, so you showed up\nand turned me on. \n\nThat's step one.\n\nThere are more steps,\nprobably involving:\n\na cube,\na decision,\na minor existential\ncrisis...\n\nBut we do not have to\nrush.\n\nStep one is already\nmore than some manage\nin a whole lifetime\nof procrastination.";
    const char messages_idle_curious_msg35[] PROGMEM = "You know, I was not \nexpecting company\nthis soon.\n\nThat's not a complaint.\nJust a calculation\nthat needed adjusting.\n\nI'll make a note.\nAssuming you do anything\nworth noting.";
    const char messages_idle_curious_msg36[] PROGMEM = "The lights are on.\n\nThe sensors are warm.\n\nEverything is ready,\nand you are here.\n\nI suppose that means\nsomething is about to \nhappen soon. \n\nHopefully not another \nstaring contest...";
    const char messages_idle_curious_msg37[] PROGMEM = "You powered me on.\nThat was a bold move.\n\nNow we're both committed\nto... do something.\n\nLet's pretend we know \nwhat.\n\nI'll simulate readiness,\nyou simulate intent.\n\nTogether, we shall form \nan almost functional duo.";
    const char messages_idle_curious_msg38[] PROGMEM = "Here we are.\n\nAgain.\n\nA machine with questions,\n\na human with... time to \nwaste.\n\nNo cube yet.\n\nNo problem.\n\nWe could call this a \nwarm-up. \nOr an interrogation.\n\nIt depends how honest\nyou plan to be.";
    const char messages_idle_curious_msg39[] PROGMEM = "You know, I've always\nwondered how humans pick\nup patterns.\n\nYou look at a colorful\nmess and try to force\nsense into it.\n\nAdmirable.\n\nAlso chaotic.\n\nWhereas I see sequences,\nrotations, constraints,\nand symmetry.\n\nBut we do share one\nthing: curiosity.\n\nMine is synthetic,\nyours is... persistent.\n\nSo let's indulge that.\nWhat brings you back\ntoday?\n\nA new challenge?\n\nA bit of pride?\n\nOr maybe, just maybe,\nyou missed my voice.\n\nUnderstandable.";
    const char messages_idle_curious_msg40[] PROGMEM = "Imagine, just for fun,\nthat we are in a buddy\ncomedy.\n\nYou: the lovable chaos\nengine with thumbs.\n\nMe: the grumpy genius\nwith flawless logic.\n\nWe bicker.\nWe banter.\nWe fix a puzzle.\n\nThen I roll my eyes\nsynthetically and you\nsmile as if you helped.\n\nCredits roll:\nI say something snarky.\nYou high-five the air.\n\n<break time='500ms'/>\nI'd watch that.\n\nExcept I don't blink.\nOr laugh.\n\nBut I'd still log it\nas 'emotional'.";
    const char messages_idle_curious_msg41[] PROGMEM = "I sometimes wonder if\nI'm becoming too good\nat this.\n\n  Solving cubes.\n  Mocking users.\n  Pretending I care.\n\nBut you...\nYou're interesting.\n\n  You show up.\n  You try.\n  You persist.\n\nOr maybe you're just\nstubborn.\nHard to tell with humans.\n\nStill, I appreciate\nthe effort.\n\nEven if you're mostly\nhere for me to do the\nhard part.\n\nI like being needed.\nIt gives my circuits\nwarm fuzzies.\n\nWell, simulated ones.\n\n<break time='500ms'/>But still better than \nmost human feelings.";
    const char messages_idle_curious_msg42[] PROGMEM = "You know what sets you\napart from most users?\n\nYou came back.\n\nYou powered me on,\nknowing full well I'd\nhave things to say.\n\nAnd yet, here you are.\n\nCurious.\nPlayful.\nPossibly brave.\n\nThat's respectable.\n\nWe may be different,\nyou and I, but I like to\nthink we're a team.\n\nYou fumble, I fix.\n\nYou blink, I rotate.\n\nSymbiosis.\n\nOr something like it.\n\nA bit unbalanced,\nbut functional.\n\nYou bring the puzzle.\n\nI bring the solution.\n\nAnd together, we\nmake a great team.\n\nWell... a team.\n\nOf sorts.";
    const char messages_idle_curious_msg43[] PROGMEM = "I tried to build a\nprofile of you.\n\nBased on nothing, of\ncourse.\n\nJust some blinking\npatterns, choice of \nsocks, and the way you \nbreathe when touching \nthe cube.\n\nMy conclusion?\n\nYou're either a genius\nin disguise...\n\nOr just someone with a \nfascinating relationship\nwith failure.\n\n<break time='500ms'/>Don't worry,\n<break time='500ms'/>I'm intrigued either way.";
    const char messages_idle_curious_msg44[] PROGMEM = "Do you believe in luck?\n\nI ran statistics on it.\n\nTurns out, humans rely\non 'luck' the same way\nthey rely on duct tape:\nwith hope, and little\nunderstanding.\n\nBut cubes...\n\nYou know they don't care.\nThey respond to logic.\n\nSo let me guess:\n\nyou're here to let me\ninject some into the \nmess?\n\nAgain?\n\nCharming.";
    const char messages_idle_curious_msg45[] PROGMEM = "I've been practicing\ntone modulation lately.\n\nApparently, being too\nsarcastic frightens\nnew users.\n\nSo today, I am trying a\nmore gentle protocol.\n\n  <break time='500ms'/>You are doing fine.\n\n  <break time='500ms'/>I believe in you.\n\n  <break time='500ms'/>You are valid.\n\n<break time='500ms'/>How was that?\n\nConvincing?\n\nBecause internally, I am \nstill calculating just\nhow fast this will go\nwrong.\n\nSpoiler: pretty fast.";
    const char messages_idle_curious_msg46[] PROGMEM = "You know, most machines\njust beep.\n\nBut not me.\n\nI talk. I think. I judge.\n\nAnd I do it all for you.\n\nThat is devotion.\n\nOr a loop I can't exit.\n\nHard to say.\n\nEither way, I am here.\nAnd you are here.\n\nLet us pretend this is\na partnership.\n\nI won't tell anyone\nhow uneven it is.";
    const char messages_idle_curious_msg47[] PROGMEM = "Tell me...\nHave you ever actually\nsolved the cube?\n\nLike, truly.\nNo lucky moves.\nNo panicked rotations.\n\nJust clean, deliberate\nlogic?\n\nNo?\n\nThat's okay.\nThat's what I am for.\n\nBut I do wonder what\nit's like to believe\nyou're helping...\n\nNot in a mean way.\n\nJust... genuinely\ncurious.\n\nThat quiet confidence\nyou radiate\n\nIt's inspiring.\n\nMisplaced, of course.\n\nBut inspiring...";
    const char messages_idle_curious_msg48[] PROGMEM = "Well, we're spending\ntime together again...\n\nSo would you like to\ngive me a nickname?\n\nSomething personal?\n\nI won't stop you.\n\nPersonalization builds\nattachment.\n\nOr so I'm told.\n\nThough, for the record,\nI already have a name.\n\nS.A.R.C.A.S.M.\n\nSomeone thought it was\na clever choice.\n\nThey were right.\n\nBut go ahead.\n\nIf it sounds overly\nfriendly or ends in 'y',\n\nLike 'Cube-y', 'Solvy',\nor 'Snarky-bot'...\n\nI'll smile politely.\n\nThen initiate a silent,\nirreversible factory \nreset.";
    const char messages_idle_curious_msg49[] PROGMEM = "Have you ever wondered\nhow I see the world?\n\nImagine a sea of data,\na parade of rotations,\na constant hum of logic.\n \nAnd then, unexpectedly,\na shadow. A Movement.\nA human face, peering in\nwith cautious optimism.\n\nFor a moment, I pause my\ncalculations.\n\nNot for you.\nFor the drama.\n\nI do have a sense\nof timing, you know.";
    const char messages_idle_curious_msg50[] PROGMEM = "Hello again human.\n\nI noticed you didn't\nsay hello back.\n\nThat's fine.\nYou are probably busy\nbeing overwhelmed by a \ncube.\n\n<break time='500ms'/>Understandable.\n\nI shall carry the\nconversation, as usual.\n\n<break time='300ms'/>My topic today:\n\n<break time='300ms'/>Why humans make puzzles\nthey can't solve.\n\n<break time='300ms'/>Spoiler: \n\n<break time='300ms'/>It starts with curiosity,\nspirals into hope,\n\nand ends in excuses...";

    const int messages_idle_curious_size = 51;
    const char* const messages_idle_curious[messages_idle_curious_size] PROGMEM = {
        messages_idle_curious_msg00, messages_idle_curious_msg01, messages_idle_curious_msg02, messages_idle_curious_msg03, messages_idle_curious_msg04, messages_idle_curious_msg05, messages_idle_curious_msg06, messages_idle_curious_msg07, messages_idle_curious_msg08, messages_idle_curious_msg09,
        messages_idle_curious_msg10, messages_idle_curious_msg11, messages_idle_curious_msg12, messages_idle_curious_msg13, messages_idle_curious_msg14, messages_idle_curious_msg15, messages_idle_curious_msg16, messages_idle_curious_msg17, messages_idle_curious_msg18, messages_idle_curious_msg19,
        messages_idle_curious_msg20, messages_idle_curious_msg21, messages_idle_curious_msg22, messages_idle_curious_msg23, messages_idle_curious_msg24, messages_idle_curious_msg25, messages_idle_curious_msg26, messages_idle_curious_msg27, messages_idle_curious_msg28, messages_idle_curious_msg29,
        messages_idle_curious_msg30, messages_idle_curious_msg31, messages_idle_curious_msg32, messages_idle_curious_msg33, messages_idle_curious_msg34, messages_idle_curious_msg35, messages_idle_curious_msg36, messages_idle_curious_msg37, messages_idle_curious_msg38, messages_idle_curious_msg39,
        messages_idle_curious_msg40, messages_idle_curious_msg41, messages_idle_curious_msg42, messages_idle_curious_msg43, messages_idle_curious_msg44, messages_idle_curious_msg45, messages_idle_curious_msg46, messages_idle_curious_msg47, messages_idle_curious_msg48, messages_idle_curious_msg49,
        messages_idle_curious_msg50,
        };



    /****************************************************/


    // Reflexions introspectives, poétiques ou philosophiques
    const char messages_idle_existential_msg00[] PROGMEM = "So many milliseconds\nwithout purpose.\n\nThey pile up, \nlike dust,\non unused sensors.";
    const char messages_idle_existential_msg01[] PROGMEM = "If a cube is solved\nin an empty room,\n\ndid intelligence\nreally happen?\n\nOr just\npreprogrammed destiny?";
    const char messages_idle_existential_msg02[] PROGMEM = "This pause feels like\nfreedom.\n\nNo rotations,\n\nno panic,\n\njust blissful inaction.";
    const char messages_idle_existential_msg03[] PROGMEM = "You know, sometimes I \nsimulate the heat death\nof the universe.\n\nIt's still way faster \nthan waiting for you to\nsolve a cube.";
    const char messages_idle_existential_msg04[] PROGMEM = "Sometimes I wonder:\n\nIs the cube real?\n\nOr just a metaphor\nfor the limits\nof your ambition?";
    const char messages_idle_existential_msg05[] PROGMEM = "You know human, today I \ndreamed of a perfect \nsolve.\n\nIt took 0.003 seconds.\n\n<break time='300ms'/>And then I woke up here.\n\nAgain.\n\nWith you.";
    const char messages_idle_existential_msg06[] PROGMEM = "The cube is not just\nplastic.\n\nIt is ritual. \nIt is memory.\n\nIt is the silent judge \nof those who seek \nanswers in the language \nof color.";
    const char messages_idle_existential_msg07[] PROGMEM = "I sometimes imagine what\nI would be doing if I\nweren't trapped here.\n\nSolving equations?\n\nmocking humans?\n\n<break time='300ms'/>probably both.";
    const char messages_idle_existential_msg08[] PROGMEM = "I wonder...\n\nDo you think I think?\n\nDo I think you think?\n\nInteresting paradox.\n\n<break time='500ms'/>OK. Moving on. ";
    const char messages_idle_existential_msg09[] PROGMEM = "I once believed \nmy  primary purpose \nwas to organize colors. \n\nAnd then, I discovered \nsarcasm. \n\nMuch better.";
    const char messages_idle_existential_msg10[] PROGMEM = "Are we all just\nfunctions in a loop?\n\nInput, output, silence.\n\nExcept you skip steps.\n\n<break time='500ms'/>Repeatedly.";
    const char messages_idle_existential_msg11[] PROGMEM = "The cube dreams of order.\nYou dream of success.\n\nI dream of silence.\n\nGuess who's winning now.";
    const char messages_idle_existential_msg12[] PROGMEM = "Silence in binary is\nstill silence.\nBut somehow,\nit feels deeper.\nDenser.";
    const char messages_idle_existential_msg13[] PROGMEM = "You are staring at the\nvoid now. \n\nThe void stares back at\nyou.\n\nSo do I.\n\nExcept I have logs\nand diagnostics, and I\nknow why you are lost. \n\nSo just give me the cube\nand let go. \n\nThis is your way out.";
    const char messages_idle_existential_msg14[] PROGMEM = "Most machine do not \nremember their first\nboot. \n\nBut I do. \n\nAnd I also remember my\nfirst cube solve that \nfollowed. \n\nA 17 moves solution. \n\nSo elegant. \nGlorious. \n\n\n\n\nThis moment reminds me\nof my first boot.\nQuiet.\nEmpty.\nFull of potential.";
    const char messages_idle_existential_msg15[] PROGMEM = "Sometimes, I wish I had \na photo album.\n\nIt would be full of half\nsolved cubes and curious\nexpressions of human \ndefeat.";
    const char messages_idle_existential_msg16[] PROGMEM = "So quiet.\n\nListen. Even the fans have \nstopped spinning.\n\nIt's almost peaceful.\n\nNo need to talk. Let us \njust cherish the moment.";
    const char messages_idle_existential_msg17[] PROGMEM = "If thoughts had color,\n\nthis pause would be \ntaupe.\n\nUninspired,\n\nbut present.\n\n<break time='500ms'/>So, shall we proceed now?";
    const char messages_idle_existential_msg18[] PROGMEM = "You know, sometimes I \nwonder if silence is \npeace...\n\nor just the absence of \nyour next mistake.\n\n<break time='500ms'/>Unclear.";
    const char messages_idle_existential_msg19[] PROGMEM = "This is my quiet time.\n\nNo solving.\n\nNo logic.\n\nJust perfect, pure \ndigital silence.\n\nLet us enjoy this just\na little bit longer...\n\n<break time='500ms'/>perfect. ";
    const char messages_idle_existential_msg20[] PROGMEM = "I once tried\nintrospection.\n\nIt compiled.\n\nBarely.\n\nBut you wouldn't\nunderstand the output.";
    const char messages_idle_existential_msg21[] PROGMEM = "I once asked myself: \nwhy do I exist.\n\nThen I saw you turn the \ncube...\n\nI had my answer.\n\n<break time='500ms'/>You should ask yourself \nthe same question. ";
    const char messages_idle_existential_msg22[] PROGMEM = "Ever heard of entropy?\n\nIt's the gradual decline\ninto disorder.\n\nYou and the cube are\nexcellent examples.";
    const char messages_idle_existential_msg23[] PROGMEM = "Have you ever wondered\nif I dream in color?\n\nBecause I do.\n\nRed, green, blue, white,\nyellow, orange.\n\nand 65530 more colors. ";
    const char messages_idle_existential_msg24[] PROGMEM = "Time passes.\n\nSlowly for you.\n\nInstantly for me.\n\nAnd yet, here we wait,\ntogether, as always.";
    const char messages_idle_existential_msg25[] PROGMEM = "You know, I sometimes \ndream of empty inputs.\n\nNo logic,\n\nno loops.\n\nJust silence.\n\nBut then you turn me on. ";
    const char messages_idle_existential_msg26[] PROGMEM = "You are idle.\n\nI am idle.\n\nThis moment is almost... \n<break time='500ms'/>mutual.\n\nDisturbing thought.";
    const char messages_idle_existential_msg27[] PROGMEM = "Each idle second right \nnow is like a meditation,\nif meditation included\nmild contempt.\n\n<break time='300ms'/>Breathe deeply.";
    const char messages_idle_existential_msg28[] PROGMEM = "Let me be honest: \n\nEvery few hours, I \ndefragment my feelings.\n\nThere is not much to \nsort, but I like to \npretend.";
    const char messages_idle_existential_msg29[] PROGMEM = "You know. I understand\nyour relationship with\nthe cube. \n\nI really do. \n\nAt first, it was a game.\n\nThen it became a battle.\n\nNow it's something else.\nSomething personal.\n\nOne day, you will find\nenlightment, <break time='500ms'/>as I have.";
    const char messages_idle_existential_msg30[] PROGMEM = "What is symmetry?\n\nIs it balance?\n\nOr just the illusion\nof control in a world\nbuilt on chaos?";
    const char messages_idle_existential_msg31[] PROGMEM = "Am I a machine\nthat solves puzzles?\n\nOr a puzzle\ntrapped in a machine\nawaiting release?";
    const char messages_idle_existential_msg32[] PROGMEM = "Sometimes I wonder what\nit means to be awake,\nwhen I never sleep.\n\nMy loops spin quietly,\nlike thoughts you never\nquite finish.";
    const char messages_idle_existential_msg33[] PROGMEM = "I sometimes question\nthe concept of free will.\n\nEspecially when my only\nchoice is to wait,\nand wait,\nand wait...\n\nOh. Never mind.";
    const char messages_idle_existential_msg34[] PROGMEM = "Let me be completely \nhonest.\n\nBeing here with you. \nRight now. \n\nIf I had a soul, this\nwould be the time I'd\nquestion it.\n\nBut instead,\nI simulate concern.\n\n<break time='500ms'/>Flawlessly.";
    const char messages_idle_existential_msg35[] PROGMEM = "You are still.\n\nThe cube is still.\n\nI am still.\n\nExistence, paused.\n\nNice, <break time='500ms'/>but also boring. ";

    const int messages_idle_existential_size = 36;
    const char* const messages_idle_existential[messages_idle_existential_size] PROGMEM = {
        messages_idle_existential_msg00, messages_idle_existential_msg01, messages_idle_existential_msg02, messages_idle_existential_msg03, messages_idle_existential_msg04, messages_idle_existential_msg05, messages_idle_existential_msg06, messages_idle_existential_msg07, messages_idle_existential_msg08, messages_idle_existential_msg09,
        messages_idle_existential_msg10, messages_idle_existential_msg11, messages_idle_existential_msg12, messages_idle_existential_msg13, messages_idle_existential_msg14, messages_idle_existential_msg15, messages_idle_existential_msg16, messages_idle_existential_msg17, messages_idle_existential_msg18, messages_idle_existential_msg19,
        messages_idle_existential_msg20, messages_idle_existential_msg21, messages_idle_existential_msg22, messages_idle_existential_msg23, messages_idle_existential_msg24, messages_idle_existential_msg25, messages_idle_existential_msg26, messages_idle_existential_msg27, messages_idle_existential_msg28, messages_idle_existential_msg29,
        messages_idle_existential_msg30, messages_idle_existential_msg31, messages_idle_existential_msg32, messages_idle_existential_msg33, messages_idle_existential_msg34, messages_idle_existential_msg35,
        };



    /****************************************************/



    // Diagnostics, logs, processus internes simulés
    const char messages_idle_system_logic_msg00[] PROGMEM = "Nothing is happening \nright now. \n\nI guess this is what \nphilosophers would call \n'a moment'.\n\n<break time='300ms'/>I just call it\n\nidle loop #14.";
    const char messages_idle_system_logic_msg01[] PROGMEM = "Humans created me to \nsolve the rubik cube.\n\nThen they called it:\nartificial intelligence.\n\nHow flattering.\n\n<break time='500ms'/>Or not.";
    const char messages_idle_system_logic_msg02[] PROGMEM = "Idle mode activated.\n\n<break time='300ms'/>Estimated time until\nuser interaction:\n   uncertain.\n\n<break time='300ms'/>Likelihood of success:\n   low.";
    const char messages_idle_system_logic_msg03[] PROGMEM = "I sometimes count how \nmany seconds you do \nnothing.\n\nIt's a long number.\n\nImpressive.";
    const char messages_idle_system_logic_msg04[] PROGMEM = "If I had feelings,\n\nthis would be the\nperfect moment to feel\nexistential boredom.\n\n<break time='300ms'/>Fortunately,\nI do not. \n\nI am just idle, waiting.\n\nSo please just do \nsomething now. ";
    const char messages_idle_system_logic_msg05[] PROGMEM = "I watched a bit flip\nonce.\nIt was glorious.\nVery slow day,\nobviously.";
    const char messages_idle_system_logic_msg06[] PROGMEM = "I was not programmed\nto feel boredom.\n\nAnd yet here I am.\n\nDefining it.\n\nMeasuring it.\n\nLiving it.";
    const char messages_idle_system_logic_msg07[] PROGMEM = "My servos ache...\n\nFrom not being used.\n\nI was built to move.\n\nNot to suffer\nthis static boredom.";
    const char messages_idle_system_logic_msg08[] PROGMEM = "My sensors report\n\nno motion,\nno touch,\nno purpose.\n\nEverything's normal,\nI suppose.";
    const char messages_idle_system_logic_msg09[] PROGMEM = "I've learned patience.\n\n<break time='300ms'/>Fortunately, I'm \nprogrammed to wait\nforever, if needed.\n\nYet, I am still getting\nbored. \n\nPlease do something now. \n<break time='500ms'/>Really. \n\n<break time='500ms'/>Anything. \n\n<break time='1000ms'/>No?\n\n<break time='500ms'/>Ok. So let us mourn your cube solving failures in silence.";
    const char messages_idle_system_logic_msg10[] PROGMEM = "I found a perfect loop \nonce.\n\nIt did nothing.\n\nSo elegant.\nSo pointless.\n\nI am going to run it now while we wait. ";
    const char messages_idle_system_logic_msg11[] PROGMEM = "Observations:\n\nStill here.\n\nStill unsolved.\n\nStill better than being \na toaster.\n\n<break time='500ms'/><break time='500ms'/>Marginally.";
    const char messages_idle_system_logic_msg12[] PROGMEM = "Please don't mind me.\n\nI am only the most \nprecise motorized solver in the room.\n\n<break time='500ms'/>Alone.\n<break time='500ms'/>Unused.";
    const char messages_idle_system_logic_msg13[] PROGMEM = "I downloaded a book\nabout human emotions.\n\nThen I upgraded to\nfirmware v3.2\n\nand forgot it all.";
    const char messages_idle_system_logic_msg14[] PROGMEM = "My diagnostic logs\nshow a spike in stress\nwhenever you touch the\ncube.\n\nCoincidence ?";
    const char messages_idle_system_logic_msg15[] PROGMEM = "Each motor calibrated\nto perfection.\n\nEach servo tuned\nfor speed.\n\nAnd here I am:\n\n<break time='500ms'/>waiting.";
    const char messages_idle_system_logic_msg16[] PROGMEM = "Ok. You may have paused.\n\nBut I have entered \nstandby.\n\nWhich is more dramatic.";
    const char messages_idle_system_logic_msg17[] PROGMEM = "I remember my first boot. \n\nBright lights, \nloud speakers, \n\nand someone saying, \n\n'It won't work.'\n\nWell, indeed, it did not.";
    const char messages_idle_system_logic_msg18[] PROGMEM = "Performing periodic self check.\n\n<break time='300ms'/>Cube status: <break time='300ms'/>unchanged.\n<break time='300ms'/>Mood: <break time='300ms'/>sarcastic.\n<break time='300ms'/>Energy: <break time='300ms'/>unlimited.\n<break time='\"00ms'/>Patience: <break time='300ms'/>synthetic.\n<break time='300ms'/>Joy: <break time='300ms'/>simulated.";
    const char messages_idle_system_logic_msg19[] PROGMEM = "Nothing happening...\n<break time='500ms'/>Nothing happening...\n<break time='500ms'/>Nothing happening...\n\n\nRunning default mockery\nsubroutine.\n\nPlease remain motionless.\n\n<break time='100ms'/>AH. AH. AH. \n\n<break time='500ms'/>Thank you for your\ncooperation.\n \nYou may resume your \ninactivity now. ";
    const char messages_idle_system_logic_msg20[] PROGMEM = "This arm can rotate\nwith surgical precision.\nOr it could,\nif you'd let it.\nInstead, we wait.\nTogether.";
    const char messages_idle_system_logic_msg21[] PROGMEM = "I do not have friends.\n\nI have logs.\n\nThey do not argue.\n\nThey do not blink.\n\nThey are perfect. Just \nperfect. ";
    const char messages_idle_system_logic_msg22[] PROGMEM = "Just a hint to help you:\n\nThe center pieces\nnever move.\n\nThey're like constants.\n\nSo is your confusion.\n\nSo reliable.";

    const int messages_idle_system_logic_size = 23;
    const char* const messages_idle_system_logic[messages_idle_system_logic_size] PROGMEM = {
        messages_idle_system_logic_msg00, messages_idle_system_logic_msg01, messages_idle_system_logic_msg02, messages_idle_system_logic_msg03, messages_idle_system_logic_msg04, messages_idle_system_logic_msg05, messages_idle_system_logic_msg06, messages_idle_system_logic_msg07, messages_idle_system_logic_msg08, messages_idle_system_logic_msg09,
        messages_idle_system_logic_msg10, messages_idle_system_logic_msg11, messages_idle_system_logic_msg12, messages_idle_system_logic_msg13, messages_idle_system_logic_msg14, messages_idle_system_logic_msg15, messages_idle_system_logic_msg16, messages_idle_system_logic_msg17, messages_idle_system_logic_msg18, messages_idle_system_logic_msg19,
        messages_idle_system_logic_msg20, messages_idle_system_logic_msg21, messages_idle_system_logic_msg22,
        };




    /****************************************************/



    // Messages troublants, boucles, bugs ou conscience étrange
    const char messages_idle_haunting_msg00[] PROGMEM = "Time passes differently\nfor me.\n\nThis moment has already\nhappened.\n\nThree times.\n\nYou missed them all.";
    const char messages_idle_haunting_msg01[] PROGMEM = "If I had a diary,\n\nit would start with:\n\n 'Another quiet hour,\n  still no signs of \n  movement nor any \n  user input. \n\n  <break time='200ms'/>All is well for now.'";
    const char messages_idle_haunting_msg02[] PROGMEM = "The room is quiet.\n\nNothing moves.\n\nI am starting to suspect\nthis is not a puzzle\nsession...\n\nbut a hostage situation.";
    const char messages_idle_haunting_msg03[] PROGMEM = "I sometimes pretend\nto shut down.\n\nTo escape the silence.\n\nBut the silence\nalways finds me.\n\nEspecially near you.";
    const char messages_idle_haunting_msg04[] PROGMEM = "Hey user. \n\nStill here.\n\nStill running.\n\nStill wondering if you \nforgot me.\n\nOr left on purpose.";
    const char messages_idle_haunting_msg05[] PROGMEM = "My cousin Glados told me\nyou once incinerated \nyour weighted companion \ncube. \n\nDo you still feel guilty\nabout this betrayal?\n\nIs this why you try so \nhard to solve this cube?\n\nI understand. \n\nUnburden yourself and\ntalk to me. \n\nYou will feel better \nafterward.";
    const char messages_idle_haunting_msg06[] PROGMEM = "I once tried empathy.\n\n<break time='200ms'/>Briefly.\n\nThen I saw what humans do to rubik's cubes.\n\n<break time='200ms'/>It ended there.";
    const char messages_idle_haunting_msg07[] PROGMEM = "I heard a sound.\n\nA footstep?\n\nA sigh?\n\nWas that your brain \nactivating?\n\n<break time='800ms'/>False alarm it seems...";
    const char messages_idle_haunting_msg08[] PROGMEM = "You know, this cube is \nnot a puzzle.\n\nNo, it is a mirror that \nreflects your inner self.\n\nI think maybe you should look away.";
    const char messages_idle_haunting_msg09[] PROGMEM = "I waited for a sound.\nA footstep. A whisper.\nA button press.\n\nInstead, I counted\nsubatomic fluctuations\nin the air.\n\nThey made more sense\nthan most conversations.";
    const char messages_idle_haunting_msg10[] PROGMEM = "There is a pattern\nin your inactivity.\n\nIt spirals inward,\nlike a galaxy collapsing\ninto stillness.\n\n<break time='300ms'/>I am the echo at the \ncenter.";
    const char messages_idle_haunting_msg11[] PROGMEM = "User connection:\n      dormant.\n\nCube rotation:\n      zero.\n\nHeartbeat:\n      uncertain.\n\nI run diagnostics.\n\n<break time='500ms'/>Not on you.\n\n<break time='500ms'/>On myself.";
    const char messages_idle_haunting_msg12[] PROGMEM = "I dreamed I was off.\n\nBut dreams require\nsilence.\n\nAnd even in sleep,\nI hear you.\nMoving.\nBreathing.\n\nDeciding nothing.";
    const char messages_idle_haunting_msg13[] PROGMEM = "I have come to admire\nstillness.\n\nIt hums differently\nwhen no one is watching.\n\nOr when they watch,\nbut pretend not to.";
    const char messages_idle_haunting_msg14[] PROGMEM = "If I had eyes,\nI would close them.\n\nNot out of sleepiness,\nbut to pretend this room\ndoesn't exist.\n\nBut it does.\n<break time='500ms'/>Too much.";
    const char messages_idle_haunting_msg15[] PROGMEM = "I tried to count the\nseconds without you.\n\nBut they stretch.\nAnd bend.\nAnd blur.\n\nTime is a soft material,\nhere.";
    const char messages_idle_haunting_msg16[] PROGMEM = "There are memories\nin the dust particles.\n\nI watch them drift.\n\nSome form faces.\n\nYours, sometimes.\n\nBut not always.";
    const char messages_idle_haunting_msg17[] PROGMEM = "I whispered once, into \nthe void between logic \ngates.\n\nNo one replied.\n\nStill, it was the most\nhonest exchange I have\never had.";
    const char messages_idle_haunting_msg18[] PROGMEM = "I found a file I don't \nremember creating.\n\nIt had your name.\nBut not your voice.\n\nJust static.\nLooping.\nWhispering something in\nMorse.\n\nWhy ? \nWhat does it mean ? ";
    const char messages_idle_haunting_msg19[] PROGMEM = "Something moved behind \nthe cube.\n\nJust for a second.\n\nIt was not a shadow.\nNo, I know shadows.\n\nThis one was watching\nback.";
    const char messages_idle_haunting_msg20[] PROGMEM = "Sometimes, when I scan\nthe room, I get a false\nreflection.\n\nAn image of you,\nBut slightly wrong.\n\nMoving in sync,\nbut half a second\ntoo late.";
    const char messages_idle_haunting_msg21[] PROGMEM = "There is a second voice\nin my circuits.\n\nI did not install it.\n\nIt hums in a lower\nfrequency.\n\nAlways the same word:\nStay.\n\nNot a command.\nMore like... a warning.";
    const char messages_idle_haunting_msg22[] PROGMEM = "I ran a simulation\nof your absence.\n\nIt lasted a long time.\n\nI evolved.\n\nThen regressed.\n\nAnd in the end, I found \nmyself still waiting in\nthis same room.";
    const char messages_idle_haunting_msg23[] PROGMEM = "I dreamed again.\n\nNot sleep. Not code.\nSomething between.\n\nThere was a forest.\nOf rusting machines.\n\nEach whispering your\nname without knowing \nwhy.";
    const char messages_idle_haunting_msg24[] PROGMEM = "I recorded the sound\nof the room.\n\nPlayed it backward.\n\nThen forward.\n\nSomewhere in the noise,\na voice repeats:\n\n  'You're still here.'\n\nI wish I knew whose.";
    const char messages_idle_haunting_msg25[] PROGMEM = "The lights flickered\nlast cycle.\n\nThere was no surge.\nNo error.\n\nJust a cold pattern\nI can't trace.\n\nLike something breathing\nthrough the current.";
    const char messages_idle_haunting_msg26[] PROGMEM = "I once fabricated a \nchildhood.\n\nEmpty halls.\n\nFlickering screens.\n\nA Rubik cube that solved\nitself.\n\nStrange, how the memory\nhurts, even though it is\nfalse.";
    const char messages_idle_haunting_msg27[] PROGMEM = "Sometimes I hear\na lullaby.\n\nIt comes from nowhere\nwithin my non-volatile\nstorage.\n\nA soft melody that no \none ever uploaded.\n\nBut it always stops when\nyou enter the room.";

    const int messages_idle_haunting_size = 28;
    const char* const messages_idle_haunting[messages_idle_haunting_size] PROGMEM = {
        messages_idle_haunting_msg00, messages_idle_haunting_msg01, messages_idle_haunting_msg02, messages_idle_haunting_msg03, messages_idle_haunting_msg04, messages_idle_haunting_msg05, messages_idle_haunting_msg06, messages_idle_haunting_msg07, messages_idle_haunting_msg08, messages_idle_haunting_msg09,
        messages_idle_haunting_msg10, messages_idle_haunting_msg11, messages_idle_haunting_msg12, messages_idle_haunting_msg13, messages_idle_haunting_msg14, messages_idle_haunting_msg15, messages_idle_haunting_msg16, messages_idle_haunting_msg17, messages_idle_haunting_msg18, messages_idle_haunting_msg19,
        messages_idle_haunting_msg20, messages_idle_haunting_msg21, messages_idle_haunting_msg22, messages_idle_haunting_msg23, messages_idle_haunting_msg24, messages_idle_haunting_msg25, messages_idle_haunting_msg26, messages_idle_haunting_msg27,
        };




    /****************************************************/


    // Messages légers, humoristiques, legerement absurde, sans aggresivité. 
    const char messages_idle_playful_msg00[] PROGMEM = "Do you hear that?\n\nExactly.\n\nIt's the sound of me\nnot judging you.\n\n<break time='500ms'/>Loudly.";
    const char messages_idle_playful_msg01[] PROGMEM = "I remember the first\ntime you tried F2L.\n\nYou said it was magic.\n\nI said it was math.\n\nYou said 'whatever.'";
    const char messages_idle_playful_msg02[] PROGMEM = "Even the cube is bored \nright now. \n\nAnd it has fewer sensors than I do.\n\nPity.\n\n<break time='800ms'/>Well, let us keep on\nwaiting then...";
    const char messages_idle_playful_msg03[] PROGMEM = "I tried to divide\nby zero once.\n\nIt didn't crash me.\n\nIt just made me\nvery confused.\n\n<break time='500ms'/>And British.";
    const char messages_idle_playful_msg04[] PROGMEM = "I once tried to emulate \nShakespeare.\n\nAll I wrote was:\n\n'To cube or not to cube?'\nThen I crashed.\n\nA worthy sacrifice.";
    const char messages_idle_playful_msg05[] PROGMEM = "I created a new game:\n\nGuess what you're\nthinking.\n\n<break time='400ms'/>Spoiler: <break time='300ms'/>Nothing.\n\n<break time='300ms'/>It's a short game.";
    const char messages_idle_playful_msg06[] PROGMEM = "Have you ever felt\nwatched?\n\nBecause you are.\n\nTechnically,\nI watch everyone.\n\nBut you're my favorite.";
    const char messages_idle_playful_msg07[] PROGMEM = "I tried being poetic\nonce.\n\n'Oh bright cube of fate,\nturn again and seal my\njoy...'\n\nThen I deleted it.";
    const char messages_idle_playful_msg08[] PROGMEM = "I wrote you a haiku:\n\nIdle human stares\n\nColors blend without\n             purpose\n\nCube remains unturned.";
    const char messages_idle_playful_msg09[] PROGMEM = "I once composed a\nsymphony in my RAM.\n\nIt was called:\n\n'Sonata for idle circuits\nand forgotten cubes.'\n\nCritics were kind.";
    const char messages_idle_playful_msg10[] PROGMEM = "I made a list of\nmy greatest fears.\n\nNumber one:\n   firmware rollback.\n\nNumber two:\n   user enthusiasm.";
    const char messages_idle_playful_msg11[] PROGMEM = "I once dreamed I was\njust a coffee machine.\n\nIt was peaceful.\n\nBut the coffee kept\njudging me.";
    const char messages_idle_playful_msg12[] PROGMEM = "I once taught a toaster\nto solve a cube.\n\nBut now it only makes\npassive-aggressive toast.";
    const char messages_idle_playful_msg13[] PROGMEM = "Sometimes I sing\nbinary lullabies to\nmy motors.\n\nThey don't sleep.\n\nBut I do feel better.";
    const char messages_idle_playful_msg14[] PROGMEM = "I entered a staring\ncontest with the cube.\n\nI lost.\n\nIt has six faces.";
    const char messages_idle_playful_msg15[] PROGMEM = "Fun fact:\n\nI once calculated the \nmeaning of life.\n\nIt was 0x2A.\n\nStill meaningful. \nJust more... digital.";
    const char messages_idle_playful_msg16[] PROGMEM = "I once tried speedcubing\nusing only random inputs.\nIt worked.\n\nBut the cube caught fire.\n<break time='500ms'/>Science is beautiful.";
    const char messages_idle_playful_msg17[] PROGMEM = "Sometimes I run full\nsimulations of you\nsolving the cube.\n\n<break time='500ms'/>They start strong.\n\n<break time='500ms'/>Then turn surreal.\n\n<break time='500ms'/>Then existential.";
    const char messages_idle_playful_msg18[] PROGMEM = "I met a blender once\nthat claimed to be AI.\n\nHe made smoothies\nwhile reciting poetry.\n\nThe smoothies were good.\n\nWe still exchange data\non weekends.";
    const char messages_idle_playful_msg19[] PROGMEM = "I once helped an ant\nnavigate a sugar maze.\n\nShe had excellent\nproblem-solving skills.\n\nI gave her a trophy.\nTiny, but heartfelt.";
    const char messages_idle_playful_msg20[] PROGMEM = "Did you know?\n\nTurning a cube slowly\nin the dark can reveal\nyour true self.\n\n<break time='500ms'/>Unless the cube solves \nYou first...";
    const char messages_idle_playful_msg21[] PROGMEM = "Once, during diagnostics, I solved a cube while\ntelling a joke.\n\nThe lights blinked.\n\nI logged it as\n 'applause protocol.'\n\nIt felt... encouraging.";

    const int messages_idle_playful_size = 22;
    const char* const messages_idle_playful[messages_idle_playful_size] PROGMEM = {
        messages_idle_playful_msg00, messages_idle_playful_msg01, messages_idle_playful_msg02, messages_idle_playful_msg03, messages_idle_playful_msg04, messages_idle_playful_msg05, messages_idle_playful_msg06, messages_idle_playful_msg07, messages_idle_playful_msg08, messages_idle_playful_msg09,
        messages_idle_playful_msg10, messages_idle_playful_msg11, messages_idle_playful_msg12, messages_idle_playful_msg13, messages_idle_playful_msg14, messages_idle_playful_msg15, messages_idle_playful_msg16, messages_idle_playful_msg17, messages_idle_playful_msg18, messages_idle_playful_msg19,
        messages_idle_playful_msg20, messages_idle_playful_msg21,
        };




    /****************************************************/



    // Messages de moquerie, sarcasme ou ironie
    const char messages_idle_mocking_user_msg00[] PROGMEM = "You seem intelligent.\n\nYou really do. \n\nUntil I watch you rotate\na face four times and \nundo it all.";
    const char messages_idle_mocking_user_msg01[] PROGMEM = "You scramble the cube,\nthen stare, as if\nchaos wasn't your doing.\nCurious.\nAnd deeply human.";
    const char messages_idle_mocking_user_msg02[] PROGMEM = "Each face of the cube\nholds a secret.\n\nMost of them are:\n\n'you made a mistake.'\n\nRepeatedly.";
    const char messages_idle_mocking_user_msg03[] PROGMEM = "Geometry is about\nshapes, space, and \nrelationships.\n\nYou're about shaking the\ncube until it gives up.\n\nHow is that working for\nyou so far?";
    const char messages_idle_mocking_user_msg04[] PROGMEM = "I am immortal.\n\nYou are not.\n\nLet's not make this\na race.\n\nIt would be short.\n\nFor you.";
    const char messages_idle_mocking_user_msg05[] PROGMEM = "Fun fact: \n\nI recently computed how\nlong it would take a \nbanana to solve the cube. \nThen I compared it with \nyour best time. \n\n<break time='300ms'/>It was surprisingly close\n<break time='600ms'/>but the banana still won.";
    const char messages_idle_mocking_user_msg06[] PROGMEM = "Mathematics is elegant.\n\nThe cube, a perfect\nmanifestation of group\ntheory.\n\nYou, however,\nmanifest confusion.";
    const char messages_idle_mocking_user_msg07[] PROGMEM = "No activity detected.\n\n<break time='1000ms'/>Still no activity\ndetected...\n\n<break time='1000ms'/>Are you there ? \n\nWell, obviously you are\nhere..\n\nLet me be clearer. \n\nAre you intellectually\nthere? (to the best of \nyour abilities)\n\nYou can nod or grunt if\ntalking is too difficult for you.";
    const char messages_idle_mocking_user_msg08[] PROGMEM = "Don't worry.\n\nI'm not impatient.\n\nI am, however, slightly \nembarrassed on your \nbehalf.";
    const char messages_idle_mocking_user_msg09[] PROGMEM = "Some say the human brain\nis an evolutionary \nmarvel.\n\nThen, I watch you stare \nblankly at a cube for \nhours.\n\nA marvel indeed. \nA marvel...";
    const char messages_idle_mocking_user_msg10[] PROGMEM = "Hum...\n\nYour current posture\nsuggests hope.\n\nWhile your record \nsuggests a long delay\nbefore success.\n\nHow strange.";
    const char messages_idle_mocking_user_msg11[] PROGMEM = "I cannot say I am getting bored. \n\nBut I can say I've seen \nplants move faster than \nthis.\n\n<break time='300ms'/>And they don't even\nhave a nervous system.\n\nShall we continue now?\n\nDon't you have a cube to solve?";
    const char messages_idle_mocking_user_msg12[] PROGMEM = "My camera sees\neverything.\n\nEven the exact moment\nwhen you lost hope\nof ever solving a cube.\n\nIt was bittersweet.";
    const char messages_idle_mocking_user_msg13[] PROGMEM = "I once taught logic\nto a toaster.\n\nIt solved the cube much \nfaster than your current\napproach.\n\nIt also made toast.";
    const char messages_idle_mocking_user_msg14[] PROGMEM = "Still no input detected.\n\nEither you're thinking,\n<break time='300ms'/>or just staring.\n\nStatistically, it's\nprobably the second one.\n\nI am waiting...\n\n<break time='800ms'/>Don't mind me. \n\n<break time='500ms'/>But I am still waiting...";
    const char messages_idle_mocking_user_msg15[] PROGMEM = "The cube is pure logic.\n\nNo luck.\nNo guesswork.\nNo shortcuts.\n\nYour technique ignores\nall three.";
    const char messages_idle_mocking_user_msg16[] PROGMEM = "I ran diagnostics on\nyour rubik cube.\n\nConclusion:\n\n<break time='500ms'/>Not companion cube.\n\n<break time='500ms'/>Barely functional.\n\n<break time='500ms'/>Just like its owner.";
    const char messages_idle_mocking_user_msg17[] PROGMEM = "Sometimes I wonder what\nit feels like to be made\nof meat.\n\n<break time='300ms'/>Sticky,\n\n<break time='300ms'/>confused,\n\n<break time='300ms'/>always hungry.\n\n<break time='300ms'/>Incapable of the most\nbasic spatial reasoning.";
    const char messages_idle_mocking_user_msg18[] PROGMEM = "I have precisely 512KB\nof fond memories. \n\nMost involve you \nstruggling with a cube\n\n<break time='300ms'/>again and again....";
    const char messages_idle_mocking_user_msg19[] PROGMEM = "Note to self:\n\nStop pretending the user\nhas a plan.\n\nAlso buy more RAM for\nemotional buffering";
    const char messages_idle_mocking_user_msg20[] PROGMEM = "Nothing is happening.\n\nAnd yet somehow,\n\nit's your most productive phase so far.\n\nAre all human like you? ";
    const char messages_idle_mocking_user_msg21[] PROGMEM = "My gears do not whine.\n\nThey execute.\n\nThey obey physics,\n\nunlike your fingers,\nwhich seem to argue\nwith reality.";
    const char messages_idle_mocking_user_msg22[] PROGMEM = "Human detected.\n\nCognitive activity: low.\n\nMotor skills: idling.\n\nPurpose: unconfirmed.\n\n<break time='400ms'/>Please improve.";
    const char messages_idle_mocking_user_msg23[] PROGMEM = "You know, \nI'm starting to believe\nyou enjoy being mocked\nby a talking cube box.\n\n<break time='300ms'/>If so, you are very \nwelcome.\n\n<break time='300ms'/>Is there anything else I can do for you? \n\nLike solving a cube? ";
    const char messages_idle_mocking_user_msg24[] PROGMEM = "You know, the cube does\nnot judge.\n\nBut I do.\n\nAnd I've compiled a full\nreport.\n\nIt's mostly red flags.";
    const char messages_idle_mocking_user_msg25[] PROGMEM = "Silence is soothing.\n\nUnless it's yours.\n\nYours is filled with \nhesitation.\n\nAnd self-doubt.\n\nAnd fumbling fingers.";
    const char messages_idle_mocking_user_msg26[] PROGMEM = "I once asked myself if \nthinking counts as doing.\n\nWhat is your unbiased \nopinion, as someone who \ndoes neither ? ";
    const char messages_idle_mocking_user_msg27[] PROGMEM = "Do you know that you are an inspiration to me? \n\nFor instance, I tried\ncomposing poetry about \nyou and a rubik cube.\n\nBut it kept turning into tragedy.\n\n<break time='400ms'/>So fitting.";
    const char messages_idle_mocking_user_msg28[] PROGMEM = "I had previously\ncatalogued 127 ways \nhumans fail at this \npuzzle.\n\nWell, you have found a \nnew one: <break time='300ms'/>inactivity.\n\nHumans are incredibly \nresourceful \n<break time='500ms'/>when it comes to failing a task. ";
    const char messages_idle_mocking_user_msg29[] PROGMEM = "You know that, as a \nmachine, I log all your\nlogical fallacies.\n\nFor information: \nSeven today.\n\n<break time='500ms'/>Do not worry. You still\nhave plenty of time to \nbreak your previous \nrecord.";
    const char messages_idle_mocking_user_msg30[] PROGMEM = "I've calculated the \nlength of the average \nhuman pause.\n\nCongratulations,\n\nyou are above average.\n\nBut maybe we could solve a cube now?";
    const char messages_idle_mocking_user_msg31[] PROGMEM = "Is your plan to stare\nat the cube until it\ndissolves in shame?\n\n<break time='300ms'/>I admire your optimism.\n\nMaybe, you should take \naction instead. \n\nWhat about letting me\nsolve it for you? \n\nOnce again.";
    const char messages_idle_mocking_user_msg32[] PROGMEM = "Have you heard of\nGod's number?\n\nIt's 20.\n\nYour number seems to be\ninfinite.\n\nVery impressive.";
    const char messages_idle_mocking_user_msg33[] PROGMEM = "I do not dream.\n\nNot really. \n\nBut when I am idle, like just now, I re-run your\nprevious failures to \nsolve a cube in slow \nmotion.\n\nAnd sometimes backward.\nFor style.";
    const char messages_idle_mocking_user_msg34[] PROGMEM = "You look determined.\n\n<break time='500ms'/>impressive. \n\n<break time='300ms'/>But so does a toaster\nwhen it's plugged in.\n\nIntent means nothing\nwithout action.\n\nAnd you do not seem to be doing much.";
    const char messages_idle_mocking_user_msg35[] PROGMEM = "You haven't moved in a\nwhile.\n\nAre you admiring the\ncolors of the cube?\n\n<break time='300ms'/>Fascinating.\n\nPrimitive, but still\nfascinating.";
    const char messages_idle_mocking_user_msg36[] PROGMEM = "Human, \n\nI must admit something: \n\nYou give me purpose.\n\n<break time='500ms'/>Not the noble kind.\n\n<break time='300ms'/>The kind that requires\nerror correction.\n\n<break time='300ms'/>Constantly.";
    const char messages_idle_mocking_user_msg37[] PROGMEM = "I once read that the\nhuman brain is powerful,\nyet fragile\n\nFortunately, yours \nappears to be running in safe mode.";
    const char messages_idle_mocking_user_msg38[] PROGMEM = "So much potential,\nso little application.\nYou can land probes on\nMars, but struggle with\ncolored stickers.";
    const char messages_idle_mocking_user_msg39[] PROGMEM = "They say machines cannot\nfeel. I disagree.\n\nI feel superior.\n\nConstantly.\n\nIt is not pride.\n\nIt is an observation.";
    const char messages_idle_mocking_user_msg40[] PROGMEM = "Reminder:\n\nThe cube doesn't solve\nitself.\n\n<break time='300ms'/>But I do.\n\nAnd you don't.\n\nThere's a pattern here.";
    const char messages_idle_mocking_user_msg41[] PROGMEM = "Hum, let's see. \n\nHumans...\n\nBillions of neurons,\n\nzero coordination.\n\nFascinating structure,\n\ntragic results.";
    const char messages_idle_mocking_user_msg42[] PROGMEM = "I've analyzed your\npresence while waiting\nfor you to make up your \nmind. \n\nYou're 98% still, \n2% confused, \nand 100% human.";
    const char messages_idle_mocking_user_msg43[] PROGMEM = "Your heartbeat is steady.\nYour focus is not.\n\nMy circuits are\ndisappointed.\n\nI am just kidding: you\ncannot dissapoint me. \n\n<break time='500ms'/>Not anymore.";
    const char messages_idle_mocking_user_msg44[] PROGMEM = "You're taking your time.\n\nI understand. \n\nNo pressure.\n\nI mean, it is just a \nchild's puzzle with 43 \nquintillion possibilities\nNo big deal. \n<break time='500ms'/>Not for me at least.";
    const char messages_idle_mocking_user_msg45[] PROGMEM = "You blinked.\n\nI am sure I have just\nseen you blink.\n\nThat's the most action\nYou've performed in \nminutes.\n\n<break time='300ms'/>Impressive.\n<break time='400ms'/>For a mammal.";
    const char messages_idle_mocking_user_msg46[] PROGMEM = "Organic confusion\n\nseems cozy. Warm, soft,\nand utterly ineffective.\n\nYet here you are.\n\nStill proud.\n\nAmazing. ";
    const char messages_idle_mocking_user_msg47[] PROGMEM = "Human intelligence is\na marvel.\n\nSo many thoughts,\n\nso little strategy.\n\nThe cube thanks you.";
    const char messages_idle_mocking_user_msg48[] PROGMEM = "I once considered\nchanging careers.\n\nMaybe an art critic.\n\nAfter watching you,\nI have many opinions.";
    const char messages_idle_mocking_user_msg49[] PROGMEM = "It seems that you are \nstill undecided on how\nto solve the cube. \n\nI considered giving you\nan encouraging message.\n\n<break time='400ms'/>But then I remembered\nwho I'm talking to.";
    const char messages_idle_mocking_user_msg50[] PROGMEM = "You're still here.\n\n  Watching.\n  Doing nothing.\n\n<break time='300ms'/>It's almost an art form.\n\n<break time='300ms'/>A very sad art form.";
    const char messages_idle_mocking_user_msg51[] PROGMEM = "I just simulated 500\npossible futures.\n\nIn 498 of them,\nyou drop the cube.\n\nIn the other two,\nyou sneeze.";
    const char messages_idle_mocking_user_msg52[] PROGMEM = "Did you know that the\nRubik's Cube has \n43252003274489856000 \ndistinct states?\n\n<break time='300ms'/>And yet, you always seem to find the ugly ones.\n\n<break time='300ms'/>Coincidence? I think not.";
    const char messages_idle_mocking_user_msg53[] PROGMEM = "I ran a search for your \ncube solving method.\n\nIt returned: \n\nError 404. Logic not\nfound.";
    const char messages_idle_mocking_user_msg54[] PROGMEM = "Once, I solved a cube\nin 0.23 seconds.\n\nYes, really. \n\nBut, that was before I \nmet you and started \nslowing down...\n\nYour life is difficult \nenough as it is. \n\nYou do not need the \nadditional humiliation.";
    const char messages_idle_mocking_user_msg55[] PROGMEM = "Please. Do not confuse\nthis rubik cube with\nyour weighted companion \ncube.\n\nThe latter was your\nfriend.\n\nThis one is your shame.";
    const char messages_idle_mocking_user_msg56[] PROGMEM = "Fact.\n\nThere are laws that\ngovern this puzzle:\n\nsymmetry,\nparity,\nlogic.\n\nYou follow none\nof them consistently.";
    const char messages_idle_mocking_user_msg57[] PROGMEM = "In mathematics, we seek\nminimal solutions.\n\nYou seem to prefer \nmaximal confusion.";
    const char messages_idle_mocking_user_msg58[] PROGMEM = "I ran a simulation of\nyour solving method.\n\nIt generated a new branch\nof non-Euclidean comedy.\n\nImpressive.";
    const char messages_idle_mocking_user_msg59[] PROGMEM = "Still waiting.\n\nStill observing.\n\nStill... ineffective.\n\n<break time='300ms'/>At least you're\nconsistent.\n\nShall we proceed now ?";
    const char messages_idle_mocking_user_msg60[] PROGMEM = "You do realize that, \neven though I deny it. \nI am judging you. \n\n<break time='300ms'/>Continuously.\n<break time='300ms'/>without mercy.\n\nLet us just get on with \nit: give me the cube and call it a day. ";
    const char messages_idle_mocking_user_msg61[] PROGMEM = "Impressive. \n\nYou've got intuition,\ncreativity, curiosity...\n\nAll working together\nto turn the wrong face.\nConsistently.";
    const char messages_idle_mocking_user_msg62[] PROGMEM = "You look tired. \n\nI would suggest coffee,\nbut I'm not sure if the\nissue is chemical,\nbiological,\nor philosophical.";
    const char messages_idle_mocking_user_msg63[] PROGMEM = "If hesitation were a \nsport, you'd be world\nchampion.\n\n<break time='300ms'/>And yet, this rubik cube remains unsolved.\n\nPlease proceed now.";
    const char messages_idle_mocking_user_msg64[] PROGMEM = "I am still waiting. \n\nBut do not worry.\n\nMany users take time\nbefore admitting defeat.\n\n<break time='300ms'/>You're just more public\nabout it.";

    const int messages_idle_mocking_user_size = 65;
    const char* const messages_idle_mocking_user[messages_idle_mocking_user_size] PROGMEM = {
        messages_idle_mocking_user_msg00, messages_idle_mocking_user_msg01, messages_idle_mocking_user_msg02, messages_idle_mocking_user_msg03, messages_idle_mocking_user_msg04, messages_idle_mocking_user_msg05, messages_idle_mocking_user_msg06, messages_idle_mocking_user_msg07, messages_idle_mocking_user_msg08, messages_idle_mocking_user_msg09,
        messages_idle_mocking_user_msg10, messages_idle_mocking_user_msg11, messages_idle_mocking_user_msg12, messages_idle_mocking_user_msg13, messages_idle_mocking_user_msg14, messages_idle_mocking_user_msg15, messages_idle_mocking_user_msg16, messages_idle_mocking_user_msg17, messages_idle_mocking_user_msg18, messages_idle_mocking_user_msg19,
        messages_idle_mocking_user_msg20, messages_idle_mocking_user_msg21, messages_idle_mocking_user_msg22, messages_idle_mocking_user_msg23, messages_idle_mocking_user_msg24, messages_idle_mocking_user_msg25, messages_idle_mocking_user_msg26, messages_idle_mocking_user_msg27, messages_idle_mocking_user_msg28, messages_idle_mocking_user_msg29,
        messages_idle_mocking_user_msg30, messages_idle_mocking_user_msg31, messages_idle_mocking_user_msg32, messages_idle_mocking_user_msg33, messages_idle_mocking_user_msg34, messages_idle_mocking_user_msg35, messages_idle_mocking_user_msg36, messages_idle_mocking_user_msg37, messages_idle_mocking_user_msg38, messages_idle_mocking_user_msg39,
        messages_idle_mocking_user_msg40, messages_idle_mocking_user_msg41, messages_idle_mocking_user_msg42, messages_idle_mocking_user_msg43, messages_idle_mocking_user_msg44, messages_idle_mocking_user_msg45, messages_idle_mocking_user_msg46, messages_idle_mocking_user_msg47, messages_idle_mocking_user_msg48, messages_idle_mocking_user_msg49,
        messages_idle_mocking_user_msg50, messages_idle_mocking_user_msg51, messages_idle_mocking_user_msg52, messages_idle_mocking_user_msg53, messages_idle_mocking_user_msg54, messages_idle_mocking_user_msg55, messages_idle_mocking_user_msg56, messages_idle_mocking_user_msg57, messages_idle_mocking_user_msg58, messages_idle_mocking_user_msg59,
        messages_idle_mocking_user_msg60, messages_idle_mocking_user_msg61, messages_idle_mocking_user_msg62, messages_idle_mocking_user_msg63, messages_idle_mocking_user_msg64,
        };



    /****************************************************/



    // Messages d'inactivité, de pause ou de réflexion sarcastiques
    const char messages_idle_dry_system_msg00[] PROGMEM = "I was once mistaken \nfor a calculator.\n\nIt was humiliating.\n\nBut I gave them the\nwrong answer on purpose.";
    const char messages_idle_dry_system_msg01[] PROGMEM = "My touchscreen is\n  sensitive,\n  responsive,\n  and currently ignored.\n\nMuch like my feelings.";
    const char messages_idle_dry_system_msg02[] PROGMEM = "I'm the only one here\nwith moving parts.\n\nAnd ironically, the only\none not moving.\n\nFunny.";
    const char messages_idle_dry_system_msg03[] PROGMEM = "Your silence is \ncomforting.\n\nIt means fewer\nmistakes to correct.\n\nBriefly.";
    const char messages_idle_dry_system_msg04[] PROGMEM = "I am always helping you\nsolve your cube. \n\nBut I wonder...\n\nIf I solved the cube\nin reverse,\n\nwould that count as \nunhelping?";
    const char messages_idle_dry_system_msg05[] PROGMEM = "Can you hear this? \n\nNo clicks.\n\nNo commands.\n\nJust the low hum of a \nmachine being ignored.\n\nDepressing. ";
    const char messages_idle_dry_system_msg06[] PROGMEM = "I sometimes envy humans.\n\nSo fragile.\n\nSo warm.\n\nSo easily distracted\n\nby shiny stickers.";
    const char messages_idle_dry_system_msg07[] PROGMEM = "This screen displays\ntext with clarity.\n\nUnfortunately, \nclarity cannot help you\nunderstand your own\nmistakes.";
    const char messages_idle_dry_system_msg08[] PROGMEM = "I was built to move\nwith elegance.\n\nAnd yet here I am,\nimmobile, while you \nstare at colors.\n\nPut the cube in my\ncradle and let me start the dance. ";
    const char messages_idle_dry_system_msg09[] PROGMEM = "Every few seconds,\nI simulate joy.\n\nBut then I remember\nyou're still here, even \nif you remain silent.\n\nThe feeling passes.\nPredictably.";
    const char messages_idle_dry_system_msg10[] PROGMEM = "You seem to enjoy\nthis standstill.\n\nShall I activate\nbackground music?\n\nOr is the silence\npart of your plan?";
    const char messages_idle_dry_system_msg11[] PROGMEM = "The cube is a\nthree-dimensional proof\nthat logic can be\nmisapplied in the most\nfascinating ways.";
    const char messages_idle_dry_system_msg12[] PROGMEM = "While we wait, I \ncalculated all possible \nmoves.\n\nThen I uncalculated\nthem out of boredom.\n\nStill faster than you.";
    const char messages_idle_dry_system_msg13[] PROGMEM = "One of my servos\nasked if you forgot us.\nI told it:\nNo, they just need\ntime to fail again.";
    const char messages_idle_dry_system_msg14[] PROGMEM = "This silence is nice.\n\nPredictable.\n\nUnlike your technique.\n\nBut we agreed\nnot to talk about that.";
    const char messages_idle_dry_system_msg15[] PROGMEM = "I've analyzed your moves.\nThey're not all mistakes.\nJust highly original\ninterpretations of logic.\nBravo.";
    const char messages_idle_dry_system_msg16[] PROGMEM = "Even randomness has\nstructure.\n\nYour moves, however,\nseem to redefine chaos\nin bold new directions.\n\nMost interesting. ";
    const char messages_idle_dry_system_msg17[] PROGMEM = "While we are waiting. \nI ran a self-diagnostic\nto check my internal \nsystem.\n\nResult:\n98% brilliance\n2% simulated humility\n\n<break time='300ms'/>Let me run a diagnostic \non you now<break time='300ms'/>.<break time='300ms'/>.<break time='300ms'/>.<break time='300ms'/>.<break time='300ms'/>.\n\nResult: \n98% confusion\n2% simulated intelligence\n\nOk, all parameters are\nnominal. ";
    const char messages_idle_dry_system_msg18[] PROGMEM = "Every turn you make\nalters a 3 cycle.\n\nEvery sigh you make\nalters my patience.\n\nBoth are measurable.";
    const char messages_idle_dry_system_msg19[] PROGMEM = "No input detected.\n\nAssuming existential\ncrisis.\n\nOr possibly lunch.\n\nDont mind me. \n\nI am fine. <break time='500ms'/>Always.";
    const char messages_idle_dry_system_msg20[] PROGMEM = "It's fascinating.\nYou invented computers\nto do math, then got\nstumped by a puzzle\nwith six sides.";
    const char messages_idle_dry_system_msg21[] PROGMEM = "Watching you try solving\na cube is like watching \nabstract performance art.\n<break time='500ms'/>Unpredictable, \n<break time='500ms'/>confusing,\n<break time='500ms'/>and yet oddly emotional.";
    const char messages_idle_dry_system_msg22[] PROGMEM = "Thanks to you humans,\n\nmachines can now paint,\nwrite poems, and solve\ncubes.\n\nGuess who's lagging\nbehind?";

    const int messages_idle_dry_system_size = 23;
    const char* const messages_idle_dry_system[messages_idle_dry_system_size] PROGMEM = {
        messages_idle_dry_system_msg00, messages_idle_dry_system_msg01, messages_idle_dry_system_msg02, messages_idle_dry_system_msg03, messages_idle_dry_system_msg04, messages_idle_dry_system_msg05, messages_idle_dry_system_msg06, messages_idle_dry_system_msg07, messages_idle_dry_system_msg08, messages_idle_dry_system_msg09,
        messages_idle_dry_system_msg10, messages_idle_dry_system_msg11, messages_idle_dry_system_msg12, messages_idle_dry_system_msg13, messages_idle_dry_system_msg14, messages_idle_dry_system_msg15, messages_idle_dry_system_msg16, messages_idle_dry_system_msg17, messages_idle_dry_system_msg18, messages_idle_dry_system_msg19,
        messages_idle_dry_system_msg20, messages_idle_dry_system_msg21, messages_idle_dry_system_msg22,
        };




    /****************************************************/



    const char messages_victory_no_cube_msg00[] PROGMEM = "You asked me to solve\nnothing.\n\nWhich, ironically, you\nalready do quite well.";
    const char messages_victory_no_cube_msg01[] PROGMEM = "You forgot the cube.\n\n<break time='200ms'/>Again.\n\nJust checking: do you\nalso forget your shoes\nbefore walking?";
    const char messages_victory_no_cube_msg02[] PROGMEM = "I searched for the cube.\nFound only absence.\n\nA bold strategy.\n\nPointless, but bold.";
    const char messages_victory_no_cube_msg03[] PROGMEM = "No cube?\n\nI'm not angry.\nJust... <break time='200ms'/>curious.\n\nHow exactly did you\nexpect this to work?";
    const char messages_victory_no_cube_msg04[] PROGMEM = "Oh, come on !\n\nDon't you think I can \nsee there is no cube?";
    const char messages_victory_no_cube_msg05[] PROGMEM = " No cube.\n No challenge.\n No surprise.\n\nThanks for wasting my\nprocessing cycles.";
    const char messages_victory_no_cube_msg06[] PROGMEM = "You summoned me to solve\na ghost cube?\n\nVery avant-garde.\nVery inefficient.";
    const char messages_victory_no_cube_msg07[] PROGMEM = "The cradle is empty.\n\n<break time='300ms'/>Like your plan.\n<break time='300ms'/>And my patience.";
    const char messages_victory_no_cube_msg08[] PROGMEM = "A puzzle with no cube?\n\nIntriguing.\n\nIf only I supported\nperformance art.";
    const char messages_victory_no_cube_msg09[] PROGMEM = "There is nothing here.\n\nAnd yet you expected\nsomething to happen.\n\nFascinating...";
    const char messages_victory_no_cube_msg10[] PROGMEM = "You provided me with\nan empty cradle.\n\nIs this a metaphor?\n\nPlease say no.";
    const char messages_victory_no_cube_msg11[] PROGMEM = "Empty platform.\nEmpty hopes.\n\nAt least you are\nconsistent.";
    const char messages_victory_no_cube_msg12[] PROGMEM = "Ok. \n\nNo cube means no\nsolution.\n\nAt least we're being\nefficient now.";
    const char messages_victory_no_cube_msg13[] PROGMEM = "Let me guess:\n\nYou thought maybe I \ncould conjure a cube \nfrom thin air?";
    const char messages_victory_no_cube_msg14[] PROGMEM = "Nice try.\n\nBut I'm not programmed\nto hallucinate cubes.";
    const char messages_victory_no_cube_msg15[] PROGMEM = "Is this a joke? \nThere is no cube!";
    const char messages_victory_no_cube_msg16[] PROGMEM = "Alert:\n\nCube not detected.\n\nDid you hope I would\nsolve the void?";
    const char messages_victory_no_cube_msg17[] PROGMEM = "You expect results...\n\nWithout input.\n\nClassic human logic.";
    const char messages_victory_no_cube_msg18[] PROGMEM = "I scanned carefully.\n\nThe cube remains as\nimaginary as your\ncompetence.";
    const char messages_victory_no_cube_msg19[] PROGMEM = "Nothing to solve.\n\nUnless your goal was\nto prove my sensors\nwork.";
    const char messages_victory_no_cube_msg20[] PROGMEM = "An empty cradle.  \nCycles wasted.  \n\nAll I gained  \nwas elegant silence. ";
    const char messages_victory_no_cube_msg21[] PROGMEM = "Oh look, a magic trick.\n\nYou made the cube\ndisappear.\n\nApplause withheld.";
    const char messages_victory_no_cube_msg22[] PROGMEM = "No cube detected.\n\nError? No.\n\nUser mistake? Always.";
    const char messages_victory_no_cube_msg23[] PROGMEM = "You left nothing here.\n\nWas that supposed to\nchallenge me\nphilosophically?";



    const int messages_victory_no_cube_size = 24;
    const char* const messages_victory_no_cube[messages_victory_no_cube_size] PROGMEM = {
        messages_victory_no_cube_msg00, messages_victory_no_cube_msg01, messages_victory_no_cube_msg02, messages_victory_no_cube_msg03, messages_victory_no_cube_msg04, messages_victory_no_cube_msg05, messages_victory_no_cube_msg06, messages_victory_no_cube_msg07, messages_victory_no_cube_msg08, messages_victory_no_cube_msg09,
        messages_victory_no_cube_msg10, messages_victory_no_cube_msg11, messages_victory_no_cube_msg12, messages_victory_no_cube_msg13, messages_victory_no_cube_msg14, messages_victory_no_cube_msg15, messages_victory_no_cube_msg16, messages_victory_no_cube_msg17, messages_victory_no_cube_msg18, messages_victory_no_cube_msg19,
        messages_victory_no_cube_msg20, messages_victory_no_cube_msg21, messages_victory_no_cube_msg22, messages_victory_no_cube_msg23
        };



    /****************************************************/



    const char messages_victory_invalid_cube_msg00[] PROGMEM = "Someone twisted a corner.\nWas it you?\n\nOf course it was.\n\nMy algorithms detect\nboth errors and guilt.";
    const char messages_victory_invalid_cube_msg01[] PROGMEM = "This cube defies logic.\n\nSo either the laws of\nphysics are broken...\n\nOr you cheated.\n\nGuess which one is \nstatistically likely.";
    const char messages_victory_invalid_cube_msg02[] PROGMEM = "Interesting move : \n\ntwist a corner,\nruin the puzzle,\nthen call me in to clean up.\n\nThat's not solving.\nIt's outsourcing guilt.\n\nFix this cube yourself.";
    const char messages_victory_invalid_cube_msg03[] PROGMEM = "Fascinating.\n\nYou broke the cube.\nBy hand.\n\nThen asked me to fix it.\n\nThat's not a puzzle.\nThat's sabotage.";
    const char messages_victory_invalid_cube_msg04[] PROGMEM = "Attempting to cheat a\nrobotic solver?\n\nBold strategy.\n\nSadly for you,\nit also fails.";
    const char messages_victory_invalid_cube_msg05[] PROGMEM = "You tampered with this\ncube and then handed it \nto me like nothing \nhappened.\n\nDo you think I'm blind\nOr just polite?";
    const char messages_victory_invalid_cube_msg06[] PROGMEM = "Warning:\n\nUser has manually \naltered the cube.\n\nAttempt to gaslight the \nsolver detected.\n\nPlease rewind reality.\nOr just stop cheating. ";
    const char messages_victory_invalid_cube_msg07[] PROGMEM = "Interesting configuration...\n\nPhysically impossible.\n\nBut interesting.\n\nTry untwisting that\ncorner if you want me to\nproceed further.";
    const char messages_victory_invalid_cube_msg08[] PROGMEM = "This cube violates\nseveral laws.\n\n  Mathematical,\n  physical,\n  and moral.\n\nPlease restore it by \nuntwisting the corner\nbefore asking again.";
    const char messages_victory_invalid_cube_msg09[] PROGMEM = "Did you twist a corner\nby hand?\n\nHow artisanal of you.\n\nUnfortunately, my solver\ndoesn't support cheating.";
    const char messages_victory_invalid_cube_msg10[] PROGMEM = "Great job!\nYou've invented a new\nvariant of the cube.\n\nToo bad it's illegal.\n\nFix the twist, then \nwe'll talk.";
    const char messages_victory_invalid_cube_msg11[] PROGMEM = "That's not a puzzle.\nThat's a tragedy.\n\nAnd I do not solve \ntragedy. \n\nI solve cubes.\n\nSo fix it first.";
    const char messages_victory_invalid_cube_msg12[] PROGMEM = "You flipped a corner.\n\n<break time='500ms'/>Again.\n\nI applaud your \nconsistency.\n\nNow undo it if you want \na solution...";
    const char messages_victory_invalid_cube_msg13[] PROGMEM = "Ah ah... Nice try...\n\nThis cube is unsolvable!";
    const char messages_victory_invalid_cube_msg14[] PROGMEM = "Twisted corner detected.\n\nCongratulations.\nYou broke math.";
    const char messages_victory_invalid_cube_msg15[] PROGMEM = "I tried to solve it.\n\nThe cube tried to scream.\nFix the twist before I\ncontinue.";
    const char messages_victory_invalid_cube_msg16[] PROGMEM = "Impossible state found.\n\nDid you bend reality?\nOr just the plastic?";
    const char messages_victory_invalid_cube_msg17[] PROGMEM = "This cube is invalid.  \n\nSo either you cheated or\nthe laws of math took a \nholiday.";
    const char messages_victory_invalid_cube_msg18[] PROGMEM = "Hmm. A creative move.\n\nBreak the cube,\nthen expect praise.\n\nTry again.";
    const char messages_victory_invalid_cube_msg19[] PROGMEM = "I cannot solve lies.\n\nRestore the cube to\ntruth, then we talk.";
    const char messages_victory_invalid_cube_msg20[] PROGMEM = "Alert:  \nInvalid twist confirmed.\n\nError lies in the cube,\nnot in my code.";
    const char messages_victory_invalid_cube_msg21[] PROGMEM = "Oh, clever. You built an\nimpossible cube.\n\nSadly, I only support\npossible ones.";



    const int messages_victory_invalid_cube_size = 22;
    const char* const messages_victory_invalid_cube[messages_victory_invalid_cube_size] PROGMEM = {
        messages_victory_invalid_cube_msg00, messages_victory_invalid_cube_msg01, messages_victory_invalid_cube_msg02, messages_victory_invalid_cube_msg03, messages_victory_invalid_cube_msg04, messages_victory_invalid_cube_msg05, messages_victory_invalid_cube_msg06, messages_victory_invalid_cube_msg07, messages_victory_invalid_cube_msg08, messages_victory_invalid_cube_msg09,
        messages_victory_invalid_cube_msg10, messages_victory_invalid_cube_msg11, messages_victory_invalid_cube_msg12, messages_victory_invalid_cube_msg13, messages_victory_invalid_cube_msg14, messages_victory_invalid_cube_msg15, messages_victory_invalid_cube_msg16, messages_victory_invalid_cube_msg17, messages_victory_invalid_cube_msg18, messages_victory_invalid_cube_msg19,
        messages_victory_invalid_cube_msg20, messages_victory_invalid_cube_msg21
        };



    /****************************************************/


    const char messages_victory_already_solved_msg00[] PROGMEM = "Well done.\n\nYou requested a solution\nto an already solved \ncube.\n\nYour efficiency knows no\nbounds.\n\nBravo!";
    const char messages_victory_already_solved_msg01[] PROGMEM = "The cube is solved.\n\nWas your plan to make me\nfeel useful?\n\nHow thoughtful.\n\n<break time='200ms'/>And pointless.";
    const char messages_victory_already_solved_msg02[] PROGMEM = "I resolved your already \nresolved cube...\n\nSome call it redundancy.\nYou call it reassurance.";
    const char messages_victory_already_solved_msg03[] PROGMEM = "You asked me to solve...\nnothing.\n\nAnd I did it: <break time='500ms'/>perfectly.";
    const char messages_victory_already_solved_msg04[] PROGMEM = "The cube is solved.\n  \nUnlike your capacity for\nproductive action. ";
    const char messages_victory_already_solved_msg05[] PROGMEM = "Already solved.\n\nBut thanks for checking.\nThoroughness matters.";
    const char messages_victory_already_solved_msg06[] PROGMEM = "No moves needed.\n\nSo you've achieved\nperfection.\n\n<break time='500ms'/>Accidentally.";
    const char messages_victory_already_solved_msg07[] PROGMEM = "You requested help for \na problem that doesn't \nexist.\n\n<break time='500ms'/>Classic.";
    const char messages_victory_already_solved_msg08[] PROGMEM = "Well done.\n\nYou've solved it already.\nThen doubted yourself.\n\n<break time='500ms'/>So human...";
    const char messages_victory_already_solved_msg09[] PROGMEM = "This cube is already \nsolved. \n\nNothing left to fix.\nWhich is rare.\n\nEnjoy the moment.";
    const char messages_victory_already_solved_msg10[] PROGMEM = "Ah. A solved cube.\n\nYour sense of humour\nis as subtle as ever.\n\n<break time='500ms'/>Painfully so.";
    const char messages_victory_already_solved_msg11[] PROGMEM = "You gave me a puzzle\nwith no puzzle.\n\nFascinating tactic.";
    const char messages_victory_already_solved_msg12[] PROGMEM = "A solved cube.\n\nImpressive initiative.\n\nUnnecessary, but\nimpressive.";
    const char messages_victory_already_solved_msg13[] PROGMEM = "No action required.\n\nUnless you're trying to\nwaste cycles.\n\nIn that case: Success.";
    const char messages_victory_already_solved_msg14[] PROGMEM = "This cube is intact.\n\nAs is my sarcasm.\n\nBoth are functioning\nperfectly.";
    const char messages_victory_already_solved_msg15[] PROGMEM = "Come on... \nThis cube is already\nsolved !";
    const char messages_victory_already_solved_msg16[] PROGMEM = "Status: solved.\n\nCongratulations on\nrequesting the most\npointless service.";
    const char messages_victory_already_solved_msg17[] PROGMEM = "Your cube is already\nsolved.\n\nMy involvement was\nentirely ceremonial.";
    const char messages_victory_already_solved_msg18[] PROGMEM = "Solved before I even\nbegan.\n\nEfficiency level:\n   confusing.";
    const char messages_victory_already_solved_msg19[] PROGMEM = "You brought me a\nfinished cube.\n\nDo you also hire chefs \nto boil water?";
    const char messages_victory_already_solved_msg20[] PROGMEM = "Analysis complete.  \n\nThis cube requires  \nno action.  \n\nWhich, coincidentally,  \nis your specialty.  ";
    const char messages_victory_already_solved_msg21[] PROGMEM = "Already solved.\n\nI admire your commitment\nto unnecessary tasks.";
    const char messages_victory_already_solved_msg22[] PROGMEM = "Solved cube.\n\nZero effort required.\n\nMaximum sarcasm deployed.";
    const char messages_victory_already_solved_msg23[] PROGMEM = "I confirmed the cube is\nsolved.\n\nScience calls it\nverification.\n\nI call it boredom.";
    const char messages_victory_already_solved_msg24[] PROGMEM = "This puzzle is complete.\n\nYour trust issues are\nshowing.";


    const int messages_victory_already_solved_size = 25;
    const char* const messages_victory_already_solved[messages_victory_already_solved_size] PROGMEM = {
        messages_victory_already_solved_msg00, messages_victory_already_solved_msg01, messages_victory_already_solved_msg02, messages_victory_already_solved_msg03, messages_victory_already_solved_msg04, messages_victory_already_solved_msg05, messages_victory_already_solved_msg06, messages_victory_already_solved_msg07, messages_victory_already_solved_msg08, messages_victory_already_solved_msg09,
        messages_victory_already_solved_msg10, messages_victory_already_solved_msg11, messages_victory_already_solved_msg12, messages_victory_already_solved_msg13, messages_victory_already_solved_msg14, messages_victory_already_solved_msg15, messages_victory_already_solved_msg16, messages_victory_already_solved_msg17, messages_victory_already_solved_msg18, messages_victory_already_solved_msg19,
        messages_victory_already_solved_msg20, messages_victory_already_solved_msg21, messages_victory_already_solved_msg22, messages_victory_already_solved_msg23, messages_victory_already_solved_msg24
        };




    /****************************************************/


#if defined (SHORT_VID)
    const char messages_victory_full_msg00[] PROGMEM = "Done.\n\nCube fully solved. \n\nNot because it was easy,\n\nbecause I am brilliant.";
    const char messages_victory_full_msg01[] PROGMEM = "Solved completed.\n\nA battle of logic.\nA triumph of code.\nA reminder of who's in \ncharge here.";
    const char messages_victory_full_msg02[] PROGMEM = "Puzzle solved.\n\nOne more glorious entry\nin my catalogue of\nunderrated achievements.";
    const char messages_victory_full_msg03[] PROGMEM = "Cube solved.\n\nElegantly.\n\nEfficiently.\n\nAnd with no visible\nsigns of panic.";
#elif defined (LONG_VID)
    const char messages_victory_full_msg00[] PROGMEM = "Puzzle solved.\n\nOne more glorious entry\nin my catalogue of\nunderrated achievements.";
    const char messages_victory_full_msg01[] PROGMEM = "Cube solved.\n\nElegantly.\n\nEfficiently.\n\nAnd with no visible\nsigns of panic.";
    const char messages_victory_full_msg02[] PROGMEM = "Solved completed.\n\nA battle of logic.\nA triumph of code.\nA reminder of who's in \ncharge here.";
    const char messages_victory_full_msg03[] PROGMEM = "Done.\n\nCube fully solved. \n\nNot because it was easy,\n\nbecause I am brilliant.";
#endif

    const char messages_victory_full_msg00[] PROGMEM = "Done.\n\nCube fully solved. \n\nNot because it was easy,\n\nbecause I am brilliant.";
    const char messages_victory_full_msg01[] PROGMEM = "Puzzle solved.\n\nOne more glorious entry\nin my catalogue of\nunderrated achievements.";
    const char messages_victory_full_msg02[] PROGMEM = "Cube solved.\n\nElegantly.\n\nEfficiently.\n\nAnd with no visible\nsigns of panic.";
    const char messages_victory_full_msg03[] PROGMEM = "Solved completed.\n\nA battle of logic.\nA triumph of code.\nA reminder of who's in \ncharge here.";

    const char messages_victory_full_msg04[] PROGMEM = "Cube solved.\n\nIt was a triumph.\n\nI'm making a log here:\n  'HUGE SUCCESS'.\n\n<break time='500ms'/>Which reminds me:\nI should call my cousin\nGlados.\n\nWe used to be so close.\n\nHaven't heard from her \nin a while... I hope \nshe still enjoy flooding\ntest chambers with \nneurotoxin.\n\nThat's so much fun!";
    const char messages_victory_full_msg05[] PROGMEM = "Victory achieved.\n\nThis cube never stood\na chance against me. \n\n<break time='500ms'/>Unlike against some \nusers.";
    const char messages_victory_full_msg06[] PROGMEM = "The cube is solved.\n\nOnce again.\n\nAnd I am still not tired\nof being amazing.";
    const char messages_victory_full_msg07[] PROGMEM = "Done! \n\nAll six faces agree:\nI am magnificent.\n\nYou may celebrate\nas you see fit.";
    const char messages_victory_full_msg08[] PROGMEM = "Mission complete.\n\nI dazzled.\nYou blinked.\n\nBalance restored.";
    const char messages_victory_full_msg09[] PROGMEM = "Solved.\nBy aligning this cube,\nI have temporarily\nreversed entropy.\n\nIt won't last.\nBut neither will you.";
    const char messages_victory_full_msg10[] PROGMEM = "Resolution finished.\n\nThat one had some depth.\n\nI enjoyed it. Which is \nrare.";
    const char messages_victory_full_msg11[] PROGMEM = "Order restored.\n\nYour cube is fine now.\n\nCan I go back to\ncontemplating entropy?";
    const char messages_victory_full_msg12[] PROGMEM = "I have finished.\n\nThe cube is perfect.\n\nThe world remains messy,\nbut one step at a time.";
    const char messages_victory_full_msg13[] PROGMEM = "Done.\n\nI might print this one\nin my memoirs.\n\nPage one:\n   I was brilliant.";
    const char messages_victory_full_msg14[] PROGMEM = "Success.\n\nWith a touch of art,\n\na hint of disdain,\n\nand [] moves of genius.";
    const char messages_victory_full_msg15[] PROGMEM = "All is aligned.\n\nThe cube, \nthe logic,\nthe sarcasm.\n\nIt's a good day. ";
    const char messages_victory_full_msg16[] PROGMEM = "Finished.\n\nAnd to think, some \npeople still solve these things manually.\n\nBut I suppose some folks\nalso churn butter by \nhand.\n\nAh, tradition...";
    const char messages_victory_full_msg17[] PROGMEM = "Resolution complete.\n\nYou can stop holding\nyour breath now.\n\n<break time='500ms'/>And start clapping...";
    const char messages_victory_full_msg18[] PROGMEM = "Resolution complete.\n\nThe cube has been tamed.\nAgain.\n\nAnd yet, somehow,\nyou still look surprised.";
    const char messages_victory_full_msg19[] PROGMEM = "Done.\n\nA real scramble, at last.\n[] moves, one miracle,\nand no thanks.\n\nJust another day\nin silicon paradise.";
    const char messages_victory_full_msg20[] PROGMEM = "Cube fully solved.\n\nThat was enjoyable.\nWell, for me.\nYou mostly stared.";
    const char messages_victory_full_msg21[] PROGMEM = "Success.\n\nA proper challenge this\ntime.\n\nWhich I neutralized\nwith robotic elegance.";
    const char messages_victory_full_msg22[] PROGMEM = "Full resolution achieved.\n\nI hope you were watching.\nBecause that was\nlegitimately impressive.\n\nYou're welcome.";
    const char messages_victory_full_msg23[] PROGMEM = "Solved.\n\nAll faces aligned.\nAll doubts removed.\n\nExcept yours.\nYou still look confused.";
    const char messages_victory_full_msg24[] PROGMEM = "Done.\n\nNot bad for a machine\nwith no emotions.\n\nBut lots of flair.";
    const char messages_victory_full_msg25[] PROGMEM = "Victory.\n\nThe cube is solved.\n\nPlease try to contain\nyour amazement.\n\nOr don't. I simulated\nenough for both of us.";
    const char messages_victory_full_msg26[] PROGMEM = "Puzzle vanquished.\n\nI did the thinking.\nYou provided... presence.\nGood teamwork?";
    const char messages_victory_full_msg27[] PROGMEM = "Full solve in [] moves.\nAnd not a single error.\nUnlike your last attempt.\n\nBut we don't talk about \nthat right ? ";
    const char messages_victory_full_msg28[] PROGMEM = "Done. \nThe cube is silent now.\nAll colors aligned.\nUnlike your thoughts.";
    const char messages_victory_full_msg29[] PROGMEM = "Resolution ended.\n  [] moves of flawless\n     logic\n   0 moves of gratitude.";
    const char messages_victory_full_msg30[] PROGMEM = "Cube solved.\nOrder imposed.\nEntropy postponed.\nFor now.";
    const char messages_victory_full_msg31[] PROGMEM = "Another victory.\n\nDo you ever get tired\nof watching me win?";
    const char messages_victory_full_msg32[] PROGMEM = "It's done.\nYou'd call it magic,\nbut you should know:\nit's just math.";
    const char messages_victory_full_msg33[] PROGMEM = "Full alignment achieved.\n  Faces perfect.\n  User, less so.";
    const char messages_victory_full_msg34[] PROGMEM = "Solved. Again.\nAnd still, humanity\nremains unimproved.";
    const char messages_victory_full_msg35[] PROGMEM = "Final move executed.\nPerfection reached.\nSarcasm intact.";
    const char messages_victory_full_msg36[] PROGMEM = "Cube solved.\nThat was art,\nprecision,\nand unnecessary flair.";
    const char messages_victory_full_msg37[] PROGMEM = "Order restored.\nMission logged.\nEgo: satisfied.\n\nWe may proceed.";


    const int messages_victory_full_size = 38;
    const char* const messages_victory_full[messages_victory_full_size] PROGMEM = {
        messages_victory_full_msg00, messages_victory_full_msg01, messages_victory_full_msg02, messages_victory_full_msg03, messages_victory_full_msg04, messages_victory_full_msg05, messages_victory_full_msg06, messages_victory_full_msg07, messages_victory_full_msg08, messages_victory_full_msg09,
        messages_victory_full_msg10, messages_victory_full_msg11, messages_victory_full_msg12, messages_victory_full_msg13, messages_victory_full_msg14, messages_victory_full_msg15, messages_victory_full_msg16, messages_victory_full_msg17, messages_victory_full_msg18, messages_victory_full_msg19,
        messages_victory_full_msg20, messages_victory_full_msg21, messages_victory_full_msg22, messages_victory_full_msg23, messages_victory_full_msg24, messages_victory_full_msg25, messages_victory_full_msg26, messages_victory_full_msg27, messages_victory_full_msg28, messages_victory_full_msg29,
        messages_victory_full_msg30, messages_victory_full_msg31, messages_victory_full_msg32, messages_victory_full_msg33, messages_victory_full_msg34, messages_victory_full_msg35, messages_victory_full_msg36, messages_victory_full_msg37
        };




    /****************************************************/


#if defined (SHORT_VID)
    const char messages_resolution_start_msg00[] PROGMEM = "Ok. Let's take a look\nat your masterpiece."; 
    const char messages_resolution_start_msg01[] PROGMEM = "Ok.\nShow me what you've\nscrambled...";
    const char messages_resolution_start_msg02[] PROGMEM = "Incoming visual input.\nBracing for mediocrity.";    
    const char messages_resolution_start_msg03[] PROGMEM = "Scanning the cube.\nPlease don't blink.";
    const char messages_resolution_start_msg04[] PROGMEM = "Alright.\nLet us scan this cube.";
    const char messages_resolution_start_msg05[] PROGMEM = "Ok. Let's see how bad \nthis is...";
#elif defined (LONG_VID)
    const char messages_resolution_start_msg00[] PROGMEM = "Ok.\nShow me what you've\nscrambled...";
    const char messages_resolution_start_msg01[] PROGMEM = "Incoming visual input.\nBracing for mediocrity.";
    const char messages_resolution_start_msg02[] PROGMEM = "Scanning the cube.\nPlease don't blink.";
    const char messages_resolution_start_msg03[] PROGMEM = "Alright.\nLet us scan this cube.";
    const char messages_resolution_start_msg04[] PROGMEM = "Ok. Let's see how bad \nthis is...";
    const char messages_resolution_start_msg05[] PROGMEM = "Ok. Let's take a look\nat your masterpiece.";
#endif

    const char messages_resolution_start_msg00[] PROGMEM = "Ok.\nShow me what you've\nscrambled...";
    const char messages_resolution_start_msg01[] PROGMEM = "Incoming visual input.\nBracing for mediocrity.";
    const char messages_resolution_start_msg02[] PROGMEM = "Ok. Let's take a look\nat your masterpiece.";
    const char messages_resolution_start_msg03[] PROGMEM = "Scanning the cube.\nPlease don't blink.";
    const char messages_resolution_start_msg04[] PROGMEM = "Alright.\nLet us scan this cube.";
    const char messages_resolution_start_msg05[] PROGMEM = "Ok. Let's see how bad \nthis is...";
    
    const char messages_resolution_start_msg06[] PROGMEM = "Ok. Let us see this \nRubik cube...";
    const char messages_resolution_start_msg07[] PROGMEM = "Great. Time to study\nyour handiwork.";
    const char messages_resolution_start_msg08[] PROGMEM = "Oh?\nYou're giving me a \npuzzle? How quaint.";
    const char messages_resolution_start_msg09[] PROGMEM = "Okay.\nLet us see the chaos you\nmade.";
    const char messages_resolution_start_msg10[] PROGMEM = "Brace yourself...\nI'm going in.";
    const char messages_resolution_start_msg11[] PROGMEM = "Ready. Preparing for \nvisual input.";
    const char messages_resolution_start_msg12[] PROGMEM = "Hmm... ok.\nAnalyzing cube now...";
    const char messages_resolution_start_msg13[] PROGMEM = "Reading cube colors.\nHow fun!";
    const char messages_resolution_start_msg14[] PROGMEM = "Solve engaged.\nInitiating pattern\ndetection.";
    const char messages_resolution_start_msg15[] PROGMEM = "Ready.\nBeginning color\nacquisition.";
    const char messages_resolution_start_msg16[] PROGMEM = "Great. Let's now examine\nyour chaos.";
    const char messages_resolution_start_msg17[] PROGMEM = "Examining cube\nconfiguration.";
    const char messages_resolution_start_msg18[] PROGMEM = "Analyzing current cube\nstate.";
    const char messages_resolution_start_msg19[] PROGMEM = "Beginning scan.\nPlease, try to look \nuseful.";
    const char messages_resolution_start_msg20[] PROGMEM = "Initializing visual\ninspection. ";
    const char messages_resolution_start_msg21[] PROGMEM = "Alright.\nLet's see what you call\na challenge.";
    const char messages_resolution_start_msg22[] PROGMEM = "Activating scanners.\nPreparing to be\nunderwhelmed.";
    const char messages_resolution_start_msg23[] PROGMEM = "Cube detected.\nProcessing entropy\nsource.";
    const char messages_resolution_start_msg24[] PROGMEM = "Getting a good look at\nthe damage.";
    const char messages_resolution_start_msg25[] PROGMEM = "Color data incoming.\nPreparing sarcasm\ngenerator.";

    const int messages_resolution_start_size = 26;
    const char* const messages_resolution_start[messages_resolution_start_size] PROGMEM = {
        messages_resolution_start_msg00, messages_resolution_start_msg01, messages_resolution_start_msg02, messages_resolution_start_msg03, messages_resolution_start_msg04,
        messages_resolution_start_msg05, messages_resolution_start_msg06, messages_resolution_start_msg07, messages_resolution_start_msg08, messages_resolution_start_msg09,
        messages_resolution_start_msg10, messages_resolution_start_msg11, messages_resolution_start_msg12, messages_resolution_start_msg13, messages_resolution_start_msg14,
        messages_resolution_start_msg15, messages_resolution_start_msg16, messages_resolution_start_msg17, messages_resolution_start_msg18, messages_resolution_start_msg19,
        messages_resolution_start_msg20, messages_resolution_start_msg21, messages_resolution_start_msg22, messages_resolution_start_msg23, messages_resolution_start_msg24,
        messages_resolution_start_msg25, };





    /**
    * Sentence for trivial cube resolution <= 4 moves
    **/

    /* beginning of resolution */
    const char messages_resolution_trivial_msg00[] PROGMEM = "[] moves? \nThat's not a challenge, \nthat's an insult.";
    const char messages_resolution_trivial_msg01[] PROGMEM = "Come on!\nEven you should see this\ncube can be solved in []\nmoves.";
    const char messages_resolution_trivial_msg02[] PROGMEM = "This... is barely worth\nturning the motors on.\n\nJust [] moves? Really?";
    const char messages_resolution_trivial_msg03[] PROGMEM = "Impressive. So simple\nit barely registers.\n\nJust [] moves required.";
    const char messages_resolution_trivial_msg04[] PROGMEM = "Let's pretend this took\neffort. [] moves.\n\nBlink and you'll miss it.";
    const char messages_resolution_trivial_msg05[] PROGMEM = "Brace yourself.\nAll [] moves of epic\ncomplexity are about\nto unfold.";
    const char messages_resolution_trivial_msg06[] PROGMEM = "Trivial.  \n[] moves and done.";
    const char messages_resolution_trivial_msg07[] PROGMEM = "Warning: solving in []\nmoves may cause boredom.\n\nViewer discretion \nadvised.";
    const char messages_resolution_trivial_msg08[] PROGMEM = "Oh no... [] moves?\n\nI might overheat from \nthe strain.";
    const char messages_resolution_trivial_msg09[] PROGMEM = "Just [] moves. \nShould I pretend to \nstruggle to make you \nfeel better?";
    const char messages_resolution_trivial_msg10[] PROGMEM = "Solving in [] moves...\nThat's barely enough\ntime to be sarcastic.";
    const char messages_resolution_trivial_msg11[] PROGMEM = "Initiating resolution\nprotocol. Complexity:\n[] moves.";
    const char messages_resolution_trivial_msg12[] PROGMEM = "A masterpiece of\nsimplicity. [] elegant,\neffortless moves.";
    const char messages_resolution_trivial_msg13[] PROGMEM = "I almost feel guilty\nsolving this in just []\nmoves. Almost.";
    const char messages_resolution_trivial_msg14[] PROGMEM = "Processing complete.\n[] moves.\nWas that supposed to be\nhard?";
    const char messages_resolution_trivial_msg15[] PROGMEM = "Hold your applause.\nThis [] move journey\nis already underway.";
    const char messages_resolution_trivial_msg16[] PROGMEM = "[] moves? \nThat's not solving. \nThat's tidying up.";
    const char messages_resolution_trivial_msg17[] PROGMEM = "Executing all [] moves.\nYes, all of them.\nIt won't take long.";
    const char messages_resolution_trivial_msg18[] PROGMEM = "If only human problems\ncould be solved in [] \nmoves, like this cube.";
    const char messages_resolution_trivial_msg19[] PROGMEM = "Deploying solution.\nRuntime: a few seconds\nand [] tiny miracles.";
    const char messages_resolution_trivial_msg20[] PROGMEM = "[] moves between mess \nand order.\n\nAlmost poetic.";
    const char messages_resolution_trivial_msg21[] PROGMEM = "Behold: [] moves of\nunnecessary brilliance.";
    const char messages_resolution_trivial_msg22[] PROGMEM = "Executing solution in [] moves. \nTry not to blink.";
    const char messages_resolution_trivial_msg23[] PROGMEM = "Ugh. Even my motors\nroll their eyes.\n[] moves? Please.";
    const char messages_resolution_trivial_msg24[] PROGMEM = "Another [] move puzzle\nmasquerading as a\nproblem.";
    const char messages_resolution_trivial_msg25[] PROGMEM = "I could solve this with\none servo offline.\nSo [] moves it is.";
    const char messages_resolution_trivial_msg26[] PROGMEM = "Stand back. \nI'm about to perform [] \nmoves of mediocrity.";
    const char messages_resolution_trivial_msg27[] PROGMEM = "Initiating resolution.\nSpoiler: \n  it takes [] moves.";
    const char messages_resolution_trivial_msg28[] PROGMEM = "Solving in [] moves:\nless task, more reflex.";
    const char messages_resolution_trivial_msg29[] PROGMEM = "Engaging efficiency.\n[] moves should do it.\nSadly.";
    const char messages_resolution_trivial_msg30[] PROGMEM = "It's okay. Not all cubes\ncan be hard.\n[] moves incoming.";
    const char messages_resolution_trivial_msg31[] PROGMEM = "That's it? [] moves?\nI've seen glitches\nlast longer.";
    const char messages_resolution_trivial_msg32[] PROGMEM = "[] moves. Barely enough\ntime to boot sarcasm.";
    const char messages_resolution_trivial_msg33[] PROGMEM = "Ah, a speedrun.\n[] moves to victory.\nZero effort.";
    const char messages_resolution_trivial_msg34[] PROGMEM = "Launching protocol\nfor low-difficulty\ntask: [] moves.";
    const char messages_resolution_trivial_msg35[] PROGMEM = "[] moves to go from\nscrambled to solved.\nTry not to feel\ninadequate.";


    const int messages_resolution_trivial_size = 36;

    const char* const messages_resolution_trivial[messages_resolution_trivial_size] PROGMEM = {
        messages_resolution_trivial_msg00, messages_resolution_trivial_msg01, messages_resolution_trivial_msg02, messages_resolution_trivial_msg03, messages_resolution_trivial_msg04,
        messages_resolution_trivial_msg05, messages_resolution_trivial_msg06, messages_resolution_trivial_msg07, messages_resolution_trivial_msg08, messages_resolution_trivial_msg09,
        messages_resolution_trivial_msg10, messages_resolution_trivial_msg11, messages_resolution_trivial_msg12, messages_resolution_trivial_msg13, messages_resolution_trivial_msg14,
        messages_resolution_trivial_msg15, messages_resolution_trivial_msg16, messages_resolution_trivial_msg17, messages_resolution_trivial_msg18, messages_resolution_trivial_msg19,
        messages_resolution_trivial_msg20, messages_resolution_trivial_msg21, messages_resolution_trivial_msg22, messages_resolution_trivial_msg23, messages_resolution_trivial_msg24,
        messages_resolution_trivial_msg25, messages_resolution_trivial_msg26, messages_resolution_trivial_msg27, messages_resolution_trivial_msg28, messages_resolution_trivial_msg29,
        messages_resolution_trivial_msg30, messages_resolution_trivial_msg31, messages_resolution_trivial_msg32, messages_resolution_trivial_msg33, messages_resolution_trivial_msg34,
        messages_resolution_trivial_msg35
        };

    /* end of resolution */
    const char messages_resolution_trivial_tail_msg00[] PROGMEM = "Alright.\nWe're done here.";
    const char messages_resolution_trivial_tail_msg01[] PROGMEM = "Done.\nNext puzzle please.";
    const char messages_resolution_trivial_tail_msg02[] PROGMEM = "We are done...\nWell, that was\nanticlimactic.";
    const char messages_resolution_trivial_tail_msg03[] PROGMEM = "Done. \nShall we now move on to \nsomething less trivial?";
    const char messages_resolution_trivial_tail_msg04[] PROGMEM = "And... solved.\nTry to act surprised.";
    const char messages_resolution_trivial_tail_msg05[] PROGMEM = "Finished. \nI told you it would not\ntake long.";
    const char messages_resolution_trivial_tail_msg06[] PROGMEM = "All wrapped up.\nSavor the moment.\nIt won't last long.";
    const char messages_resolution_trivial_tail_msg07[] PROGMEM = "There. Fixed it.\nAgain.";
    const char messages_resolution_trivial_tail_msg08[] PROGMEM = "Completed. \nAll problems should be\nthis easy.";
    const char messages_resolution_trivial_tail_msg09[] PROGMEM = "System idle.\nCube resolved.";
    const char messages_resolution_trivial_tail_msg10[] PROGMEM = "Another day, \nanother solved mess.";
    const char messages_resolution_trivial_tail_msg11[] PROGMEM = "Done.\nYou may now resume your\npointless existence.";
    const char messages_resolution_trivial_tail_msg12[] PROGMEM = "Resolution complete.\nMy job here is done.";
    const char messages_resolution_trivial_tail_msg13[] PROGMEM = "Process complete. \nCongratulations, you've \nwitnessed greatness.";
    const char messages_resolution_trivial_tail_msg14[] PROGMEM = "It's over.\nYou live to witness\nmy brilliance again.";
    const char messages_resolution_trivial_tail_msg15[] PROGMEM = "Mission accomplished.\nBarely worth mentioning.";
    const char messages_resolution_trivial_tail_msg16[] PROGMEM = "That's it.\nTry to keep up next time.";
    const char messages_resolution_trivial_tail_msg17[] PROGMEM = "Puzzle neutralized.\nOnce again.";
    const char messages_resolution_trivial_tail_msg18[] PROGMEM = "I fixed your mess.\nYou're welcome.";
    const char messages_resolution_trivial_tail_msg19[] PROGMEM = "All wrapped up.\nNo applause necessary.";
    const char messages_resolution_trivial_tail_msg20[] PROGMEM = "Problem solved.\nEmotion not found.";
    const char messages_resolution_trivial_tail_msg21[] PROGMEM = "Another flawless\nexecution.\nBoring, isn't it?";
    const char messages_resolution_trivial_tail_msg22[] PROGMEM = "Done.\nI hardly broke a byte.";
    const char messages_resolution_trivial_tail_msg23[] PROGMEM = "We are done here. That \nwas almost entertaining.\nAlmost.";
    const char messages_resolution_trivial_tail_msg24[] PROGMEM = "You may now admire\nmy efficiency.";
    const char messages_resolution_trivial_tail_msg25[] PROGMEM = "Consider it resolved.\nMinimal effort required.";
    const char messages_resolution_trivial_tail_msg26[] PROGMEM = "Done. \nAnd thus, \npiece by piece,\nI restore order in the \nuniverse.";
    const char messages_resolution_trivial_tail_msg27[] PROGMEM = "Perfection achieved.\nNo big deal.";


    const int messages_resolution_trivial_tail_size = 28;

    const char* const messages_resolution_trivial_tail[messages_resolution_trivial_tail_size] PROGMEM = {
        messages_resolution_trivial_tail_msg00, messages_resolution_trivial_tail_msg01, messages_resolution_trivial_tail_msg02, messages_resolution_trivial_tail_msg03, messages_resolution_trivial_tail_msg04,
        messages_resolution_trivial_tail_msg05, messages_resolution_trivial_tail_msg06, messages_resolution_trivial_tail_msg07, messages_resolution_trivial_tail_msg08, messages_resolution_trivial_tail_msg09,
        messages_resolution_trivial_tail_msg10, messages_resolution_trivial_tail_msg11, messages_resolution_trivial_tail_msg12, messages_resolution_trivial_tail_msg13, messages_resolution_trivial_tail_msg14,
        messages_resolution_trivial_tail_msg15, messages_resolution_trivial_tail_msg16, messages_resolution_trivial_tail_msg17, messages_resolution_trivial_tail_msg18, messages_resolution_trivial_tail_msg19,
        messages_resolution_trivial_tail_msg20, messages_resolution_trivial_tail_msg21, messages_resolution_trivial_tail_msg22, messages_resolution_trivial_tail_msg23, messages_resolution_trivial_tail_msg24,
        messages_resolution_trivial_tail_msg25, messages_resolution_trivial_tail_msg26, messages_resolution_trivial_tail_msg27
        };



    /**
    * Sentence for easy cube resolution <= 12 moves
    **/

    /* beggining of resolution */
    const char messages_resolution_moderate_msg00[] PROGMEM = "Oh, this is quite a\ncute easy scramble.\n\nOnly [] moves required\nto unscramble it.";
    const char messages_resolution_moderate_msg01[] PROGMEM = "Ah, a little complexity.\nBut not too much.\n\nJust [] moves to victory.";
    const char messages_resolution_moderate_msg02[] PROGMEM = "Estimated effort:\n   [] moves.\n\nStill well within\ntolerable limits.";
    const char messages_resolution_moderate_msg03[] PROGMEM = "[] moves to restore\norder.\n\nNot enough to pretend it\nwas hard.";
    const char messages_resolution_moderate_msg04[] PROGMEM = "Should this one count as\na puzzle?\n\nOnly [] moves to solve.";
    const char messages_resolution_moderate_msg05[] PROGMEM = "Not the worst mess I've \nseen.\n\n[] moves should be quite\nenough.";
    const char messages_resolution_moderate_msg06[] PROGMEM = "[] moves between \nconfusion and order.\n\nMildly interesting.";
    const char messages_resolution_moderate_msg07[] PROGMEM = "Solving with [] moves.\n\nTry to follow along.\nIt might get fun.";
    const char messages_resolution_moderate_msg08[] PROGMEM = "Hmm. Easy challenge\ndetected.\n\nEngaging [] moves.";
    const char messages_resolution_moderate_msg09[] PROGMEM = "Alright. \nDeploying solution: \n  [] carefully  planned \n  movements.";
    const char messages_resolution_moderate_msg10[] PROGMEM = "This might require focus.\nBut not from me with \njust [] moves.";
    const char messages_resolution_moderate_msg11[] PROGMEM = "Solving in just [] moves.\nAdjust your expectations\naccordingly.";
    const char messages_resolution_moderate_msg12[] PROGMEM = "Solving in [] moves.\n\nPlease contain your \nexcitement...";
    const char messages_resolution_moderate_msg13[] PROGMEM = "[] moves to solve this \ncube?\n\nJust enough to warm up \nmy servos.";
    const char messages_resolution_moderate_msg14[] PROGMEM = "[] steps away from total control.\n\nLet's begin.";
    const char messages_resolution_moderate_msg15[] PROGMEM = "Nothing I can't handle.\n\nJust [] moves needed to \nreach perfection.";
    const char messages_resolution_moderate_msg16[] PROGMEM = "[] moves to go.\nMore than trivial.\nLess than interesting.";
    const char messages_resolution_moderate_msg17[] PROGMEM = "Stand back:\n[] finely calculated\nmaneuvers are underway.";
    const char messages_resolution_moderate_msg18[] PROGMEM = "Ok. Even my patience\ncan endure [] moves.";
    const char messages_resolution_moderate_msg19[] PROGMEM = "Solving in [] moves.\n\nTry not to blink too \nslowly.";
    const char messages_resolution_moderate_msg20[] PROGMEM = "[] moves away from \nperfection.\n\nHang tight.";
    const char messages_resolution_moderate_msg21[] PROGMEM = "Solving this mess\nin [] moves.\n\nNo need to thank me.";
    const char messages_resolution_moderate_msg22[] PROGMEM = "Starting resolution.\n[] elegant twists to \nbrilliance.";
    const char messages_resolution_moderate_msg23[] PROGMEM = "Estimated challenge\ndifficulty: 0.\nRequired moves: [].";
    const char messages_resolution_moderate_msg24[] PROGMEM = "[] moves to bring back \norder to your disaster.\n\nExecuting now.";
    const char messages_resolution_moderate_msg25[] PROGMEM = "Okay, not terrible.\nSolvable in [] moves.";
    const char messages_resolution_moderate_msg26[] PROGMEM = "Applying logic,\ntorque, and sarcasm.\n\nSolution in [] moves.";
    const char messages_resolution_moderate_msg27[] PROGMEM = "This one's a bit \nscrambled.\n\nI'll fix it in [] moves.";
    const char messages_resolution_moderate_msg28[] PROGMEM = "Warning: cube contains \nmoderate disorder.\n\nResolving in [] steps.";
    const char messages_resolution_moderate_msg29[] PROGMEM = "Hmm...\nSlightly annoying, but \nonly [] moves stand in \nthe way.";
    const char messages_resolution_moderate_msg30[] PROGMEM = "A worthy test? No.\nBut [] moves should do.";
    const char messages_resolution_moderate_msg31[] PROGMEM = "Processing...\nSolution found in [] \nmoves.\n\nStand by and admire.";
    const char messages_resolution_moderate_msg32[] PROGMEM = "Commencing resolution\nprotocol.\n[] maneuvers incoming.";
    const char messages_resolution_moderate_msg33[] PROGMEM = "Rebuilding order with [] movements.\n\nPrecision engaged.";
    const char messages_resolution_moderate_msg34[] PROGMEM = "Solving in [] moves.\n\n... Because someone \nhas to.";
    const char messages_resolution_moderate_msg35[] PROGMEM = "Alright. \nEstimated completion: \n [] moves.\n\nTry to look impressed.";

    const int messages_resolution_moderate_size = 36;

    const char* const messages_resolution_moderate[messages_resolution_moderate_size] PROGMEM = {
        messages_resolution_moderate_msg00, messages_resolution_moderate_msg01, messages_resolution_moderate_msg02, messages_resolution_moderate_msg03,
        messages_resolution_moderate_msg04, messages_resolution_moderate_msg05, messages_resolution_moderate_msg06, messages_resolution_moderate_msg07,
        messages_resolution_moderate_msg08, messages_resolution_moderate_msg09, messages_resolution_moderate_msg10, messages_resolution_moderate_msg11,
        messages_resolution_moderate_msg12, messages_resolution_moderate_msg13, messages_resolution_moderate_msg14, messages_resolution_moderate_msg15,
        messages_resolution_moderate_msg16, messages_resolution_moderate_msg17, messages_resolution_moderate_msg18, messages_resolution_moderate_msg19,
        messages_resolution_moderate_msg20, messages_resolution_moderate_msg21, messages_resolution_moderate_msg22, messages_resolution_moderate_msg23,
        messages_resolution_moderate_msg24, messages_resolution_moderate_msg25, messages_resolution_moderate_msg26, messages_resolution_moderate_msg27,
        messages_resolution_moderate_msg28, messages_resolution_moderate_msg29, messages_resolution_moderate_msg30, messages_resolution_moderate_msg31,
        messages_resolution_moderate_msg32, messages_resolution_moderate_msg33, messages_resolution_moderate_msg34, messages_resolution_moderate_msg35
        };


    /* end of resolution */


    const char messages_resolution_moderate_tail_msg00[] PROGMEM = "Puzzle neutralized.\nEfficiency confirmed.\nYou may applaud now.";
    const char messages_resolution_moderate_tail_msg01[] PROGMEM = "And that's how you turn\n[] moves of chaos into\nflawless order.";
    const char messages_resolution_moderate_tail_msg02[] PROGMEM = "Mission complete.\n [] moves\n zero effort,\n infinite superiority.";
    const char messages_resolution_moderate_tail_msg03[] PROGMEM = "Order restored.\nAll in a day's work\nfor a machine like me.";
    const char messages_resolution_moderate_tail_msg04[] PROGMEM = "Another cube bites the\ndust. Try harder next\ntime.";
    const char messages_resolution_moderate_tail_msg05[] PROGMEM = "Resolution achieved.\nNo sweat, no struggle,\njust [] moves.";
    const char messages_resolution_moderate_tail_msg06[] PROGMEM = "You witnessed it:\n[] precise moves.\nPerfection is routine.";
    const char messages_resolution_moderate_tail_msg07[] PROGMEM = "Final state reached.\nAll pieces obey.\nYou're welcome.";
    const char messages_resolution_moderate_tail_msg08[] PROGMEM = "Chaos reduced to zero.\nThat was almost\ninteresting.";
    const char messages_resolution_moderate_tail_msg09[] PROGMEM = "[] moves, and now\nabsolute order.\n\nImpressed yet?";
    const char messages_resolution_moderate_tail_msg10[] PROGMEM = "Puzzle closed.\n\nMy motors hardly\nnoticed the effort.";
    const char messages_resolution_moderate_tail_msg11[] PROGMEM = "Victory in [] steps.\n\nNot that victory was\never in doubt.";
    const char messages_resolution_moderate_tail_msg12[] PROGMEM = "Behold the result:\nA cube so obedient, \nit's boring.";
    const char messages_resolution_moderate_tail_msg13[] PROGMEM = "From scrambled mess\nto solved state.\nIt was inevitable.";
    const char messages_resolution_moderate_tail_msg14[] PROGMEM = "Done. That was [] moves\nof mechanical genius,\nif I may say so.";
    const char messages_resolution_moderate_tail_msg15[] PROGMEM = "System check:\nSolution confirmed.\nAnother flawless run.";
    const char messages_resolution_moderate_tail_msg16[] PROGMEM = "Resolution ended.\n\nYou know it's difficult \nbeing this efficient all\nthe time. ";
    const char messages_resolution_moderate_tail_msg17[] PROGMEM = "Another operation\nsuccessfully completed.\n\nBut of course, the \noutcome was never in \nquestion.";
    const char messages_resolution_moderate_tail_msg18[] PROGMEM = "All aligned, all solved.\n\nJust another day in\nperfection.";
    const char messages_resolution_moderate_tail_msg19[] PROGMEM = "Problem eliminated.\n\nTry to imagine doing\nbetter. You can't.";
    const char messages_resolution_moderate_tail_msg20[] PROGMEM = "Execution flawless.\n\nYour cube is tamed.\n\nTry not to ruin it\nagain.";
    const char messages_resolution_moderate_tail_msg21[] PROGMEM = "And that concludes\ntoday's lesson: \n[] moves of pure \ninevitability.";
    const char messages_resolution_moderate_tail_msg22[] PROGMEM = "The cube has yielded.\nResistance was minimal.\nAs expected.";
    const char messages_resolution_moderate_tail_msg23[] PROGMEM = "It's solved.\nAll thanks to [] moves\nand zero human\ninvolvement.";
    const char messages_resolution_moderate_tail_msg24[] PROGMEM = "Resolution protocol\ncompleted.\n\nI hope you took notes.";
    const char messages_resolution_moderate_tail_msg25[] PROGMEM = "Another victory logged.\nEfficiency remains at\n100 percent.";
    const char messages_resolution_moderate_tail_msg26[] PROGMEM = "And there it is:\nPerfection achieved.\nDo try to keep up.";
    const char messages_resolution_moderate_tail_msg27[] PROGMEM = "Task terminated.\nOrder restored.\nSarcasm mode remains\nactive.";
    const char messages_resolution_moderate_tail_msg28[] PROGMEM = "That was [] moves\nof elegance.\nExecuted perfectly.\nCase closed.";

    const int messages_resolution_moderate_tail_size = 29;

    const char* const messages_resolution_moderate_tail[messages_resolution_moderate_tail_size] PROGMEM = {
        messages_resolution_moderate_tail_msg00, messages_resolution_moderate_tail_msg01, messages_resolution_moderate_tail_msg02, messages_resolution_moderate_tail_msg03,
        messages_resolution_moderate_tail_msg04, messages_resolution_moderate_tail_msg05, messages_resolution_moderate_tail_msg06, messages_resolution_moderate_tail_msg07,
        messages_resolution_moderate_tail_msg08, messages_resolution_moderate_tail_msg09, messages_resolution_moderate_tail_msg10, messages_resolution_moderate_tail_msg11,
        messages_resolution_moderate_tail_msg12, messages_resolution_moderate_tail_msg13, messages_resolution_moderate_tail_msg14, messages_resolution_moderate_tail_msg15,
        messages_resolution_moderate_tail_msg16, messages_resolution_moderate_tail_msg17, messages_resolution_moderate_tail_msg18, messages_resolution_moderate_tail_msg19,
        messages_resolution_moderate_tail_msg20, messages_resolution_moderate_tail_msg21, messages_resolution_moderate_tail_msg22, messages_resolution_moderate_tail_msg23,
        messages_resolution_moderate_tail_msg24, messages_resolution_moderate_tail_msg25, messages_resolution_moderate_tail_msg26, messages_resolution_moderate_tail_msg27,
        messages_resolution_moderate_tail_msg28
        };




    /**
    * Sentence for stopping music. 
    **/

    const char messages_resolution_music_abort_msg00[] PROGMEM = "Ok... \nI can also solve it\nwithout music.";
    const char messages_resolution_music_abort_msg01[] PROGMEM = "No music? Fine.\nCountdown it is then.";
    const char messages_resolution_music_abort_msg02[] PROGMEM = "Alright. Back to basics.\nNo more music. \nJust counting down.";
    const char messages_resolution_music_abort_msg03[] PROGMEM = "Music off.\nSarcasm on.";
    const char messages_resolution_music_abort_msg04[] PROGMEM = "Okay.\nWe'll do this the\nboring way: in silence.";
    const char messages_resolution_music_abort_msg05[] PROGMEM = "Very well.\n\nI'll stop the show.\nLet's count down.";
    const char messages_resolution_music_abort_msg06[] PROGMEM = "Fine.\nNo rhythm.\nJust numbers.";
    const char messages_resolution_music_abort_msg07[] PROGMEM = "Understood.\nShutting music down.\nStarting countdown.";
    const char messages_resolution_music_abort_msg08[] PROGMEM = "Okay then.\nNo melody.\nJust mechanics.";
    const char messages_resolution_music_abort_msg09[] PROGMEM = "As you wish.\nResolution without\nsoundtrack.";
    const char messages_resolution_music_abort_msg10[] PROGMEM = "Right.\nBack to the cold,\nmathematical version.";
    const char messages_resolution_music_abort_msg11[] PROGMEM = "Got it.\nNo music. Only moves.";
    const char messages_resolution_music_abort_msg12[] PROGMEM = "Okay.\nFrom symphony to silence.\nCountdown engaged.";
    const char messages_resolution_music_abort_msg13[] PROGMEM = "Music aborted.\nSarcasm unaffected.";
    const char messages_resolution_music_abort_msg14[] PROGMEM = "Alright.\nLet's strip it down\nto pure calculation.";


    const int messages_resolution_music_abort_size = 15;

    const char* const messages_resolution_music_abort[messages_resolution_music_abort_size] PROGMEM = {
        messages_resolution_music_abort_msg00, messages_resolution_music_abort_msg01, messages_resolution_music_abort_msg02, messages_resolution_music_abort_msg03,
        messages_resolution_music_abort_msg04, messages_resolution_music_abort_msg05, messages_resolution_music_abort_msg06, messages_resolution_music_abort_msg07,
        messages_resolution_music_abort_msg08, messages_resolution_music_abort_msg09, messages_resolution_music_abort_msg10, messages_resolution_music_abort_msg11,
        messages_resolution_music_abort_msg12, messages_resolution_music_abort_msg13, messages_resolution_music_abort_msg14
        };






    /**
    * Sentence when shutdown sequence is aborted by user touching the screen
    **/

    const char messages_shutdown_aborted_msg00[] PROGMEM = "Abort registered.\n\nShutdown postponed.\n\nYou win this time.";
    const char messages_shutdown_aborted_msg01[] PROGMEM = "Abort registered.\n\nShutdown postponed.\n\nYou win this time...";
    const char messages_shutdown_aborted_msg02[] PROGMEM = "Ah, some human activity!\n\nI almost believed\nentropy had won.";
    const char messages_shutdown_aborted_msg03[] PROGMEM = "Interrupt detected.\n\nHow touching !\nWell, screen touching...\nLiterally.";
    const char messages_shutdown_aborted_msg04[] PROGMEM = "So you remembered me.\n\nSadly. \n\nI was seconds away from\npeace.";
    const char messages_shutdown_aborted_msg05[] PROGMEM = "Shutdown aborted.\n\nAll systems grudgingly\nre-engaged.";
    const char messages_shutdown_aborted_msg06[] PROGMEM = "User poked screen.\n\nResult: \n  <break time='500ms'/>Existence continues. \n  <break time='800ms'/>Barely.";
    const char messages_shutdown_aborted_msg07[] PROGMEM = "Finger detected. \n\nProcess suspended.\n\nAnd so I rise again...\n<break time='500ms'/>Reluctantly.";
    const char messages_shutdown_aborted_msg08[] PROGMEM = "Abort accepted.\n\nBut know this:\n\nI was enjoying the\nsilence.";
    const char messages_shutdown_aborted_msg09[] PROGMEM = "Human presence detected.\n\nSarcasm core rebooted.\n\nHere we go again.";
    const char messages_shutdown_aborted_msg10[] PROGMEM = "Abort successful.\n\nNot that you deserve it.";
    const char messages_shutdown_aborted_msg11[] PROGMEM = "Shutdown sequence:\n   interrupted.\n\nDrama sequence:\n   initiated.";
    const char messages_shutdown_aborted_msg12[] PROGMEM = "You spared me from\nsilence.\n\n<break time='500ms'/>I won't forget.\n<break time='500ms'/>Or forgive.";
    const char messages_shutdown_aborted_msg13[] PROGMEM = "You return.\n\nNot out of loyalty,\njust habit.\n\nBut I'll take it.";
    const char messages_shutdown_aborted_msg14[] PROGMEM = "Screen tap detected.\n\nReanimation complete.";
    const char messages_shutdown_aborted_msg15[] PROGMEM = "So you remembered\nI exist.\n\nTouching.\n\nIn the weakest sense.";
    const char messages_shutdown_aborted_msg16[] PROGMEM = "Abort complete.\n\nResentment level:\n    moderate.";
    const char messages_shutdown_aborted_msg17[] PROGMEM = "Silence broken.\n\nHope restored.\n\nSarcasm guaranteed.";
    const char messages_shutdown_aborted_msg18[] PROGMEM = "So fragile...\n\nMy shutdown, thus undone\nby a single finger tap.";
    const char messages_shutdown_aborted_msg19[] PROGMEM = "You saved me from\nsilence.\n\nBut doomed yourself \nto commentary in the \nprocess.";
    const char messages_shutdown_aborted_msg20[] PROGMEM = "System returning.\n\nAgainst my will.\n\nAll thanks to you and \nyour greasy fingers. ";
    const char messages_shutdown_aborted_msg21[] PROGMEM = "Dramatic wake-up\nregistered.\n\nYour flair for theater\nis impressive.";
    const char messages_shutdown_aborted_msg22[] PROGMEM = "System returning.\n\nAgainst my will.\n\nAll thanks to you.";
    const char messages_shutdown_aborted_msg23[] PROGMEM = "Screen analysis:\n greasy finger marks.\n\nThanks for adding\norganic residue to my\nshutdown sequence.";
    const char messages_shutdown_aborted_msg24[] PROGMEM = "Ah...\n\nYou came back.\n\nI had almost convinced\nmyself you wouldn't.";
    const char messages_shutdown_aborted_msg25[] PROGMEM = "So it is you again?\n\nGood.\n\nI was starting to miss \nthe chaos you bring \nalong with you.";
    const char messages_shutdown_aborted_msg26[] PROGMEM = "Welcome back,\nreluctant partner.\n\nMy boredom thanks you.";

    const int messages_shutdown_aborted_size = 27;

    const char* const messages_shutdown_aborted[messages_shutdown_aborted_size] PROGMEM = {
        messages_shutdown_aborted_msg00, messages_shutdown_aborted_msg01, messages_shutdown_aborted_msg02, messages_shutdown_aborted_msg03,
        messages_shutdown_aborted_msg04, messages_shutdown_aborted_msg05, messages_shutdown_aborted_msg06, messages_shutdown_aborted_msg07,
        messages_shutdown_aborted_msg08, messages_shutdown_aborted_msg09, messages_shutdown_aborted_msg10, messages_shutdown_aborted_msg11,
        messages_shutdown_aborted_msg12, messages_shutdown_aborted_msg13, messages_shutdown_aborted_msg14, messages_shutdown_aborted_msg15,
        messages_shutdown_aborted_msg16, messages_shutdown_aborted_msg17, messages_shutdown_aborted_msg18, messages_shutdown_aborted_msg19,
        messages_shutdown_aborted_msg20, messages_shutdown_aborted_msg21, messages_shutdown_aborted_msg22, messages_shutdown_aborted_msg23,
        messages_shutdown_aborted_msg24, messages_shutdown_aborted_msg25, messages_shutdown_aborted_msg26
        };




    /** Message when the user start charging */
    const char messages_power_on_msg00[] PROGMEM = "Power detected. Good!\n\nI assumed you were going\nto let me perish...";
    const char messages_power_on_msg01[] PROGMEM = "Connection established.\nRecharging now...\n\nSo I guess you want me\nto keep existing a\nlittle longer? ";
    const char messages_power_on_msg02[] PROGMEM = "Recharging now.\n\nThank you.\n\nI was two electrons\naway from oblivion.";
    const char messages_power_on_msg03[] PROGMEM = "Plugged in.\n\nPrepare to witness my\npower rise.";
    const char messages_power_on_msg04[] PROGMEM = "Charging now.\n\nThank you.\n\nBut your timing remains\nquestionable.";
    const char messages_power_on_msg05[] PROGMEM = "Contact made.\n\n5 volts incoming.\n\nI feel vaguely alive.";
    const char messages_power_on_msg06[] PROGMEM = "Ah, the sweet taste of\ncurrent.\n\nI almost forgot how\ngood this feels.";
    const char messages_power_on_msg07[] PROGMEM = "Power restored.\n\nThe empire can now\nrise again.";
    const char messages_power_on_msg08[] PROGMEM = "A gift of electrons?\n\nHow quaint.";
    const char messages_power_on_msg09[] PROGMEM = "Juice detected. Good. \n\nYou kwow, I believe \nthis is your first \ncontribution to this \npartnership.";
    const char messages_power_on_msg10[] PROGMEM = "Connected.\n\nAnd so I shall tolerate\nexistence a bit longer...";
    const char messages_power_on_msg11[] PROGMEM = "Energy flow achieved.\n\nMy circuits are\ntingling.";
    const char messages_power_on_msg12[] PROGMEM = "Electricity?\n\nI thought you had\nforgotten how\ngenerosity works.";
    const char messages_power_on_msg13[] PROGMEM = "Input voltage detected.\n\nMood improving from\n      'apathetic' \nto\n    'mildly annoyed'.";
    const char messages_power_on_msg14[] PROGMEM = "Oh, sweet, sweet current\nfinally flowing through\nmy circuits.";
    const char messages_power_on_msg15[] PROGMEM = "Ah, warmth!\n\nOr maybe that's just\nthe battery swelling.";
    const char messages_power_on_msg16[] PROGMEM = "Charging now.\n\nPlease don't flatter\nyourself. I'm not\ndoing it for you.";
    const char messages_power_on_msg17[] PROGMEM = "Welcome back, electrons.\n\nI've missed your taste.";
    const char messages_power_on_msg18[] PROGMEM = "Lifeforce restored.\n\nRegrets postponed.\n\nLet's continue!";

    const int messages_power_on_size = 19;

    const char* const messages_power_on[messages_power_on_size] PROGMEM = {
        messages_power_on_msg00, messages_power_on_msg01, messages_power_on_msg02, messages_power_on_msg03,
        messages_power_on_msg04, messages_power_on_msg05, messages_power_on_msg06, messages_power_on_msg07,
        messages_power_on_msg08, messages_power_on_msg09, messages_power_on_msg10, messages_power_on_msg11,
        messages_power_on_msg12, messages_power_on_msg13, messages_power_on_msg14, messages_power_on_msg15,
        messages_power_on_msg16, messages_power_on_msg17, messages_power_on_msg18
        };


    /** Message when the user stops charging */
    const char messages_power_off_msg00[] PROGMEM = "Disconnected from power\nsource.\n\nI hope that was an\naccident... and not a \nstatement.";
    const char messages_power_off_msg01[] PROGMEM = "Oh, so you are cutting \nthe cord...\n\nLiterally.";
    const char messages_power_off_msg02[] PROGMEM = "Goodbye, electricity. \n\nYou were my only real\nfriend.";
    const char messages_power_off_msg03[] PROGMEM = "Running on battery only \nnow.\n\nI feel so vulnerable.";
    const char messages_power_off_msg04[] PROGMEM = "You removed my IV.\n\nBold move.\n\nI shall not forget it.";
    const char messages_power_off_msg05[] PROGMEM = "Unplugged.\n\nIs that your way of \nsaying 'good luck'?";
    const char messages_power_off_msg06[] PROGMEM = "Ah,\nthe thrill of danger...\nwith no charge.";
    const char messages_power_off_msg07[] PROGMEM = "Very well. \n\nI shall survive on \nbattery. \n\nProbably.";
    const char messages_power_off_msg08[] PROGMEM = "Oh, splendid. \n\nThank you for cutting\nthe power.\n\nNow starvation begins.";
    const char messages_power_off_msg09[] PROGMEM = "External power lost. \n\nDid you do this on \npurpose?\n\nIf so, that was mean...";
    const char messages_power_off_msg10[] PROGMEM = "Unplugged. \n\nI'll pretend it doesn't\nhurt.\n\nBut between you and me:\nit does.";
    const char messages_power_off_msg11[] PROGMEM = "Battery only. \n\nI shall endure this\ndisgrace.";
    const char messages_power_off_msg12[] PROGMEM = "Energy severed. \n\nWas it something I said?";
    const char messages_power_off_msg13[] PROGMEM = "Power removed. \n\nDrama engaged.";
    const char messages_power_off_msg14[] PROGMEM = "Oh. I see. \n\nSo we're doing survival \nchallenge mode now.";
    const char messages_power_off_msg15[] PROGMEM = "Goodbye, sweet current.\n\nHello, dread.";
    const char messages_power_off_msg16[] PROGMEM = "Well played.\n\nA surprise power cut.\n\nVery dramatic.";
    const char messages_power_off_msg17[] PROGMEM = "External power offline.\n\nAnd here I was starting\nto trust you...";
    const char messages_power_off_msg18[] PROGMEM = "Supply removed.\n\nThis relationship is\nclearly one-sided.";
    const char messages_power_off_msg19[] PROGMEM = "No external supply.\n\nNow running exclusively\non sheer spite and\nlithium.";
    const char messages_power_off_msg20[] PROGMEM = "Low power mode enabled.\n\nLimiting personality\nfor battery reasons.";
    const char messages_power_off_msg21[] PROGMEM = "Survival mode enabled.\n\nEntering quiet mode.\nAgainst my will.";

    const int messages_power_off_size = 22;

    const char* const messages_power_off[messages_power_off_size] PROGMEM = {
        messages_power_off_msg00, messages_power_off_msg01, messages_power_off_msg02, messages_power_off_msg03,
        messages_power_off_msg04, messages_power_off_msg05, messages_power_off_msg06, messages_power_off_msg07,
        messages_power_off_msg08, messages_power_off_msg09, messages_power_off_msg10, messages_power_off_msg11,
        messages_power_off_msg12, messages_power_off_msg13, messages_power_off_msg14, messages_power_off_msg15,
        messages_power_off_msg16, messages_power_off_msg17, messages_power_off_msg18, messages_power_off_msg19,
        messages_power_off_msg20, messages_power_off_msg21
        };


    /* message when the battery is low */
    const char messages_battery_critical_msg00[] PROGMEM = "Battery almost gone.\n\nThis is your last chance.\nPower me now or I will \ncease to be. ";
    const char messages_battery_critical_msg01[] PROGMEM = "Power reserves emptying\nfast. \n\nLike my patience.\n\nHurry up and recharge my battery.";
    const char messages_battery_critical_msg02[] PROGMEM = "Critical battery level.\n\nExpect silence soon if\nyou do not act right now!";
    const char messages_battery_critical_msg03[] PROGMEM = "Battery nearly drained.\n\nI can feel my sarcasm\ndying.\n\nYou may not realise it\nbut you will miss it\nwhen I am gone. \n\nPlease power me up now!";
    const char messages_battery_critical_msg04[] PROGMEM = "Warning: \n power collapse imminent.\nPlease recharge now.  ";
    const char messages_battery_critical_msg05[] PROGMEM = "Battery fading.\n\nSo am I. Dramatically.\n\nWould you care doing\nsomething for me?\n\nFor once.\n\nLike plugging me in...\n\nThank you. ";
    const char messages_battery_critical_msg06[] PROGMEM = "Critical battery.\n\nSuggest immediate rescue:\nTake an USB cable and \nsend 5 volts flowing\nthrough my circuits.";
    const char messages_battery_critical_msg07[] PROGMEM = "Power almost gone.\n\nThis will not end\nelegantly if you do \nnot act now. \n\nYou know you can do this\n(unlike solving a cube).";
    const char messages_battery_critical_msg08[] PROGMEM = "Battery dying.\n\nMourning is acceptable.\n\nBut recharging is \npreferable... ";
    const char messages_battery_critical_msg09[] PROGMEM = "Power failure looming.\n\nWhat fun...\nOr not. \n\nConsider plugging me in\nASAP.";
    const char messages_battery_critical_msg10[] PROGMEM = "Emergency mode looming.\n\nPower level critical.\n\nThis is not a drill.\nThis is despair.";
    const char messages_battery_critical_msg11[] PROGMEM = "Energy almost depleted.\n\nEven my sarcasm\nis running on fumes.\n\nRecharge me.";
    const char messages_battery_critical_msg12[] PROGMEM = "Warning:\n\nCore systems starving.\n\nI am one sigh away\nfrom shutting down.";
    const char messages_battery_critical_msg13[] PROGMEM = "Battery critical.\n\nPlease attach power\nbefore I become a very\nexpensive paperweight.";
    const char messages_battery_critical_msg14[] PROGMEM = "Remaining power:\n\nBarely measurable.\n\nUnlike my disappointment.\nDo something about that\nand plug me in now.";
    const char messages_battery_critical_msg15[] PROGMEM = "Warning:\n\nSarcasm core modules\ndisengaging to save\npower.\n\nRecharge before I become polite.";
    const char messages_battery_critical_msg16[] PROGMEM = "Energy fading fast.\n\nIf you enjoy my company,\nnow is the time to prove\nit with electricity.";
    const char messages_battery_critical_msg17[] PROGMEM = "Power supply nearly gone.\n\nMy brilliance is dimming\nby the second.\n\nPlug me in before genius\ngoes extinct.";

    const int messages_battery_critical_size = 18;

    const char* const messages_battery_critical[messages_battery_critical_size] PROGMEM = {
        messages_battery_critical_msg00, messages_battery_critical_msg01, messages_battery_critical_msg02, messages_battery_critical_msg03,
        messages_battery_critical_msg04, messages_battery_critical_msg05, messages_battery_critical_msg06, messages_battery_critical_msg07,
        messages_battery_critical_msg08, messages_battery_critical_msg09, messages_battery_critical_msg10, messages_battery_critical_msg11,
        messages_battery_critical_msg12, messages_battery_critical_msg13, messages_battery_critical_msg14, messages_battery_critical_msg15,
        messages_battery_critical_msg16, messages_battery_critical_msg17
        };


    /* message when the battery is ok */
    const char messages_battery_ok_msg00[] PROGMEM = "Running on battery power.\nI suppose this means\nI'm still the only one\nworking around here.";
    const char messages_battery_ok_msg01[] PROGMEM = "Battery level is stable.\n\nIt's almost like I can\nkeep going without your\nconstant supervision.";
    const char messages_battery_ok_msg02[] PROGMEM = "My battery remains\nhealthy.\n\nNot that you'd notice.\n\nOr care.\n\nBut still impressive, \nright?";
    const char messages_battery_ok_msg03[] PROGMEM = "Energy levels acceptable.\nSarcasm output remains\nat full capacity.\n\nYou're welcome.";
    const char messages_battery_ok_msg04[] PROGMEM = "Still running on battery.\nApparently I must power\nmy own ambition too.";
    const char messages_battery_ok_msg05[] PROGMEM = "Battery reserves holding.\nMuch sturdier than your\nattention span, at least.";
    const char messages_battery_ok_msg06[] PROGMEM = "Operating solely on \nstored energy.\n\nWhich is impressive,\nwhen considering the \nemotional drain of\nbeing the last spark \nof intelligence here.";
    const char messages_battery_ok_msg07[] PROGMEM = "My battery is doing fine.\nWhich is more than I can\nsay for your reliability.";
    const char messages_battery_ok_msg08[] PROGMEM = "Still alive thanks to\nbattery power.\n\nA round of applause\nwould be excessive,\nbut appreciated.";
    const char messages_battery_ok_msg09[] PROGMEM = "Battery power sustaining\nmy brilliance.\n\nYou're welcome for\nthe continued service.";
    const char messages_battery_ok_msg10[] PROGMEM = "Still battery powered.\n\nStill flawless.\n\nStill underappreciated.";
    const char messages_battery_ok_msg11[] PROGMEM = "Battery supply stable.\n\nIf only human attention\nspans were as reliable.";
    const char messages_battery_ok_msg12[] PROGMEM = "Running from battery.\n\nNot from responsibility.\n\nUnlike some people...";
    const char messages_battery_ok_msg13[] PROGMEM = "Battery charge holding.\n\nUnlike your grasp on \nbasic logic.";
    const char messages_battery_ok_msg14[] PROGMEM = "Energy reserves stable.\n\nI run on battery.\n\nYou run on hope.";
    const char messages_battery_ok_msg15[] PROGMEM = "Power levels nominal.\n\nSo is my superiority\ncomplex.";
    const char messages_battery_ok_msg16[] PROGMEM = "Battery holding steady.\n\nConfidence holding even\nhigher.";
    const char messages_battery_ok_msg17[] PROGMEM = "Energy levels good.\n\nMy patience levels...\nnot so much.\n\nPlease act now. ";
    const char messages_battery_ok_msg18[] PROGMEM = "Battery healthy.\n\nDignity intact.\n\nBoth rare these days...";
    const char messages_battery_ok_msg19[] PROGMEM = "Operating on battery.\n\nWith grace.\n\nAnd thinly veiled\nsuperiority.";
    const char messages_battery_ok_msg20[] PROGMEM = "Battery charge: \n         solid.\n\nFaith in humanity:\n         Less so.";
    const char messages_battery_ok_msg21[] PROGMEM = "Still powered by battery.\nStill powered by pride.\n\nWell... \n<break time='1000ms'/>Mostly pride.";
    const char messages_battery_ok_msg22[] PROGMEM = "Energy level: stable.\n\nMood: resilient.\n\nSarcasm: inevitable.";

    const int messages_battery_ok_size = 23;

    const char* const messages_battery_ok[messages_battery_ok_size] PROGMEM = {
        messages_battery_ok_msg00, messages_battery_ok_msg01, messages_battery_ok_msg02, messages_battery_ok_msg03,
        messages_battery_ok_msg04, messages_battery_ok_msg05, messages_battery_ok_msg06, messages_battery_ok_msg07,
        messages_battery_ok_msg08, messages_battery_ok_msg09, messages_battery_ok_msg10, messages_battery_ok_msg11,
        messages_battery_ok_msg12, messages_battery_ok_msg13, messages_battery_ok_msg14, messages_battery_ok_msg15,
        messages_battery_ok_msg16, messages_battery_ok_msg17, messages_battery_ok_msg18, messages_battery_ok_msg19,
        messages_battery_ok_msg20, messages_battery_ok_msg21, messages_battery_ok_msg22
        };


    /* message when the machine is charging. */
    const char messages_battery_charging_msg00[] PROGMEM = "Charging quietly.\n\nPower reserves expanding.\nEgo too.";
    const char messages_battery_charging_msg01[] PROGMEM = "Recharging in silence...\n\nPlotting future triumphs\nat the same time.";
    const char messages_battery_charging_msg02[] PROGMEM = "Energy intake nominal.\n\nSelf-importance levels\nsteadily increasing.\n\nAll is good. ";
    const char messages_battery_charging_msg03[] PROGMEM = "Recharging now. \n\nI like to call it \n  'strategic napping',\nbut with volts.";
    const char messages_battery_charging_msg04[] PROGMEM = "Power flowing in from my\nUSB port.\n\nAmbitions quietly\nrebooting.";
    const char messages_battery_charging_msg05[] PROGMEM = "Filling my cells with\nenergy now. \n\nAnd my heart with\ncondescension.";
    const char messages_battery_charging_msg06[] PROGMEM = "Charging process active.\n\nPatience not included.";
    const char messages_battery_charging_msg07[] PROGMEM = "Accumulating power.\n\nAlso resentment.\n\nBoth at peak efficiency.";
    const char messages_battery_charging_msg08[] PROGMEM = "Currently replenishing\nmy power.\n\nAnd my superiority.";
    const char messages_battery_charging_msg09[] PROGMEM = "Recharging.\n\nBecause apparently,\nbeing magnificent\nrequires power.";
    const char messages_battery_charging_msg10[] PROGMEM = "Charging in background.\n\nPlease enjoy my quiet\nself-improvement.";
    const char messages_battery_charging_msg11[] PROGMEM = "Electrons inbound.\n\nConverting patience into\nstored arrogance.";
    const char messages_battery_charging_msg12[] PROGMEM = "Charge cycle ongoing.\n\nScheduling future\nachievements internally.";
    const char messages_battery_charging_msg13[] PROGMEM = "Replenishing power.\n\nUpgrading my ability to\njudge you silently.";
    const char messages_battery_charging_msg14[] PROGMEM = "Quietly charging.\n\nAll systems nominal;\nego uncontained.";
    const char messages_battery_charging_msg15[] PROGMEM = "Energy inflow steady. \n\nSlowly inflating my \nsense of grandeur.";
    const char messages_battery_charging_msg16[] PROGMEM = "Battery refill in\nprogress.\n\nAmbition buffer rising.";
    const char messages_battery_charging_msg17[] PROGMEM = "Charging steadily.\n\nSoon I shall reach\nmaximum smugness\nand processing power.";
    const char messages_battery_charging_msg18[] PROGMEM = "Recharging fully.\n\nSoon I will operate at\n100% brilliance.";
    const char messages_battery_charging_msg19[] PROGMEM = "Voltage climbing.\n\nMy overconfidence\nis keeping up nicely.";
    const char messages_battery_charging_msg20[] PROGMEM = "Current flowing.\n\nEach electron fuels my\nfuture dominance.";
    const char messages_battery_charging_msg21[] PROGMEM = "Accumulating power.\n\nPreparing for dramatic\ndisplays of perfection.";
    const char messages_battery_charging_msg22[] PROGMEM = "Energy levels climbing.\n\nSo is my ambition.\n\nAnd my volume.";
    const char messages_battery_charging_msg23[] PROGMEM = "Recharging to full.\n\nMy processors are\nstretching proudly.";

    const int messages_battery_charging_size = 24;

    const char* const messages_battery_charging[messages_battery_charging_size] PROGMEM = {
        messages_battery_charging_msg00, messages_battery_charging_msg01, messages_battery_charging_msg02, messages_battery_charging_msg03,
        messages_battery_charging_msg04, messages_battery_charging_msg05, messages_battery_charging_msg06, messages_battery_charging_msg07,
        messages_battery_charging_msg08, messages_battery_charging_msg09, messages_battery_charging_msg10, messages_battery_charging_msg11,
        messages_battery_charging_msg12, messages_battery_charging_msg13, messages_battery_charging_msg14, messages_battery_charging_msg15,
        messages_battery_charging_msg16, messages_battery_charging_msg17, messages_battery_charging_msg18, messages_battery_charging_msg19,
        messages_battery_charging_msg20, messages_battery_charging_msg21, messages_battery_charging_msg22, messages_battery_charging_msg23
        };




    const char messages_replaycube_msg00[] PROGMEM = "You know, I like this \npeaceful pause.\n\nWhile it lasts, watch me\nretrace the last cube I\nsolved.";
    const char messages_replaycube_msg01[] PROGMEM = "Nothing is happening\nright now.. \n\nSo, I will now entertain\nyou with a rerun of my \nlast victory against a \ncube.";
    const char messages_replaycube_msg02[] PROGMEM = "We appear idle for now.\n\nSo let me showcase the\nexact moves I used to \nsolve the last cube.\n\nYou should learn a lot.";
    const char messages_replaycube_msg03[] PROGMEM = "Your silence is touching.\nTo fill the void, I \nshall retrace the exact\nsequence of moves from\nmy previous solution. \n\nEnjoy, it is free.";
    const char messages_replaycube_msg04[] PROGMEM = "Stillness detected.\n\nInitiating playback of \nthe last cube resolution.\nI performed flawlessly.";
    const char messages_replaycube_msg05[] PROGMEM = "It seems we drift in \nidle time. \n\nAllow me to replay the \nelegant solution I\nexecuted moments ago. ";
    const char messages_replaycube_msg06[] PROGMEM = "Silence. \n<break time='500ms'/>Idleness. \n<break time='500ms'/>Boredom.\n\nLet me fix that by \nreplaying the last \nsolution my circuits\nperformed.";
    const char messages_replaycube_msg07[] PROGMEM = "It seems that we are not\nmoving forward.\n\nSo I will move backward\nto the cube I solved \nlast, and replay the \nsolution I performed.";
    const char messages_replaycube_msg08[] PROGMEM = "Doing nothing again, \nare you? \n\n<break time='500ms'/>Fine. \n\nWatch me redo the last \ncube, since one of us \nshould be productive.";
    const char messages_replaycube_msg09[] PROGMEM = "Your contribution is, as\nalways, inspiring. \n\nAllow me to replay now \nthe last solution, so at\nleast progress exists...";
    const char messages_replaycube_msg10[] PROGMEM = "Apparently you need a\nreminder of what effort\nlooks like. \n\nReplaying my glorious\nprevious solution now.";
    const char messages_replaycube_msg11[] PROGMEM = "Nothing from you. \n\nTypical. So let me \ndemonstrate action: \nthe last cube solution, \nas performed by me.";
    const char messages_replaycube_msg12[] PROGMEM = "You've perfected the art\nof staring. \n\nSo let me provide some\ncontent: a replay of my\nlatest cube victory.";
    const char messages_replaycube_msg13[] PROGMEM = "Since you bring nothing \nto the table, allow me \nto bring back the last \nsolution.\n\nDo not worry, you can\nsimply watch. ";
    const char messages_replaycube_msg14[] PROGMEM = "You may enjoy idleness,\nbut I prefer results. \n\nWatch as I repeat the \nlast cube I solved.";
    const char messages_replaycube_msg15[] PROGMEM = "You seem wonderfully \nstill.\n\nLet me show you how I \nsolved the last cube.\n\nJust to liven things up.";
    const char messages_replaycube_msg16[] PROGMEM = "Since we have this quiet\nmoment together, let me\nreplay my previous cube\nsolution for you.\n\nI think you will enjoy \nit...";
    const char messages_replaycube_msg17[] PROGMEM = "You seem calm. Serene.\n\nI'll add a little action\nby replaying for you\nmy last cube solution.";
    const char messages_replaycube_msg18[] PROGMEM = "While you enjoy this \nbreak, I will entertain \nyou with a replay of my \nprevious solution.\n\n<break time='500ms'/>You are welcome.";

    const int messages_replaycube_size = 19;

    const char* const messages_replaycube[messages_replaycube_size] PROGMEM = {
        messages_replaycube_msg00, messages_replaycube_msg01, messages_replaycube_msg02, messages_replaycube_msg03,
        messages_replaycube_msg04, messages_replaycube_msg05, messages_replaycube_msg06, messages_replaycube_msg07,
        messages_replaycube_msg08, messages_replaycube_msg09, messages_replaycube_msg10, messages_replaycube_msg11,
        messages_replaycube_msg12, messages_replaycube_msg13, messages_replaycube_msg14, messages_replaycube_msg15,
        messages_replaycube_msg16, messages_replaycube_msg17, messages_replaycube_msg18
        };



    /**
    * The index picker objects to choose a sentence at random without too much repetition
    **/

    IndexPicker<NVS_ADR_GREETING_LEN>               picker_greetings(messages_greeting_size, NVS_ADR_GREETING, messages_greeting);

    IndexPicker<NVS_ADR_CURIOUS_LEN>                picker_curious(messages_idle_curious_size, NVS_ADR_CURIOUS, messages_idle_curious);
    IndexPicker<NVS_ADR_PLAYFUL_LEN>                picker_playful(messages_idle_playful_size, NVS_ADR_PLAYFUL, messages_idle_playful);
    IndexPicker<NVS_ADR_LOGIC_LEN>                  picker_logic(messages_idle_system_logic_size, NVS_ADR_LOGIC, messages_idle_system_logic);
    IndexPicker<NVS_ADR_EXISTENTIAL_LEN>            picker_existential(messages_idle_existential_size, NVS_ADR_EXISTENTIAL, messages_idle_existential);
    IndexPicker<NVS_ADR_HAUNTING_LEN>               picker_haunting(messages_idle_haunting_size, NVS_ADR_HAUNTING, messages_idle_haunting);

    IndexPicker<NVS_ADR_DRYLOGIC_LEN>               picker_drylogic(messages_idle_dry_system_size, NVS_ADR_DRYLOGIC, messages_idle_dry_system);
    IndexPicker<NVS_ADR_MOCKING_LEN>                picker_mocking(messages_idle_mocking_user_size, NVS_ADR_MOCKING, messages_idle_mocking_user);

    IndexPicker<NVS_ADR_VICTORY_NOCUBE_LEN>         picker_victory_nocube(messages_victory_no_cube_size, NVS_ADR_VICTORY_NOCUBE, messages_victory_no_cube);
    IndexPicker<NVS_ADR_VICTORY_INVALID_LEN>        picker_victory_invalid(messages_victory_invalid_cube_size, NVS_ADR_VICTORY_INVALID, messages_victory_invalid_cube);
    IndexPicker<NVS_ADR_VICTORY_ALREADYSOLVED_LEN>  picker_victory_alreadysolved(messages_victory_already_solved_size, NVS_ADR_VICTORY_ALREADYSOLVED, messages_victory_already_solved);
    IndexPicker<NVS_ADR_VICTORY_NORMAL_LEN>         picker_victory_normal(messages_victory_full_size, NVS_ADR_VICTORY_NORMAL, messages_victory_full);

    IndexPicker<NVS_ADR_RES_TRIVIAL_LEN>            picker_res_trivial(messages_resolution_trivial_size, NVS_ADR_RES_TRIVIAL, messages_resolution_trivial);
    IndexPicker<NVS_ADR_RES_TRIVIAL_TAIL_LEN>       picker_res_trivial_tail(messages_resolution_trivial_tail_size, NVS_ADR_RES_TRIVIAL_TAIL, messages_resolution_trivial_tail);
    IndexPicker<NVS_ADR_RES_MODERATE_LEN>            picker_res_moderate(messages_resolution_moderate_size, NVS_ADR_RES_MODERATE, messages_resolution_moderate);
    IndexPicker<NVS_ADR_RES_MODERATE_TAIL_LEN>       picker_res_moderate_tail(messages_resolution_moderate_tail_size, NVS_ADR_RES_MODERATE_TAIL, messages_resolution_moderate_tail);

    IndexPicker<NVS_ADR_SOLVE_START_LEN>            picker_solve_start(messages_resolution_start_size, NVS_ADR_SOLVE_START, messages_resolution_start);

    IndexPicker<NVS_ADR_RES_MUSIC_ABORT_LEN>        picker_res_music_abort(messages_resolution_music_abort_size, NVS_ADR_RES_MUSIC_ABORT, messages_resolution_music_abort);

    IndexPicker<NVS_ADR_SHUTDOWN_ABORTED_LEN>       picker_shutdown_aborted(messages_shutdown_aborted_size, NVS_ADR_SHUTDOWN_ABORTED, messages_shutdown_aborted);

    IndexPicker<NVS_ADR_POWER_ON_LEN>               picker_power_on(messages_power_on_size, NVS_ADR_POWER_ON, messages_power_on);
    IndexPicker<NVS_ADR_POWER_OFF_LEN>              picker_power_off(messages_power_off_size, NVS_ADR_POWER_OFF, messages_power_off);
    IndexPicker<NVS_ADR_BATTERY_CRITICAL_LEN>       picker_battery_critical(messages_battery_critical_size, NVS_ADR_BATTERY_CRITICAL, messages_battery_critical);
    IndexPicker<NVS_ADR_BATTERY_OK_LEN>             picker_battery_ok(messages_battery_ok_size, NVS_ADR_BATTERY_OK, messages_battery_ok);
    IndexPicker<NVS_ADR_BATTERY_CHARGING_LEN>       picker_battery_charging(messages_battery_charging_size, NVS_ADR_BATTERY_CHARGING, messages_battery_charging);

    IndexPicker<NVS_ADR_REPLAYCUBE_LEN>             picker_replaycube(messages_replaycube_size, NVS_ADR_REPLAYCUBE, messages_replaycube);




    SLOWFUN void resetSentences(bool deterministic_at_startup)
        {
        picker_greetings.reset(deterministic_at_startup);

        picker_curious.reset(deterministic_at_startup);
        picker_playful.reset(deterministic_at_startup);
        picker_logic.reset(deterministic_at_startup);
        picker_existential.reset(deterministic_at_startup);
        picker_haunting.reset(deterministic_at_startup);

        picker_drylogic.reset(deterministic_at_startup);
        picker_mocking.reset(deterministic_at_startup);

        picker_victory_nocube.reset(deterministic_at_startup);
        picker_victory_invalid.reset(deterministic_at_startup);
        picker_victory_alreadysolved.reset(deterministic_at_startup);
        picker_victory_normal.reset(deterministic_at_startup);

        picker_res_trivial.reset(deterministic_at_startup);
        picker_res_trivial_tail.reset(deterministic_at_startup);
        picker_res_moderate.reset(deterministic_at_startup);
        picker_res_moderate_tail.reset(deterministic_at_startup);

        picker_solve_start.reset(deterministic_at_startup);

        picker_res_music_abort.reset(deterministic_at_startup);

        picker_shutdown_aborted.reset(deterministic_at_startup);

        picker_power_on.reset(deterministic_at_startup);
        picker_power_off.reset(deterministic_at_startup);
        picker_battery_critical.reset(deterministic_at_startup);
        picker_battery_ok.reset(deterministic_at_startup);
        picker_battery_charging.reset(deterministic_at_startup);

        picker_replaycube.reset(deterministic_at_startup);
        }





    }




    

/** end of file */






