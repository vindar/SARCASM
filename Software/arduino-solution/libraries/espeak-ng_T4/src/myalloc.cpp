#include <stdlib.h>
#include "myalloc.h"

#include <Arduino.h>


#define DEBUG_ALLOC 0

#if DEBUG_ALLOC
#define DEBUG(X) Serial.print(X)
#define DEBUGLN(X) Serial.println(X)
#else
#define DEBUG(x) 
#define DEBUGLN(x) 
#endif


#ifdef ESPEAK_HEAP_HACK2

#ifdef __cplusplus
extern "C" {
#endif

int my_init_ph_list2(void);
void my_free_ph_list2(void);

int my_init_ssml_param_stack(void);
int my_free_ssml_param_stack(void);

int my_init_soundicon_tab(void);
void my_free_soundicon_tab(void);

int my_init_phoneme_tab_phoneme_tab_list(void);
void my_free_phoneme_tab_phoneme_tab_list(void);

int my_init_wavegen(void);
void my_free_wavegen(void);

int my_init_frame_pool_pttr(void);
int my_free_frame_pool_pttr(void);

int my_init_voice_voice_list(void);
void my_free_voice_voice_list(void);

int my_init_dictionary(void);
void my_free_dictionary(void);


#ifdef __cplusplus
    }
#endif


static int initheaphack2_done = 0;

FLASHMEM void initheaphack2()
    {
    DEBUGLN("--------------------------------");
    DEBUG("initheaphack2()... ");
    noInterrupts();
    if (initheaphack2_done) return;
    initheaphack2_done = 1;
    my_init_dictionary();
    my_init_voice_voice_list();
    my_init_frame_pool_pttr();
    my_init_ph_list2();
    my_init_ssml_param_stack();
    my_init_soundicon_tab();
    my_init_phoneme_tab_phoneme_tab_list();
    my_init_wavegen();
    interrupts();
    DEBUGLN(" done!");
    DEBUGLN("--------------------------------");
    }

FLASHMEM void freeheaphack2()
    {
    DEBUGLN("--- - freeheaphack2() --");
    if (!initheaphack2_done) return;
    noInterrupts();
    my_free_dictionary();
    my_free_voice_voice_list();
    my_free_frame_pool_pttr();
    my_free_ph_list2();
    my_free_ssml_param_stack();
    my_free_soundicon_tab();
    my_free_phoneme_tab_phoneme_tab_list();
    my_free_wavegen();
    initheaphack2_done = 0;
    interrupts();
    DEBUGLN("--- - freeheaphack2() done --");
    }


#endif



extern uint8_t external_psram_size; // detect if external PSRAM is available

static int _my_alloc_use_extmem = 0; // 0 = DMAMEM, 1 = EXTMEM


FLASHMEM void* sub_my_calloc(size_t elementCount, size_t elementSize)
    {
    return (_my_alloc_use_extmem ? extmem_calloc(elementCount, elementSize) : calloc(elementCount, elementSize));
    }

FLASHMEM void* sub_my_malloc(size_t memorySize)
    {
    return (_my_alloc_use_extmem ? extmem_malloc(memorySize) : malloc(memorySize));
    }

FLASHMEM void* sub_my_realloc(void* pointer, size_t memorySize)
    {
    return (_my_alloc_use_extmem ? extmem_realloc(pointer, memorySize) : realloc(pointer, memorySize));
    }

FLASHMEM void sub_my_free(void* pointer)
    {
    if (_my_alloc_use_extmem) { extmem_free(pointer); } else { free(pointer); }
    }



/**
* Simple allocator monitor
**/

struct myAllocInfo
    {
    size_t size;
    void* ptr;
    myAllocInfo* prev;
    myAllocInfo* next;
    };


static myAllocInfo* _start_alloc_list = NULL;
size_t _nb_alloc = 0; 
size_t _nb_alloc_max = 0;
size_t _nb_alloc_size = 0;
size_t _nb_alloc_size_max = 0;


int my_alloc_memory_use()
    {
    return _nb_alloc_size;
    }

int my_alloc_memory_max()
    {
    return _nb_alloc_size_max;
    }



FLASHMEM void allocInfo()
    {
    # ifdef DEBUG_ALLOC
    Serial.println("---------- allocInfo -----------");
    Serial.printf(" - number of objects : %d ", _nb_alloc);
    Serial.printf(" [ maximum : %d ]\n", _nb_alloc_max);
    Serial.printf(" - total size : %d ", _nb_alloc_size);
    Serial.printf(" [ maximum : %d ]\n\n", _nb_alloc_size_max);
    Serial.println("--------------------------------");
    #endif
    }

FLASHMEM void add_alloc_list(void* ptr, size_t size)
    {
    DEBUG("-> ALLOCATION ptr:");
    DEBUG((int)ptr);
    DEBUG(" size:");
    DEBUGLN(size);

    if (ptr == NULL)
        {
        DEBUGLN("***** add_alloc_list(): called with NULL pointer *****");
        DEBUG("requested size :");
        DEBUGLN(size);
        return; 
        }
    myAllocInfo* pinfo = (myAllocInfo*)sub_my_malloc(sizeof(myAllocInfo));
    if (pinfo == NULL)
        {
        Serial.println("***** add_alloc_list(): malloc failed *****");
        return;
        }
    _nb_alloc++;
    _nb_alloc_size += size;
    if (_nb_alloc > _nb_alloc_max) _nb_alloc_max = _nb_alloc;
    if (_nb_alloc_size > _nb_alloc_size_max) _nb_alloc_size_max = _nb_alloc_size;
    pinfo->ptr = ptr;
    pinfo->size = size;
    pinfo->prev = NULL;
    pinfo->next = _start_alloc_list;
    if (_start_alloc_list != NULL) { _start_alloc_list->prev = pinfo; }
    _start_alloc_list = pinfo;
    return;
    }


FLASHMEM bool remove_alloc_list(void* ptr)
    {
    DEBUG("-> FREE ptr:");
    DEBUGLN((int)ptr);

    if (ptr == NULL) return true; // not an error. 
    myAllocInfo* pinfo = _start_alloc_list;
    while (pinfo != NULL)
        {
        if (pinfo->ptr == ptr)
            {
            _nb_alloc--;
            _nb_alloc_size -= pinfo->size;
            if (pinfo->prev != NULL) { pinfo->prev->next = pinfo->next; }
            if (pinfo->next != NULL) { pinfo->next->prev = pinfo->prev; }
            if (pinfo == _start_alloc_list) { _start_alloc_list = pinfo->next; }
            sub_my_free(pinfo);
            return true;
            }
        pinfo = pinfo->next;
        }
    DEBUGLN("\n***** remove_alloc_list(): pointer not found *****\n");
    DEBUG("pointer :");
    DEBUGLN((int)ptr);
    DEBUG("\n\n");
    return false;
    }


FLASHMEM void update_alloc_list(void* ptr, void * new_ptr, size_t new_size)
    {
    if (new_ptr == NULL)
        {
        DEBUGLN("\n***** update_alloc_list(): called with a NULL pointer *****");
        DEBUG("old pointer :");
        DEBUGLN((int)ptr);
        DEBUG("new pointer :");
        DEBUGLN((int)new_ptr);
        DEBUGLN("requested size :");
        DEBUGLN(new_size);
        DEBUGLN("************************************************************\n\n");
        return;
        }
    if (ptr == NULL)
        {        
        add_alloc_list(new_ptr, new_size);
        return;
        }            
    myAllocInfo* pinfo = _start_alloc_list;
    while (pinfo != NULL)
        {
        if (pinfo->ptr == ptr)
            {
            _nb_alloc_size -= pinfo->size;
            _nb_alloc_size += new_size;
            if (_nb_alloc_size > _nb_alloc_size_max) _nb_alloc_size_max = _nb_alloc_size;
            pinfo->ptr = new_ptr;
            pinfo->size = new_size;
            return;
            }
        pinfo = pinfo->next;
        }
    DEBUGLN("\n***** update_alloc_list(): pointer not found *****\n");
    }



FLASHMEM void set_my_alloc_location(int use_extmem)
    {
    if (_nb_alloc > 0) return; // cannot change allocation location if there are already allocated objects
    _my_alloc_use_extmem = ((use_extmem) && (external_psram_size > 0)) ? 1 : 0;
    }


FLASHMEM int get_my_alloc_location()
    {
    return _my_alloc_use_extmem;
    }


FLASHMEM void* my_calloc(size_t elementCount, size_t elementSize)
    {
    void * p = sub_my_calloc(elementCount, elementSize);
    add_alloc_list(p, elementCount * elementSize);    
    return p;
    }

FLASHMEM void* my_malloc(size_t memorySize)
    {
    void* p = sub_my_malloc(memorySize);
    add_alloc_list(p, memorySize);
    return p;
    }

FLASHMEM void* my_realloc(void* pointer, size_t memorySize)
    {
    void * p = sub_my_realloc(pointer, memorySize);
    update_alloc_list(pointer, p, memorySize);
    return p;
    }

FLASHMEM void my_free(void* pointer)
    {    
    if (remove_alloc_list(pointer)) { sub_my_free(pointer); }
    }



