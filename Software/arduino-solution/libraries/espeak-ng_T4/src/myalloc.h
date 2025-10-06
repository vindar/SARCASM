#pragma once
#include "config-espk.h"



#ifdef __cplusplus
extern "C" {
#endif


#ifdef ESPEAK_HEAP_HACK2
void initheaphack2();
void freeheaphack2();
#endif

/** Print info about the allocated memory */
void allocInfo();


/** Return the current memory allocation in bytes */
int my_alloc_memory_use();


/** Return the maximum memory allocation in bytes */
int my_alloc_memory_max();


/** Set where memory allocation should go (0 = DMAMEM, 1 = EXTMEM). Can only be change when no object are currently allocated */
void set_my_alloc_location(int use_extmem); // 

/* Return the current allocation location*/
int get_my_alloc_location();

void* my_aligned_alloc(size_t alignment, size_t memorySize); // defined but not implemented on purpose

/** calloc() */
void* my_calloc(size_t elementCount, size_t elementSize);

/** malloc() */
void* my_malloc(size_t memorySize);

/** realloc() */
void* my_realloc(void* pointer, size_t memorySize);

/** free() */
void my_free(void* pointer);


#ifdef __cplusplus
    }
#endif


