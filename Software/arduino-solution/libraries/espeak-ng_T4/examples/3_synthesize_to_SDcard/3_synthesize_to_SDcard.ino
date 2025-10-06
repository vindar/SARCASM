/*******************************************************************
* Example 'synthesize_to_SDcard' for the epseak-ng_T4 library.
*
* Show how to use espeak.synthesize() method to get the audio of 
* a text without actually playing it. Then, the audio samples are
* save as a wav file using the internal SD card
*
* Require a Teensy 4.1 with a valid, formatted, SD card inserted
* in the internal SD slot.
********************************************************************/
#include <Arduino.h>
#include <SD.h> // SDFat, bundled with teensyduino. 

#include <espeak-ng_T4.h> // the espeak-ng library for Teensy 4.x



// the memory buffer used to receive the audio samples. 
#define BUFFER_NB_SAMPLES_SIZE 120000
int16_t buffer[BUFFER_NB_SAMPLES_SIZE]; 


// write the WAV header to the file
void writeWavHeader(File& f, int numSamples, int sampleRate) 
    {
    int dataSize = numSamples * 2;
    f.write("RIFF", 4);
    uint32_t chunkSize = dataSize + 36;
    f.write((uint8_t*)&chunkSize, 4);
    f.write("WAVE", 4);
    f.write("fmt ", 4);
    uint32_t subChunk1Size = 16;
    f.write((uint8_t*)&subChunk1Size, 4);
    uint16_t audioFormat = 1;
    f.write((uint8_t*)&audioFormat, 2);
    uint16_t numChannels = 1;
    f.write((uint8_t*)&numChannels, 2);
    f.write((uint8_t*)&sampleRate, 4);
    uint32_t byteRate = sampleRate * 2;
    f.write((uint8_t*)&byteRate, 4);
    uint16_t blockAlign = 2;
    f.write((uint8_t*)&blockAlign, 2);
    uint16_t bitsPerSample = 16;
    f.write((uint8_t*)&bitsPerSample, 2);
    f.write("data", 4);
    f.write((uint8_t*)&dataSize, 4);
    }


const char* text = "This is a test of the espeak-ng library for Teensy 4.1";

const char* filename = "test.wav";


void setup() 
        {
        Serial.begin(115200);
        while ((!Serial)&&(millis()<3000)) { delay(10); } // wait a bit for serial to be ready. 

        espeak.begin();                 // initialise the espeak-ng library -> english in loaded automatically. 
        espeak.registerAllVariants();   // now we can use all voice variants. 
        espeak.setRate(120);            // don't speak too fast. 
        espeak.setVoice("en+linda");    // use linda's voice, because why not...

        // perform synthesis into the RAM buffer
        Serial.printf("Synthesizing text: [%s]\n", text);
        int nbsamples = espeak.synthesize(text, buffer, BUFFER_NB_SAMPLES_SIZE); 
        Serial.printf("Synthesized %d samples.\n", nbsamples);
        if (nbsamples > BUFFER_NB_SAMPLES_SIZE)
            { // check for buffer overflow
            Serial.printf("Error: buffer overflow, nbsamples (%d) > BUFFER_NB_SAMPLES_SIZE (%d)\n", nbsamples, BUFFER_NB_SAMPLES_SIZE);
            nbsamples = BUFFER_NB_SAMPLES_SIZE; // real number of samples in the buffer (the remainnig one were discarded). 
            return;
            }

        // write the buffer to a WAV file on the SD card
        if (!SD.begin(BUILTIN_SDCARD)) { Serial.println("SD card init failed!"); return; }
        
        File file = SD.open(filename, FILE_WRITE);        
        if (!file)  { Serial.printf("Failed to create file [%s]\n", filename); return; }
        
        writeWavHeader(file, nbsamples, 22050); // write header        
        file.write((uint8_t*)buffer, nbsamples*2); // write data
        
        file.close();         
        Serial.printf("WAV file written to [%s]\n", filename);
        }



void loop()
    {
    delay(100);
    }
