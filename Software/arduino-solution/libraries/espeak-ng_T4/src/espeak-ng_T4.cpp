#include "espeak-ng_T4.h"
#include "speak_lib.h"
#include "AudioStream.h"
#include "myalloc.h"


#define AUDIOBUFFER_SIZE 3969 // 180ms of audio at 22050Hz = 7.7KB of RAM
#define TIMEOUT_PLAY_MS 200 // timeout for play(). Should give enough time to audiobuffer to become empty. 



/** find the first utf8 codepoint in the string and return its size in bytes.
    set codepoint to -1 if invalid. Return 0 if end of string or invalid codepoint.*/
static int get_utf8(const char* s, int& codepoint)
    {   
    if (s == nullptr)
        {
        codepoint = -1;
        return 0;
        }
    const uint8_t* bytes = (const uint8_t*)s;
    if (bytes[0] == 0)
        {
        codepoint = 0;
        return 0;
        }
    if (bytes[0] <= 0x7F)
        {
        codepoint = bytes[0];
        return 1; // 1-byte codepoint
        }
    if ((bytes[0] & 0xE0) == 0xC0)
        { // 2-byte sequence: 110xxxxx 10xxxxxx
        if ((!bytes[1]) || (bytes[1] & 0xC0) != 0x80) { codepoint = -1; return 0; }
        codepoint = ((bytes[0] & 0x1F) << 6) | (bytes[1] & 0x3F);
        if (codepoint < 0x80) { codepoint = -1;  return 0; } // overlong
        return 2; // 2-bytes codepoint
        }
    if ((bytes[0] & 0xF0) == 0xE0)
        { // 3-byte sequence: 1110xxxx 10xxxxxx 10xxxxxx
        if ((!bytes[1]) || (!bytes[2]) || (bytes[1] & 0xC0) != 0x80 || (bytes[2] & 0xC0) != 0x80) { codepoint = -1;  return 0; }
        codepoint = ((bytes[0] & 0x0F) << 12) | ((bytes[1] & 0x3F) << 6) | (bytes[2] & 0x3F);
        if (codepoint < 0x800) { codepoint = -1;  return 0; } // overlong
        if (codepoint >= 0xD800 && codepoint <= 0xDFFF) { codepoint = -1;  return 0; } // surrogate                        
        return 3; // 3-bytes codepoint
        }
    if ((bytes[0] & 0xF8) == 0xF0)
        { // 4-byte sequence: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        if ((!bytes[1]) || (!bytes[2]) || (!bytes[3]) || (bytes[1] & 0xC0) != 0x80 || (bytes[2] & 0xC0) != 0x80 || (bytes[3] & 0xC0) != 0x80) { codepoint = -1;  return 0; }
        codepoint = ((bytes[0] & 0x07) << 18) | ((bytes[1] & 0x3F) << 12) | ((bytes[2] & 0x3F) << 6) | (bytes[3] & 0x3F);
        if (codepoint < 0x10000 || codepoint > 0x10FFFF) { codepoint = -1;  return 0; } // overlong or too big
        return 4; // 4-bytes codepoint
        }
    codepoint = -1;
    return 0;
    }


/**
* Return the number of utf-8 characters in the string
* - return 0 for the empty string.
* - return -1 if s=null or is not a valid utf8 string.
**/
static int strlen_utf8(const char* s)
    {
    int codepoint;
    int l;
    int nbc = 0;
    while ((l = get_utf8(s, codepoint)) > 0) { s += l; nbc++; }
    return (codepoint >= 0) ? nbc : -1; 
    }







EspeakNG& EspeakNG::getInstance()
    {
    static EspeakNG instance;
    return instance;
    }

FLASHMEM void EspeakNG::_checkinit()
    {
    if (_initdone) return;
    begin(); // use default initialization
    }


FLASHMEM int EspeakNG::begin(int alloc_in_EXTMEM)
    {
    // allow multiple call to reset.
    espeak_SetMemoryLocation(alloc_in_EXTMEM);
    const int options = 0; // no phoneme event. 
    const int buff_ms = 60; // 60ms buffer -> 22050Hz = 1323 samples = 2.6KB of RAM. 
    const int result = espeak_Initialize(AUDIO_OUTPUT_RETRIEVAL, buff_ms, nullptr, options);
    if (result != 22050) return -1; // error
    if (setVoice("en") != 0) return -2; // set default voice to english
    _initdone = true;
    _abort_now = 0;
    return 0;
    }


FLASHMEM int EspeakNG::registerDict(const char* language_name, const unsigned char* dict_data, int dict_data_len)
    { // may be called before begin()
    return espeak_RegisterDict(language_name, dict_data, dict_data_len);
    }


FLASHMEM int EspeakNG::registerLang(const char* voice_name, const unsigned char* voice_data, int voice_data_len)
    { // may be called before begin()
    return espeak_RegisterLang(voice_name, voice_data, voice_data_len);
    }


FLASHMEM int EspeakNG::registerVariant(const char* variant_name, const unsigned char* variant_data, int variant_data_len)
    { // may be called before begin()
    return espeak_RegisterVoiceVariant(variant_name, variant_data, variant_data_len);
    }


FLASHMEM void EspeakNG::registerAllVariants()
    { // may be called before begin()
    espeak_RegisterAllVariants();
    }


FLASHMEM int EspeakNG::setVoice(const char* voice_name)
    {
    return espeak_SetVoiceByName(voice_name);
    }



FLASHMEM void EspeakNG::play(const char * text, Print* outputLetters)
    {
    _checkinit();
    _abort_now = 0;
    if (!text || !*text) return; // nothing to play

    _isutf8 = (strlen_utf8(text) >= 0); // check if the text is a valid utf8 string

    // compute char timestamps if needed
    if (outputLetters)
        {        
        const int l = (_isutf8 ? strlen_utf8(text) : strlen(text)); // number of letters: may be less than L when using utf8 encoding...
        int* ts = (int*)my_malloc(l * sizeof(int)); // allocate the timestamps array
        if (!ts) return; 
        timestamps(text, ts); // compute the timestamps for each letter
        _outputletter = outputLetters;
        _ts = ts; // restore
        _text = text; // restore               
        _ts_pos_max = l;
        _ts_pos = 0; // current character index
        _ts_char = 0; // position in the c-array
        _insidessml = false; // not inside a SSML tag
        }
    else
        { // make sure to disable timestamps
        _outputletter = outputLetters;
        _ts = nullptr;
        }

    // allocate the audio buffer
    int16_t * const buf = (int16_t*)my_malloc(AUDIOBUFFER_SIZE * sizeof(int16_t));
    if (!buf) return;

    // set the buffer and enable update()
    noInterrupts();
    _written_audiobuf = 0; 
    _read_audiobuf = 0; 
    _audiobuffer = buf;
    interrupts();
        
    espeak_SetSynthCallback(EspeakNG::_play_cb_static); // set the espeak cb. 
    _em = 0; // reset the timer
    espeak_Synth(text, strlen(text)+1, 0, POS_CHARACTER, 0, (_isutf8 ? espeakCHARS_UTF8 : espeakCHARS_8BIT) | espeakSSML, nullptr, nullptr); // perform synthesis

    // wait for the audio to be played completely
    elapsedMillis lem = 0; 
    while ((_read_audiobuf < _written_audiobuf) && (lem < TIMEOUT_PLAY_MS)) { _delay_fun(1); }

    // disable update()
    noInterrupts();
    _audiobuffer = nullptr; 
    _written_audiobuf = 0;
    _read_audiobuf = 0;
    interrupts();

    my_free(buf); // now we can free the audio buffer
    if (_ts)
        {
        my_free(_ts);
        _ts = nullptr;
        _outputletter = nullptr;
        }
    _abort_now = 0;
    return;
    }



#define TAG_SENTENCE    (1<<30)
#define TAG_END         (1<<29)
#define TAG_MASK        (~(TAG_SENTENCE | TAG_END)) // mask for the tags

#define TAG_SENTENCE_MS 600 // duration of SENTENCE tag
#define TAG_PAUSE_MS 300 // duration of a END tag


FLASHMEM int EspeakNG::_letter_cb(short* wav, int numsamples, espeak_EVENT* events)
    {
    if ((wav) && (numsamples))  _nbsamples += numsamples;
    //Serial.printf("%d tot: %d  (%d ms)\n", numsamples, _nbsamples, _nbsamples * 100 / 2205);
    if (_ts)
        {
        for (int i = 0; events[i].type != espeakEVENT_LIST_TERMINATED; i++)
            {
            const espeak_EVENT& ev = events[i];
            switch (ev.type)
                {
                case espeakEVENT_WORD:
                    {
                    int& v = _ts[ev.text_position-1];
                    if (v < 0) v = ev.audio_position;  // ony record timestamps if not previous one. 
                    //Serial.printf("[WORDA] at %d ms at pos %d len %d\n", v, ev.text_position - 1, ev.length);
                    break;
                    }
                case espeakEVENT_SENTENCE:
                    {                    
                    _ts[ev.text_position-1] = (ev.audio_position | TAG_SENTENCE); // set the timestamp for the first letter of the sentence
                    //Serial.printf("[SENTENCE] at %d ms at pos %d len %d\n", ev.audio_position, ev.text_position - 1, ev.length);
                    break;
                    }
                case espeakEVENT_END:
                    {                    
                    _ts[ev.text_position-1] = (ev.audio_position | TAG_END); // set the timestamp for the first letter of the sentence
                    //Serial.printf("[END] at %d ms at pos %d len %d\n", ev.audio_position, ev.text_position - 1, ev.length);
                    break;
                    }
                default: 
                    break; // do nothing
                }
            }
        }
    return 0; 
    }




FLASHMEM int EspeakNG::timestamps(const char* text, int* letter_timestamps_ms)
    {
    _checkinit();
    _abort_now = 0;
    if (!text || !(*text)) return 0; // nothing to play    

    _ts = letter_timestamps_ms;
    _nbsamples = 0;

    const int L = strlen(text); // number of char* in text.     
    int l = strlen_utf8(text); // number of letters: may be less than L when using utf8 encoding...
    if (l < 0) l = L; // using iso-8859 encoding, so l = L. 

    if (!_ts) return l; // no timestamps requested, so we return the number of characters in the text.

    // set timestamps as undefined (-1)
    for (int i = 0; i < l; i++) { _ts[i] = -1; }

    // get the raw timestamps for words/sentence/end.
    espeak_SetSynthCallback(EspeakNG::_letter_cb_static); // set the espeak cb. 
    espeak_Synth(text, strlen(text) + 1, 0, POS_CHARACTER, 0, ((l==L) ? espeakCHARS_8BIT : espeakCHARS_UTF8) | espeakSSML, nullptr, nullptr); // perform synthesis
    const int tot_ms = (_nbsamples * 100) / 2205 + 1; // total time in ms for the audio (22050Hz sample rate)
    
    
    

    // we have the timestamps, so we can compute the letter timings with linear interpolation and additinal tweaks for sentence/end.      
    _ts[0] = 0; // first letter is always at 0ms
    if (_ts[l - 1] < 0) _ts[l - 1] = tot_ms; // last letter is at the end of the audio
    // we first deal with the SSML tags.     
    int i = 0; 
    int c = 0; 
    int codepoint;
    while (i < l-1)
        {
        if ((_ts[i] < 0) && (text[c] == '<'))
            {
            int r = i + 1; 
            while (_ts[r] < 0) { r++; }
            _ts[i] = (_ts[r] & TAG_MASK);
            }
        i++; 
        c += ((l == L) ? 1 : get_utf8(&(text[c]), codepoint));
        }

    //for (int i = 0; i < l; i++) { Serial.println(_ts[i]); } // DEBUG

    // interpolate
    int j = 0;
    while (j < l - 1)
        {
        int k = j + 1;
        while (_ts[k] < 0) { k++; } // find the next defined timestamp
        int type = 0;
        int r = k;
        while ((r < l) && ((_ts[r] & TAG_MASK) == (_ts[k] & TAG_MASK))) { type |= _ts[r++]; } // combine tags of letter with same timestamp.
        _ts[j] &= TAG_MASK; // remove the tags at letter j
        _ts[k] &= TAG_MASK; // remove the tags at letter k
        if (_ts[k] < _ts[j]) { _ts[k] = _ts[j]; }
        if (j + 1 < k)
            { // ok, we interpole from j to k
            int dur = _ts[k] - _ts[j]; // duration 
            if (type & TAG_SENTENCE)
                { // adjust duration for sentence.
                if (dur > TAG_SENTENCE_MS) dur -= TAG_SENTENCE_MS; else dur /= 2;
                } else if (type & TAG_END)
                    { // adjust duration for end. 
                    if (dur > TAG_PAUSE_MS) dur -= TAG_PAUSE_MS; else dur /= 2;
                    }
                dur /= (k - j); // duration per letter
                int t = _ts[j];
                while (j < k) { _ts[j++] = t; t += dur; }
            }
        j = k;
        }

    _ts = nullptr;
    return l; 
    }


FLASHMEM int EspeakNG::duration_ms(const char* text)
    {
    _checkinit();
    _abort_now = 0;
    if (!text || !(*text)) return 0; // nothing to play    

    _ts = nullptr;
    _nbsamples = 0;

    int l = strlen_utf8(text); // number of letters: may be less than L when using utf8 encoding...

    espeak_SetSynthCallback(EspeakNG::_letter_cb_static); // set the espeak cb. 
    espeak_Synth(text, strlen(text) + 1, 0, POS_CHARACTER, 0, ((l > 0) ? espeakCHARS_UTF8 : espeakCHARS_8BIT) | espeakSSML, nullptr, nullptr); // perform synthesis
    const int tot_ms = (_nbsamples * 100) / 2205 + 1; // total time in ms for the audio (22050Hz sample rate), add one for safety. 

    return tot_ms;
    }






FLASHMEM int EspeakNG::_synthbuffer_cb(short* wav, int numsamples, espeak_EVENT* events)
    {
    if ((!wav) || (numsamples <= 0)) return 0; 
    const int nbw = ((_nbsamples + numsamples) > _read_audiobuf) ? (_read_audiobuf - _nbsamples) : numsamples; // number of samples to write in the audio buffer        
    if (nbw > 0) { memcpy(_audiobuffer + _nbsamples, wav, nbw * sizeof(int16_t)); } // copy the audio data to the audio buffer    
    _nbsamples += numsamples;
    return 0;
    }


FLASHMEM int EspeakNG::synthesize(const char* text, int16_t* audiobuffer, int buffer_samples_size)
    {
    _checkinit();
    _abort_now = 0;
    if (!text || !*text) return 0; // nothing to play

    _isutf8 = (strlen_utf8(text) >= 0); // check if the text is a valid utf8 string

    _nbsamples = 0;
    _audiobuffer = audiobuffer; // set the audio buffer
    _read_audiobuf = buffer_samples_size;

    espeak_SetSynthCallback(EspeakNG::_synthbuffer_cb_static);
    espeak_Synth(text, strlen(text) + 1, 0, POS_CHARACTER, 0, (_isutf8 ? espeakCHARS_UTF8 : espeakCHARS_8BIT) | espeakSSML, nullptr, nullptr); // perform synthesis

    _audiobuffer = nullptr;
    _read_audiobuf = 0;

    return _nbsamples; // return the number of samples synthetized
    }



FLASHMEM int EspeakNG::synthesize(const char* text, p_synthetize_cb callback, void* userdata)
    {
    _checkinit();
    _abort_now = 0;
    if (!text || !*text) return 0; // nothing to play

    _isutf8 = (strlen_utf8(text) >= 0); // check if the text is a valid utf8 string
    
    _nbsamples = 0; 
    _usercallback = callback; // set the user callback
    _userdata = userdata; // set the user data

    espeak_SetSynthCallback(EspeakNG::_synthcallback_cb_static); 
    espeak_Synth(text, strlen(text) + 1, 0, POS_CHARACTER, 0, (_isutf8 ? espeakCHARS_UTF8 : espeakCHARS_8BIT) | espeakSSML, nullptr, nullptr); // perform synthesis

    _usercallback = nullptr; // reset the user callback
    _userdata = nullptr; // reset the user data

    return _nbsamples; // return the number of samples synthetized
    }



FLASHMEM int EspeakNG::setRate(int rate)
    {
    _checkinit();
    return (int)espeak_SetParameter(espeakRATE, rate, 0);
    }



FLASHMEM int EspeakNG::setVolume(int volume)
    {
    _checkinit();
    return (int)espeak_SetParameter(espeakVOLUME, volume, 0);
    }



FLASHMEM int EspeakNG::setPitch(int base_pitch)
    {
    _checkinit();
    return (int)espeak_SetParameter(espeakPITCH, base_pitch, 0);
    }


FLASHMEM int EspeakNG::setRange(int pitch_range)
    {
    _checkinit();
    return (int)espeak_SetParameter(espeakRANGE, pitch_range, 0);
    }


FLASHMEM int EspeakNG::setPunctuation(bool announcePunctuation)
    {    
    _checkinit();
    return (int)espeak_SetParameter(espeakPUNCTUATION, (announcePunctuation ? 1: 0), 0);
    }


FLASHMEM int EspeakNG::setCapitals(int method, int pitch_raise)
    {
    _checkinit();
    if (pitch_raise < 0) pitch_raise = 0;
    if (method < 0) method = 0;
    return (int)espeak_SetParameter(espeakCAPITALS, ((method < 3) ? method : pitch_raise), 0);
    }


FLASHMEM int EspeakNG::setWordGap(int ms)
    {
    _checkinit();
    return (int)espeak_SetParameter(espeakWORDGAP, ms, 0);
    }


FLASHMEM int EspeakNG::getMemoryLocation()
    { // may be called before begin()
    return espeak_GetMemoryLocation();
    }



FLASHMEM int EspeakNG::memoryCurrent()
    { // may be called before begin()
    return espeak_memoryCurrent();
    }


FLASHMEM int EspeakNG::memoryMax()
    { // may be called before begin()
    return espeak_memoryMax();
    }



FLASHMEM void EspeakNG::update()
    { // send whatever we have to the audio output provided that _audiobuffer exists and there is something to send.
    if ((!_initdone)||(!_audiobuffer)) return; // just in case... to prevent a possible 'static initialisation order fiasco'
    const int written = _written_audiobuf;   // atomic (and run in interrupt anyway)
    int read = _read_audiobuf;               // 
    if (read >= written) return; // nothing to stream
    // ok, we have something so we stream it.
    audio_block_t * block = allocate();
    if (block == NULL) return; // cannot allocate block, too bad... 

    int pos = (read % AUDIOBUFFER_SIZE); // current position in _audiobuffer (@22050Hz)    
    for (int k = 0; k < AUDIO_BLOCK_SAMPLES/2; k++) // PJRC can break this function by changing AUDIO_BLOCK_SAMPLES to an odd value... :-p
        {
        const int16_t a = ((written > read) ? _audiobuffer[pos] : 0);
        block->data[2*k] = a;
        read++;
        pos++; 
        if (pos >= AUDIOBUFFER_SIZE) pos = 0;
        const int16_t b = ((written > read) ? _audiobuffer[pos] : 0);
        block->data[2*k + 1] = (a+b)/2;
        }
    _read_audiobuf = (read > written) ? written : read; // update the read position
    transmit(block); // send the block to the output
    release(block);  // and then release it. 
    
    if ((_outputletter)&&(_ts)&&(_text))
        { // we should output the letters in sync with the audio        
        while ((_ts_pos < _ts_pos_max) && ((int)_em >= _ts[_ts_pos]))
            {
            int codepoint;
            const int l = ((_isutf8) ? get_utf8(&_text[_ts_char], codepoint) : 1);

            if (_text[_ts_char] == '<') _insidessml = true;            
            if (!_insidessml) _outputletter->write(_text + _ts_char, l);
            if ((_text[_ts_char] == '>') && (_insidessml)) _insidessml = false;

            _ts_char += l;
            _ts_pos++;
            }            
        }                
    return; 
    }


// called by espeak when there is audio sample available (or an event)
FLASHMEM int EspeakNG::_play_cb(short* wav, int numsamples, espeak_EVENT* events)
    { 
    if(_abort_now) { _abort_now = 0;  return 1;} // abort synthesis right away. 
    if (!_audiobuffer) return 0; // no audio buffer, update() disabled and nothing to do
    if (numsamples > 0)
        { // ok, we have something to copy to the buffer
        elapsedMillis lem = 0; 
        int written = _written_audiobuf; // copy the number of samples written in the buffer
        int pos = written % AUDIOBUFFER_SIZE; // current position in the buffer
        const int nbs = numsamples; // record the initial number of samples

        while ((numsamples > 0) && (lem < TIMEOUT_PLAY_MS))
            { 
            if(_abort_now) { _abort_now = 0;  return 1;} // abort synthesis right away. 
            const int read = _read_audiobuf; // atomic operation.
            int sp = AUDIOBUFFER_SIZE - (written - read); // free space in the buffer
            while((numsamples > 0) && (sp > 0))
                {
                _audiobuffer[pos] = wav[0];
                pos++;
                if (pos >= AUDIOBUFFER_SIZE) pos = 0;
                wav++;
                numsamples--;
                sp--;
                written++;
                }
            if (numsamples > 0) { _delay_fun(1); } // wait for space in the buffer
            }       
        if (numsamples == nbs) return 1; // not a single sample was written. Audio is probably not running so we abort synthesis.             
        _written_audiobuf = written; // update the number of samples written in the buffer. atomic operation.
        }

    return 0; 
    }


FLASHMEM void EspeakNG::sendAbortSignal()
    {
    _abort_now = 1;
    }






/** end of file */

