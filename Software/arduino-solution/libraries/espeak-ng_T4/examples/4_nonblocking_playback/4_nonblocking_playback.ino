/*******************************************************************
* Example 'nonblocking_playback' for the epseak-ng_T4 library.
*
* Show how to play audio synthesis in the background while programm
* is doing other thing:
*
*  - The espeak-ng C API does not provide a 'polling mode' to make 
*    play() non-blocking but this behaviour can emulated by running 
*    espeack inside its own thread, using teensythreads library.
*
* Require a Teensy 4/4.1 and the Audio shield. 
********************************************************************/
#include <Arduino.h>
#include <Audio.h> // PJRC audio library.
#include <TeensyThreads.h> // Teensy threads library bundeled with Teensyduino
#include <espeak-ng_T4.h> // the espeak-ng library for Teensy 4.x


// Teensy audio system: assuming here that we are using the audio shield. 
AudioControlSGTL5000 sgtl5000;
AudioOutputI2S audioOut;
AudioConnection patchCord1(espeak, 0, audioOut, 0); // 'espeak' is the main object of the library. 
AudioConnection patchCord2(espeak, 0, audioOut, 1); // -> we link it to the I2S output on the shield. 



// the espeak-ng library uses around 6000 bytes of stack during playback. 
// let's allocate 10000 bytes for the thread stack to be safe. 
#define ESPEAK_THREAD_STACK_SIZE 10000

//
// the thread for espeak playback. 
//
void espeak_thread(int val)
    {
    // espeak is not thread safe.
    // It is better to call it from a single thread so we initialise it here instead of inside setup()
    espeak.begin();  
    espeak.setRate(200);

    char number_str[12]; 
    int count = 0;

    while (1)
        {
        sprintf(number_str, "%d", ++count); // write the number to a string.
        espeak.play(number_str); // speak the number.
        delay(200); // wait a bit before the next number.

        }
    }



void setup() 
        {
        Serial.begin(115200);
        while ((!Serial) && (millis() < 3000)) { delay(10); } // wait a bit for serial to be ready. 

        AudioMemory(8);  // Allocate some memory for audio processing
        sgtl5000.enable(); // Enable the audio shield
        sgtl5000.volume(0.5f); // Set output volume

        // start the espeak thread which plays the audio.
        Serial.println("Starting the audio thread.");
        threads.setSliceMillis(1);
        threads.addThread(espeak_thread, 0, ESPEAK_THREAD_STACK_SIZE);
        }



void loop()
    {
    // here we could do something useful while the audio in playing in the background...
    Serial.print("\ndoing work while playing audio: ");
    for (int i = 0; i < 30; i++) { Serial.print("."); delay(100); }
    }

