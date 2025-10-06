#include "config-espk.h"

#include "speak_lib.h"
#include "myalloc.h"
#include "mystdio.h"

#include <Arduino.h>

FLASHMEM void espeak_SetMemoryLocation(int alloc_in_EXTMEM)
    {
    set_my_alloc_location(alloc_in_EXTMEM);
    }


FLASHMEM int espeak_GetMemoryLocation()
    {
    return get_my_alloc_location();
    }


int espeak_memoryCurrent()
    {
    return my_alloc_memory_use();  
    }


int espeak_memoryMax()
    {
    return my_alloc_memory_max();
    }



FLASHMEM int espeak_RegisterDict(const char* language_name, const unsigned char* dict_data, int dict_data_len)
    {
    char name[60] = "/";
    if ((language_name == NULL) || (strlen(language_name) > 40)) return -1;
    if ((dict_data == NULL) || (dict_data_len <= 0)) return -2;
    strcpy(name + 1, language_name);
    strcpy(name + strlen(name), "_dict");
    MY_FILE* f = espeak_virt_fs_addfile(name, (void*)dict_data, dict_data_len);
    return ((f == NULL) ? -3 : EE_OK);
    }


FLASHMEM int espeak_RegisterLang(const char* voice_name, const unsigned char* voice_data, int voice_data_len)
    {
    char name[60] = "/voices/";
    if ((voice_name == NULL) || (strlen(voice_name) > 40)) return -1;
    if ((voice_data == NULL) || (voice_data_len <= 0)) return -2;
    strcpy(name + 8, voice_name);
    MY_FILE* f = espeak_virt_fs_addfile(name, (void*)voice_data, voice_data_len);
    return ((f == NULL) ? -3 : EE_OK);
    }


FLASHMEM int espeak_RegisterVoiceVariant(const char* variant_name, const unsigned char* variant_data, int variant_data_len)
    {
    char name[60] = "/voices/!v/";
    if ((variant_name == NULL) || (strlen(variant_name) > 40)) return -1;
    if ((variant_data == NULL) || (variant_data_len <= 0)) return -2;
    strcpy(name + 11, variant_name);
    MY_FILE* f = espeak_virt_fs_addfile(name, (void*)variant_data, variant_data_len);
    return ((f == NULL) ? -3 : EE_OK);
    }











/** end of file */
