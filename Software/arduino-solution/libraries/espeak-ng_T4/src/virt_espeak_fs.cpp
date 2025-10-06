#include <Arduino.h> 

#include "virt_espeak_fs.h"
#include "myalloc.h"

// required
#include "espeak-ng-data/intonations.h"
#include "espeak-ng-data/phondata.h"
#include "espeak-ng-data/phonindex.h"
#include "espeak-ng-data/phontab.h"

// english is required as default language
#include "espeak-ng-data/default_lang.h"
#include "espeak-ng-data/default_dict.h"


#define DEBUG_FS 0

#if DEBUG_FS
#define DEBUG(X) Serial.print(X)
#define DEBUGLN(X) Serial.println(X)
#else
#define DEBUG(x) 
#define DEBUGLN(x) 
#endif



static MY_FILE* _first_file = nullptr; 
static size_t  _virt_fs_count = 0;


FLASHMEM MY_FILE* espeak_virt_fs_get_first()
    {
    return _first_file;
    }


FLASHMEM MY_FILE* espeak_virt_fs_get_next(MY_FILE * hfile)
    {
    return ((hfile) ? (MY_FILE*)hfile->_next : NULL);
    }



FLASHMEM MY_FILE* espeak_virt_fs_addfile(const char* file, void* buff, size_t len)
    {
    if (file == nullptr)
        {
        DEBUGLN("\nespeak_virt_fs_addfile(): **** filename is null ****\n");
        return nullptr;
        }
    if (buff == nullptr)
        {
        DEBUGLN("espeak_virt_fs_addfile(): buff is null");
        }
    if (len == 0)
        {
        DEBUGLN("espeak_virt_fs_addfile(): len is 0");
        }
    DEBUG("espeak_virt_fs_addfile(): adding file [");
    DEBUG(file);
    DEBUG("] [len: ");
    DEBUG(len);
    DEBUG("] [buffer: ");
    DEBUG((int)buff);
    DEBUGLN("]");

    MY_FILE* _oldf = _first_file;
    while (_oldf != nullptr)
        {
        if (strcmp(_oldf->name, file) == 0)
            {
            DEBUG("espeak_virt_fs_addfile(): file [");
            DEBUG(file);
            DEBUGLN("] already exists, *** REPLACING IT ***");
            _oldf->buff = buff;
            _oldf->len = len;
            _oldf->pos = 0;
            return _oldf;
            }
        _oldf = (MY_FILE*)_oldf->_next;
        }

    const char * str = (const char*)my_malloc(strlen(file)+1);
    MY_FILE * f = (MY_FILE*)my_malloc(sizeof(MY_FILE));
    if ((f == nullptr)||(str == nullptr))
        {
        DEBUGLN("\nespeak_virt_fs_addfile(): **** malloc failed **** \n");
        return nullptr;
        }
    _virt_fs_count++;
    f->name = str;
    strcpy((char*)f->name, file);
    f->buff = buff;
    f->len = len;
    f->pos = 0;
    f->_prev = nullptr;
    f->_next = _first_file;
    if (_first_file != nullptr) { _first_file->_prev = f; }
    _first_file = f;
    return f;
    }


FLASHMEM MY_FILE* espeak_virt_fs_get(const char* file)
    {
    if (file == nullptr)
        {
        DEBUGLN("\nespeak_virt_fs_get(): **** file is null ****\n");
        return nullptr;
        }
    MY_FILE* f = _first_file;
    while (f != nullptr)
        {
        if (strcmp(f->name, file) == 0)
            {
            return f;
            }
        f = (MY_FILE*)f->_next;
        }
    DEBUG("espeak_virt_fs_get(): file [");
    DEBUG(file);
    DEBUGLN("] not found !");
    return nullptr;
    }


FLASHMEM void espeak_virt_fs_remove(MY_FILE* f)
    {
    if (f == nullptr)
        {
        DEBUGLN("\nespeak_virt_fs_remove(): **** file handle is null ****\n");
        return;
        }
    if (f->_prev != nullptr) { ((MY_FILE*)f->_prev)->_next = f->_next; }
    if (f->_next != nullptr) { ((MY_FILE*)f->_next)->_prev = f->_prev; }
    if (f == _first_file) { _first_file = (MY_FILE*)f->_next; }
    my_free((void*)(f->name));
    my_free(f);
    _virt_fs_count--;
    }


FLASHMEM void espeak_virt_fs_removefile(const char* filename)
    {
    if (filename == nullptr)
        {
        DEBUGLN("\nespeak_virt_fs_removefile(): **** filename is null ****\n");
        return;
        }
    MY_FILE* f = espeak_virt_fs_get(filename);
    if (f == nullptr)
        {
        DEBUG("espeak_virt_fs_removefile(): file [");
        DEBUG(filename);
        DEBUGLN("] not found !");
        return;
        }
    espeak_virt_fs_remove(f);
    }


FLASHMEM size_t espeak_virt_fs_filecount()
    {
    return (_virt_fs_count);
    }


FLASHMEM void espeak_virt_fs_infos()
    {
    Serial.print("espeak_virt_fs_infos(): number of files ");
    Serial.print(_virt_fs_count);
    Serial.println("\n");
    int n = 0; 
    MY_FILE* p = _first_file;
    while (p != nullptr)
        {
        Serial.printf("- (%d) name [%s] size [%d] addr [%d]\n", ++n, p->name, p->len, (int)p->buff);
        p = (MY_FILE*)p->_next;
        }
    }


FLASHMEM void espeak_virt_fs_add_required_file()
    {
    if (espeak_virt_fs_get("/intonations") == nullptr) espeak_virt_fs_addfile("/intonations", (void*)espeak_ng_data_intonations, espeak_ng_data_intonations_len);
    if (espeak_virt_fs_get("/phontab") == nullptr) espeak_virt_fs_addfile("/phontab", (void*)espeak_ng_data_phontab, espeak_ng_data_phontab_len);
    if (espeak_virt_fs_get("/phonindex") == nullptr) espeak_virt_fs_addfile("/phonindex", (void*)espeak_ng_data_phonindex, espeak_ng_data_phonindex_len);
    if (espeak_virt_fs_get("/phondata") == nullptr) espeak_virt_fs_addfile("/phondata", (void*)espeak_ng_data_phondata, espeak_ng_data_phondata_len);
    if (espeak_virt_fs_get("/en_dict") == nullptr) espeak_virt_fs_addfile("/en_dict", (void*)espeak_ng_data_default_en_dict, espeak_ng_data_default_en_dict_len);
    if (espeak_virt_fs_get("/voices/en") == nullptr) espeak_virt_fs_addfile("/voices/en", (void*)espeak_ng_data_lang_default_en, espeak_ng_data_lang_default_en_len);
//    if (espeak_virt_fs_get("/voices/default") == nullptr) espeak_virt_fs_addfile("/voices/default", (void*)espeak_ng_data_lang_default_en, espeak_ng_data_lang_default_en_len);
//    if (espeak_virt_fs_get("/lang/en") == nullptr) espeak_virt_fs_addfile("/lang/en", (void*)espeak_ng_data_lang_default_en, espeak_ng_data_lang_default_en_len);
    }