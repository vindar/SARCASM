/*******************************************************************
* Example 'ssml_and_multilang' for the epseak-ng_T4 library.
*
* Show how to use multiple languages and play a text with SSML tags. 
*
* Require a Teensy 4/4.1 and the Audio shield. 
********************************************************************/
#include <Audio.h>

#include <espeak-ng_T4.h> // the espeak-ng library for Teensy 4.x

#include <espeak-ng-data/lang/roa/fr.h>   // for French 
#include <espeak-ng-data/fr_dict.h>       //

#include <espeak-ng-data/lang/gmw/de.h>   // for German
#include <espeak-ng-data/de_dict.h>       //

#include <espeak-ng-data/lang/roa/it.h>   // for Italian 
#include <espeak-ng-data/it_dict.h>       //


// Teensy audio system : assuming here that we are using the audio shield. 
AudioControlSGTL5000 sgtl5000;
AudioOutputI2S audioOut;
AudioConnection patchCord1(espeak, 0, audioOut, 0); // 'espeak' is the main object of the library. 
AudioConnection patchCord2(espeak, 0, audioOut, 1); // -> we link it to the I2S output on the shield. 



void setup() 
        {
        Serial.begin(115200);
        while ((!Serial)&&(millis()<3000)) { delay(10); } // wait a bit for serial to be ready. 

        AudioMemory(8);  // Allocate some memory for audio processing

        sgtl5000.enable();      // Enable the audio shield
        sgtl5000.volume(0.5f);  // Set output volume

        espeak.begin(1);                // initialise the espeak-ng library -> english in loaded automatically. 
        espeak.registerAllVariants();   // load all voices variants (consumes 50KB in FLASH but nothing in RAM)        

        espeak.registerLang("fr", espeak_ng_data_lang_roa_fr, espeak_ng_data_lang_roa_fr_len);  // now we can also speak french...
        espeak.registerDict("fr", espeak_ng_data_fr_dict, espeak_ng_data_fr_dict_len);          //

        espeak.registerLang("de", espeak_ng_data_lang_gmw_de, espeak_ng_data_lang_gmw_de_len);  // and german...
        espeak.registerDict("de", espeak_ng_data_de_dict, espeak_ng_data_de_dict_len);          //

        espeak.registerLang("it", espeak_ng_data_lang_roa_it, espeak_ng_data_lang_roa_it_len);  // and italian !
        espeak.registerDict("it", espeak_ng_data_it_dict, espeak_ng_data_it_dict_len);          //

        }



// The text to speak, with SSML tags. See https://github.com/espeak-ng/espeak-ng/blob/master/docs/markup.md for details.   
//
// Note:
// - the text is utf8 encoded so that accent are correctly voiced (and printed out on Serial).
// - do not use the <speak> tag (may cause problem). 

const char* dialogue = u8R"(
<voice name="en" variant="Andy">Hello everyone! Let's see who is online today.</voice>

<break time='500ms'/> 

<voice name="en" variant="robosoft7">French user has joined the chat. </voice>

<voice name="fr" variant="f3"><prosody pitch="+20%" rate="90%">Bonjour à tous ! Je suis ravie d’être ici.</prosody></voice>

<voice name="en" variant="robosoft7">German user has joined the chat. </voice>

<voice name="de" variant="m1"><prosody volume="x-loud">Guten Tag! Schön, euch zu hören.</prosody></voice>

<voice name="en" variant="robosoft7">Italian user has joined the chat. </voice>

<voice name="it" variant="f5"><prosody rate="110%" pitch="+5%">Ciao amici! Come va?</prosody></voice>

<break time="500ms"/>

<voice name="fr" variant="f3">Oh, quelle belle diversité linguistique !</voice>

<voice name="de" variant="m1"><prosody rate="70%">Ja, das ist wirklich international.</prosody></voice>

<voice name="it" variant="f5">La lingua unisce le persone, anche se vengono da mondi diversi.</voice>

<voice name="en" variant="robosoft8">SYSTEM CHECK COMPLETE. <break time="300ms"/> ALL VOICES ACTIVE.</voice>

<voice name="en" variant="f5"><prosody range="x-high">Ok... that was a little scary.</prosody></voice>

<voice name="fr" variant="f3">Mais non, c'était amusant !</voice>

<voice name="en" variant="Andy"><prosody rate="105%" pitch="+5%">Alright, thanks everyone. See you next time!</prosody></voice>
)";



void loop()
    {
    espeak.play(dialogue, &Serial); // speak through the Audio library and print letters to Serial in sync with the voice. 
    Serial.println("----------------------------------------------\n");
    delay(10000);
    }
