/*********************************************
* Example 'echo' for the epseak-ng_T4 library.
*
* Play an english text queried from serial. 
*
* Require a Teensy 4/4.1 and the Audio shield. 
*********************************************/
#include <Arduino.h>
#include <Audio.h> // PJRC audio library.

#include <espeak-ng_T4.h> // the espeak-ng library for Teensy 4.x

// Teensy audio system: assuming here that we are using the audio shield. 
AudioControlSGTL5000 sgtl5000;
AudioOutputI2S audioOut;
AudioConnection patchCord1(espeak, 0, audioOut, 0); // 'espeak' is the main object of the library. 
AudioConnection patchCord2(espeak, 0, audioOut, 1); // -> we link it to the I2S output on the shield. 


void setup() 
        {
        Serial.begin(115200);
        while ((!Serial)&&(millis()<3000)) { delay(10); } // wait a bit for serial to be ready. 

        AudioMemory(8);  // Allocate some memory for audio processing
        sgtl5000.enable(); // Enable the audio shield
        sgtl5000.volume(0.5f); // Set output volume

        espeak.begin(1);                // initialise the espeak-ng library -> english in loaded automatically. 
        espeak.registerAllVariants();   // load all voices variants (consumes an additional 50KB in FLASH but nothing in RAM)        
        espeak.setRate(150);            // speak at a normal pace
        }


void loop()
    {
    // query the text to speak from Serial. 
    espeak.setVoice("en+Andy"); // speak in english with Andy voice variant
    espeak.play(u8"Please enter the text to speak: ", &Serial); // speak through the Audio library and print letters to Serial in sync with the voice. 

    while (!Serial.available()) { delay(10); } // wait for user to input the text. 
    String s = Serial.readStringUntil('\n');   //   

    espeak.setVoice("en+f3"); // select the female voice variant f3
    Serial.print("\n[");
    espeak.play(s.c_str(), &Serial); // speak through the Audio library and print letters to Serial in sync with the voice. 
    Serial.print("]\n\n");

    delay(1000);
    }
