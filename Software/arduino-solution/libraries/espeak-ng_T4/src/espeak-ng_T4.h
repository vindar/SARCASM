
#include "speak_lib.h"
#include  <Arduino.h>
#include "AudioStream.h"


// macro to access the singleton instance
#define espeak (EspeakNG::getInstance())

// signature of the user defined callback function when synthetizing audio data
typedef int (*p_synthetize_cb)(const int16_t * audiobuffer, int nb_samples, void* userdata);

// signature for the custom delay function 
typedef void (*p_delay_cb)(uint32_t ms);


/*************************************************************************************
************************************************************************************** 
* 
* This class is a C++ wrapper for the espeak-ng C API. 
*
* ** Singleton class ** : there is only one instance of this class nameed 'espeak'. 
* 
**************************************************************************************
***************************************************************************************/
class EspeakNG : public AudioStream
    {

    public:


        /**
        * Initialize the espeak-ng library. This method must be called first.
        *
        * By default, all dynamic memory allocation is done in DMAMEM (around 150kb needed).
        * 
        * Set `alloc_in_EXTMEM` to 1 to use EXTMEM instead (if present).
        *
        * Return 0 if initialization ok.
        **/
        int begin(int alloc_in_EXTMEM = 0);



        /**********************************************
        *
        * Loading languages / voices / variants
        *
        ***********************************************/

        /**
        * Register a language dictionary. Return 0 on success or negative value on error.
        *
        * Example:  #include "espeak-ng-data/fr_dict.h"   // <- contain the array 'espeak_ng_data_fr_dict'
        *           espeak_RegisterDict("fr", espeak_ng_data_fr_dict, espeak_ng_data_fr_dict_len);
        **/
        int registerDict(const char* language_name, const unsigned char* dict_data, int dict_data_len);


        /**
        * Register a language voice. Return 0 on success or negative value on error.
        *
        * Example:  #include "espeak-ng-data/lang/roa/fr.h"   // <- contain the array espeak_ng_data_lang_roa_fr
        *           espeak_RegisterLang("fr", espeak_ng_data_lang_roa_fr, espeak_ng_data_lang_roa_fr_len);
        **/
        int registerLang(const char* voice_name, const unsigned char* voice_data, int voice_data_len);


        /**
        * Register a voice variant. Return 0 on success or negative value on error.
        *
        * Example: #include "espeak-ng-data/voices/!v/Alicia.h"   // <- contain the array espeak_ng_data_voices__v_Alicia
        *          espeak_registerVoiceVariant("Alicia", espeak_ng_data_voices__v_Alicia, espeak_ng_data_voices__v_Alicia_len);
        **/
        int registerVariant(const char* variant_name, const unsigned char* variant_data, int variant_data_len);


        /**
        * Register all the voice variants located at src/espeak-ng-data/voices/!v/.
        *
        * This provide around 100 variants while using only 50kB of FLASH memory (and no RAM).
        **/
        void registerAllVariants();



        /**********************************************
        *
        * Audio synthesis
        *
        ***********************************************/


        /**
        * Play a text using the audio library object.
        *
        * - This method only returns after the text has been fully played.
        * - If 'outtext' is specified, then the text is printed in sync with the voice (without the SSML tags)     
        *  
        * Note: the Print object 'outtext' is called from within the audio library update() method
        *       hence must interrupt safe...
        **/
        void play(const char* text, Print* outtext = nullptr);


        /**
        * Synthetize the text and put the raw samples data in the buffer (16 bits signed, 22050Hz).
        *
        * Return the number of samples of the whole audio. This value may be larger than 
        * `buffer_samples_size` which means that the buffer is full and the remaining  
        *  audio data was discarded.
        * 
        * Note: Call this method with audiobuffer=nullptr, buffer_samples_size=0 to get the size 
        *       of the audio data that would be generated for the text.
        **/
        int synthesize(const char* text, int16_t* audiobuffer, int buffer_samples_size);


        /**
        * Synthetize the text and call the callback function repeateadly to provide the audio data.
        * 
        * The callback has signature: 
        * 
        *                   int callback(const int16_t * audiobuf, int nbsamples, void * userdata);
        *
        * - `nbsamples` is the number of samples available in the buffer `audiobuf`.  
        * - `userdata` is the same pointer passed to this method and can be used to pass any user data to the callback.    
        * - The callback must return 0 to continue the synthesis, or non-zero to stop.   
        * 
        * The method returns the number of sample synthetized.
        **/
        int synthesize(const char* text, p_synthetize_cb callback, void * userdata = nullptr);




        /**
        * Compute the timestamps for each character of a text, in ms, from the begining of the text.
        * 
        * - Return the number of characters in the text. This value may be smaller than the
        *   number of 'char' in the null terminated c-string 'text' when using utf8 encoding.
        *
        * - If 'letter_timestamps_ms' is null, the method simply return the number of characters   
        *   in the text, otherwise, obviously, 'letter_timestamps_ms' must be large enough to hold
        *   the timestamps.
        **/
        int timestamps(const char* text, int * letter_timestamps_ms);


        /**
        * Return the duration, in ms, of the text when spoken with the current voice parameters. 
        **/
        int duration_ms(const char* text);



        /**********************************************
        *
        * Voice synthesis parameters
        *
        ***********************************************/


        /**
        * Set the voice to use for synthesis, with a possible variant.
        *
        * example for french with Alicia variant: setVoice("fr+Alicia");
        *
        * Note:
        * - the corresponding language/dictionary must be registered prior to this call.
        * - the english voice "en" is always available (and is the default voice if none is selected).
        **/
        int setVoice(const char* voice_name = "en");


        /**
        * Set the speaking speed in word per minute. Values 80 to 450 (normal = 175). Return 0 if ok.
        **/
        int setRate(int rate);


        /**
        * Set the volume in range 0-200 or more (0=silence, 100=normal full volume, greater values may 
        * produce amplitude compression or distortion).
        * Return 0 if ok.
        **/
        int setVolume(int volume);


        /**
         * Set the base pitch. range 0-100 (50=normal). Return 0 if ok.
         **/
        int setPitch(int base_pitch);


        /**
         * Set the pitch range. range 0-100 (0-monotone, 50=normal). Return 0 if ok.
         **/
        int setRange(int pitch_range);


        /**
        * Set whether to announce punctuation characters. Return 0 if ok.
        **/
        int setPunctuation(bool announcePunctuation);


        /**
        * Set how to announce capital letters. Return 0 if ok.
        *  0 = none,
        *  1 = sound icon,
        *  2 = spelling,
        *  3 or higher = amount in Hz by which the pitch of a word raised to indicate it has a capital letter.
        **/
        int setCapitals(int method, int pitch_raise = 0);


        /**
        * Set the pause between words, in ms (at the default speed). Return 0 if ok.
        **/
        int setWordGap(int ms);




        /**********************************************
        *
        * Memory managmement
        *
        ***********************************************/


        /**
        * Return the location use for dynamic memory allocation.
        *
        * return 0 if DMAMEM is used, 1 if EXTMEM is used.
        **/
        int getMemoryLocation();


        /**
        * Return the number of byte of dynamic memory currently allocated by the library
        **/
        int memoryCurrent();


        /**
        * Return maximum number of byte of dynamic memory used by the library (so far).
        **/
        int memoryMax();




        /*****************************************************************************
        * For async play using teensythreads
        ******************************************************************************/
        
        /**
        * Set the abort_now flag to 1 to signify to the espeak thread that play() should end.
        **/        
        void sendAbortSignal(); 
        

        /**
        * Set a custom 'void delay(int ms)' function to use instead of Arduino's delay() function.
        * 
        * This is useful to replace delay() by teensythreads's 'threads.delay()' function when 
        * running the speak() command inside a teensythreads thread.
        * 
        * Call this method without paramter to restore the default Arduino's delay().
        **/
        void setDelayFunction(p_delay_cb delay_function = nullptr)
            {
            _delay_fun = (!delay_function) ? _default_delay : delay_function;
            }


        /*****************************************************************************
        * IMPLEMENTATION DETAILS
        ******************************************************************************/


        /**
        * Static method to get the singleton instance of the EspeakNG class.
        **/
        static EspeakNG& getInstance();



        /**
        * Override of `AudioStream::update()` virtual method called by the Audio library.
        **/
        virtual void update() override; 



    private:

        /*****************************************************************************
        * Don't look below, it is my private space... And I like it dirty ! 
        ******************************************************************************/


        EspeakNG() : AudioStream(0, NULL), _initdone(false), _abort_now(0), _audiobuffer(nullptr), _ts(nullptr), _outputletter(nullptr), _text(nullptr), _delay_fun(_default_delay) {}
        EspeakNG(const EspeakNG&) = delete; // prevent copying
        EspeakNG& operator=(const EspeakNG&) = delete; // prevent assignment

        volatile bool _initdone;        //
        volatile int  _abort_now;
        void _checkinit();              // makes sure initialization was performed before calling espeak C api 

        int16_t* volatile _audiobuffer;      // pointer to the audio buffer (16 bits signed, 22050Hz)    
        volatile int _written_audiobuf; // number of sample written in the audio buffer
        volatile int _read_audiobuf;    // number of sample read from the audio buffer by update()

        int* volatile _ts; // timestamps
        volatile int _ts_pos_max;
        volatile int _ts_pos;
        volatile int _ts_char;
        volatile bool _isutf8;
        volatile bool _insidessml;
        Print * volatile _outputletter;

        const char* volatile  _text;    // text to synthetize
        volatile int _nbsamples; // number of samples processed

        elapsedMillis _em; // for timing. 


        volatile p_delay_cb _delay_fun; // pointer to the delay function to use (default is Arduino's delay())

        static void _default_delay(uint32_t ms) { delay(ms); } // default delay function

        p_synthetize_cb _usercallback; // user defined callback function for method synthetize()
        void* volatile _userdata; // user data for the callback

        static int _play_cb_static(short* wav, int numsamples, espeak_EVENT* events) { return getInstance()._play_cb(wav, numsamples, events); }
        int _play_cb(short* wav, int numsamples, espeak_EVENT* events); // espeak callback for method play()

        static int _letter_cb_static(short* wav, int numsamples, espeak_EVENT* events) { return getInstance()._letter_cb(wav, numsamples, events); }
        int _letter_cb(short* wav, int numsamples, espeak_EVENT* events); // espeak callback for method timestamps()

        static int _synthbuffer_cb_static(short* wav, int numsamples, espeak_EVENT* events) { return getInstance()._synthbuffer_cb(wav, numsamples, events); }
        int _synthbuffer_cb(short* wav, int numsamples, espeak_EVENT* events); // espeak callback for method timestamps()

        static int _synthcallback_cb_static(short* wav, int numsamples, espeak_EVENT* events) { getInstance()._nbsamples += numsamples;  return getInstance()._usercallback((const int16_t*)wav, numsamples, getInstance()._userdata); }

    };

    /** end of file */
