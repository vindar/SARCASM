
#include "mystdio.h"
#include "virt_espeak_fs.h"

#include <Arduino.h>



#define DEBUG_STDIO 0

MY_FILE* my_stderr = nullptr;
MY_FILE* my_stdout = nullptr;
MY_FILE* my_stdin = nullptr;





FLASHMEM void mydebug_wstr(const wchar_t * s)
    {
#if DEBUG_STDIO    
    if (s)
        {
        while(s[0])
            {
            Serial.write((char)(*s++));
            }
        }
    else  Serial.print("NULL");
#endif
    }



FLASHMEM void mydebug_str(const char* s)
    {
#if DEBUG_STDIO    
    if (s) Serial.print(s); else   Serial.print("NULL");    
#endif
    }


FLASHMEM void mydebug_int(int n)
    {
#if DEBUG_STDIO
    Serial.print(n);
#endif
    }



FLASHMEM static void fileInfo(MY_FILE * f)
    {
#if DEBUG_STDIO
    if (f == nullptr)
        {
        Serial.println("fileInfo: FILE DOES NOT EXIST !");
        return;
        }
    Serial.printf("[%s] pos: %d/%d\n", f->name, f->pos, f->len);
#endif
    }



FLASHMEM static void waitme()
    {
#if DEBUG_STDIO
    while (Serial.read() <= 0) { delay(10); }
#endif
    }





FLASHMEM void* espeak_mem_map(const char* path, int* len)
    {
    mydebug_str("espeak_mem_map : ");
    auto f = espeak_virt_fs_get(path);
    fileInfo(f);
    if (f == nullptr) { return nullptr; }
    if (len != nullptr)
        {
        *len = f->len;
        }
    if (f->buff == nullptr)
        {
        mydebug_str("espeak_mem_map : buff is null");
        return nullptr;
        }
    return f->buff;            
    }



FLASHMEM int my_getfilelength(const char* filename)
    {
    mydebug_str("my_getfilelength : ");
    auto f = espeak_virt_fs_get(filename);
    fileInfo(f); 
    if (f == nullptr) return -1;
    return f->len;
    }


PROGMEM static const char mytempname[5] = "temp";

FLASHMEM const char* my_tmpnam(char* buffer)
    {
    mydebug_str("*** my_tmpnam() called ***");
    return mytempname;
    }


FLASHMEM int my_fclose(MY_FILE* f)
    {
    mydebug_str("my_fclose : ");
    fileInfo(f);
    return 0;
    }


FLASHMEM MY_FILE* my_fopen(const char* filename, const char* mode)
    {
    mydebug_str("my_fopen ");
    auto f = espeak_virt_fs_get(filename);
    fileInfo(f);
    if (f == nullptr) { return nullptr; }
    f->pos = 0;
    return f;
    }


FLASHMEM int my_feof(MY_FILE* f)
    {
    mydebug_str("my_feof ");
    fileInfo(f);
    return ((f->len == f->pos) ? 1 : 0);
    }


FLASHMEM int my_fflush(MY_FILE* stream)
    {
    mydebug_str("my_fflush ");
    fileInfo(stream);
    return 0;
    }


FLASHMEM int my_fgetc(MY_FILE* f)
    {
    mydebug_str("my_fgetc");
    fileInfo(f);    
    if (f == nullptr) { return EOF; }
    if ((f->pos >= f->len) || (f->buff == nullptr)) { return EOF; }
    return (int)(((char*)f->buff)[(f->pos)++]);
    }


FLASHMEM char* my_fgets(char* string, int maxLength, MY_FILE* f)
    {
   // mydebug_str("my_fgets read : ");
    char* s = string;
    if (f == nullptr) 
        { 
        mydebug_str(" *** ERROR NULL HANDLE ***\n");
        return nullptr; 
        }
    char* b = (char*)f->buff;
    if (b == nullptr)
        {
        mydebug_str(" *** ERROR NULL BUFFER *** from ");
        fileInfo(f);
        return nullptr;
        }
    if (f->pos >= f->len) 
        {
        mydebug_str(" END OF FILE from ");
        fileInfo(f);
        return nullptr;
        }
    while ((maxLength > 1) && (f->pos < f->len))
        {
        char c = b[(f->pos)++];
        *(s++) = c;
        if ((c == 0) || (c == '\n')) { break; }
        }
    (*s) = 0;

    /*
#if DEBUG_STDIO
    Serial.print("[");
    const int ll = strlen(string);
    if (ll > 0)
        {
        Serial.write(string, ll - ((string[ll-1] == '\n') ? 1 : 0));
        }    
    Serial.print("] from ");
    fileInfo(f);
#endif
*/
    return string;
    }



FLASHMEM int my_fputc(int character, MY_FILE* stream)
    {
    mydebug_str("my_fputc [");
    mydebug_int(character);
    mydebug_str("] *** NOT IMPLEMENTED 1!!! ***\n\n");
    waitme();
    return 0;
    }


FLASHMEM size_t my_fread(void* buffer, size_t blocSize, size_t blocCount, MY_FILE* stream)
    {
    mydebug_str("my_fread ");
    auto f = espeak_virt_fs_get(stream->name);
    fileInfo(f);
    if (f == nullptr) { return 0; }
    size_t totlen = blocSize * blocCount;
    size_t rem_len = f->len - f->pos;
    if (rem_len <= 0) return 0;
    if (rem_len < totlen) totlen = rem_len;

    memcpy(buffer, ((char*)f->buff) + f->pos, totlen);
    f->pos += totlen;

    mydebug_str("my_fread read : [");
    mydebug_int(totlen);
    mydebug_str("] [new pos : ");
    mydebug_int(f->pos);
    mydebug_str("]");

    return totlen;
    }


FLASHMEM int my_fseek(MY_FILE* stream, long offset, int whence)
    {
    mydebug_str("my_fseek ");
    fileInfo(stream);
    mydebug_str("   *** NOT IMPLEMENTED 2!!! ***\n\n");
    waitme();

    return 0;
    }


FLASHMEM long my_ftell(MY_FILE* stream)
    {
    mydebug_str("my_ftell ");
    fileInfo(stream);
    mydebug_str("   *** NOT IMPLEMENTED 3!!! ***\n\n");
    waitme();

    return 0;
    }


FLASHMEM size_t my_fwrite(void* buffer, size_t blocSize, size_t blocCount, MY_FILE* stream)
    {
    mydebug_str("my_fwrite ");
    fileInfo(stream);
    mydebug_str("   *** NOT IMPLEMENTED 4!!! ***\n\n");
    waitme();

    return 0;
    }


FLASHMEM int my_fprintf(MY_FILE* stream, const char* format, ...)
    {
    mydebug_str("my_fprintf ");
    fileInfo(stream);
    mydebug_str("string : [");
    mydebug_str(format);

    mydebug_str("]\n   *** NOT IMPLEMENTED 5!!! ***\n\n");
    waitme();

    return 0;
    }

FLASHMEM int my_vfprintf(MY_FILE* stream, const char* format, va_list arg)
    {
    mydebug_str("my_vfprintf ");
    fileInfo(stream);
    mydebug_str("   *** NOT IMPLEMENTED 6!!! ***\n\n");
    waitme();

    return 0;
    }


FLASHMEM int my_remove(const char* fileName)
    {
    mydebug_str("my_remove ");
    auto f = espeak_virt_fs_get(fileName);
    fileInfo(f);
    mydebug_str("   *** NOT IMPLEMENTED 7!!! ***\n\n");
    waitme();

    return 0;
    }


FLASHMEM void my_rewind(MY_FILE* stream)
    {
    mydebug_str("my_rewind ");
    fileInfo(stream);
    mydebug_str("   *** NOT IMPLEMENTED 8!!! ***\n\n");
    waitme();

    return;
    }


FLASHMEM int my_ungetc(int charact, MY_FILE* stream)
    {
    mydebug_str("my_ungetc ");
    fileInfo(stream);
    mydebug_str("   *** NOT IMPLEMENTED 9!!! ***\n\n");
    waitme();

    return 0;
    }



FLASHMEM int init_mystdio_FS()
    {
    espeak_virt_fs_add_required_file();    
    return 0;
    }


FLASHMEM MY_FILE* my_getFirstFile()
    {
    return espeak_virt_fs_get_first();
    }


FLASHMEM MY_FILE* my_getNextFile(MY_FILE* f)
    {
    return espeak_virt_fs_get_next(f);
    }


FLASHMEM const char* my_getFileName(MY_FILE* stream)
    {
    return ((stream) ? stream->name : NULL);
    }






FLASHMEM void stdio_debug_wstr(const wchar_t* s)
    {
    mydebug_wstr(s);
    }


FLASHMEM void stdio_debug_str(const char* str)
    {
    mydebug_str(str);
    }

FLASHMEM void stdio_debug_int(int n)
    {
    mydebug_int(n);
    }

FLASHMEM void stdio_debug_pause()
    {
#if DEBUG_STDIO
    while(Serial.read() > 0) { delay(10); }
    Serial.println("Press any key to continue...");
    while (Serial.read() <= 0) { delay(10); }
#endif
    }
