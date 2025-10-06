#ifndef _MYSTDIO_H_
#define _MYSTDIO_H_

#include <stdio.h>

#include "virt_espeak_fs.h"

#ifndef FLASHMEM
#define FLASHMEM __attribute__((section(".flashmem"))) 
#endif

#ifndef PROGMEM
#define PROGMEM __attribute__((section(".progmem")))
#endif


#ifdef __cplusplus
extern "C" {
#endif






    int init_mystdio_FS();


    extern MY_FILE * my_stderr;
    extern MY_FILE * my_stdout;
    extern MY_FILE * my_stdin;

    void stdio_debug_wstr(const wchar_t* s);
    void stdio_debug_str(const char* str);
    void stdio_debug_int(int n);
    void stdio_debug_pause();


    void* espeak_mem_map(const char* path, int* len);


    int my_getfilelength(const char* filename);


    const char* my_tmpnam(char* buffer);


    int my_fclose(MY_FILE* stream);


    MY_FILE* my_fopen(const char* filename, const char* mode);


    int my_feof(MY_FILE* stream);


    int my_fflush(MY_FILE* stream);

    int my_fgetc(MY_FILE* stream);

    char* my_fgets(char* string, int maxLength, MY_FILE* stream);

    int my_fprintf(MY_FILE* stream, const char* format, ...);

    int my_vfprintf(MY_FILE* stream, const char* format, va_list arg);


    int my_fputc(int character, MY_FILE* stream);

    size_t my_fread(void* buffer, size_t blocSize, size_t blocCount, MY_FILE* stream);

    int my_fseek(MY_FILE* stream, long offset, int whence);

    long my_ftell(MY_FILE* stream);

    size_t my_fwrite(void* buffer, size_t blocSize, size_t blocCount, MY_FILE* stream);


    int my_remove(const char* fileName);

    void my_rewind(MY_FILE* stream);



    int my_ungetc(int charact, MY_FILE* stream);


    MY_FILE* my_getFirstFile();

    MY_FILE* my_getNextFile(MY_FILE * f);

    const char* my_getFileName(MY_FILE* stream);


#ifdef __cplusplus
    }
#endif





#endif


