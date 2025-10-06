#pragma once




typedef struct 
    {
    const char * name; // name of the file
    void* buff; // pointer to the buffer
    size_t len; // length of the buffer
    size_t pos; // current position in the buffer
    void * _prev; // previous file in the list
    void * _next; // next file in the list
    } MY_FILE;


/**
* Add a file in the virtual file system
* Return false if file system full.
**/
MY_FILE * espeak_virt_fs_addfile(const char* file, void* buff, size_t len);


/**
* Get a file from the virtual file system
* Return a pointer to the file descriptor or nullptr if not found
**/
MY_FILE* espeak_virt_fs_get(const char* file);


/**
* Remove a file, given by its handle
**/
void espeak_virt_fs_remove(MY_FILE* f);


/**
* Remove a file, given by its name
**/
void espeak_virt_fs_removefile(const char* filename);


/**
* Current number of files in the virtual file system
**/
size_t espeak_virt_fs_filecount();


/**
* Print information about the virtual file system
**/
void espeak_virt_fs_infos();


/** return the first file in the filesystem */
MY_FILE* espeak_virt_fs_get_first();


/** return the next file in the filesystem */
MY_FILE* espeak_virt_fs_get_next(MY_FILE* hfile);




/**
* Add the required file for espeak (and english as it is also required for translations). 
**/
void espeak_virt_fs_add_required_file();