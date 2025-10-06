#pragma once

/* config-espk.h.  Generated from config-espk.h.in by configure.  */
/* config-espk.h.in.  Generated from configure.ac by autoheader.  */

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 0

/* Define to 1 if you have the `dup2' function. */
#define HAVE_DUP2 0

/* Define to 1 if you have the <endian.h> header file. */
#define HAVE_ENDIAN_H 0

/* Define to 1 if you have the <fcntl.h> header file. */
#define HAVE_FCNTL_H 0

/* Define to 1 if you have the `fork' function. */
#define HAVE_FORK 0

/* Define to 1 if you have the <getopt.h> header file. */
#define HAVE_GETOPT_H 0

/* Define to 1 if you have the `getopt_long' function. */
#define HAVE_GETOPT_LONG 0

/* Define to 1 if you have the `gettimeofday' function. */
#define HAVE_GETTIMEOFDAY 0

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the `sonic' library (-lsonic). */
/* #undef HAVE_LIBSONIC */

/* Define to 1 if you have the <locale.h> header file. */
#define HAVE_LOCALE_H 0

/* Define to 1 if you have the `malloc' function. */
#define HAVE_MALLOC 1

/* Define to 1 if you have the `memchr' function. */
#define HAVE_MEMCHR 1

/* Define to 1 if you have the `memmove' function. */
#define HAVE_MEMMOVE 1

/* Define to 1 if you have the `memset' function. */
#define HAVE_MEMSET 1

/* Define to 1 if you have the `mkdir' function. */
#define HAVE_MKDIR 1

/* Define to 1 if you have the `mkstemp' function. */
#define HAVE_MKSTEMP 0

/* Define to 1 if you have the <pcaudiolib/audio.h> header file. */
#define HAVE_PCAUDIOLIB_AUDIO_H 0

#undef HAVE_PCAUDIOLIB_AUDIO_H


/* Define to 1 if you have the `pow' function. */
#define HAVE_POW 1

/* Define to 1 if you have the `realloc' function. */
#define HAVE_REALLOC 1

/* Define to 1 if you have the `setlocale' function. */
#define HAVE_SETLOCALE 1

/* Define to 1 if you have the <sonic.h> header file. */
/* #undef HAVE_SONIC_H */

/* Define to 1 if you have the `sqrt' function. */
#define HAVE_SQRT 1

/* Define to 1 if you have the <stdbool.h> header file. */
#define HAVE_STDBOOL_H 1

/* Define to 1 if you have the <stddef.h> header file. */
#define HAVE_STDDEF_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdio.h> header file. */
#define HAVE_STDIO_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the `strchr' function. */
#define HAVE_STRCHR 1

/* Define to 1 if you have the `strcoll' function and it is properly defined.
   */
#define HAVE_STRCOLL 0

/* Define to 1 if you have the `strdup' function. */
#define HAVE_STRDUP 1

/* Define to 1 if you have the `strerror' function. */
#define HAVE_STRERROR 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the `strrchr' function. */
#define HAVE_STRRCHR 1

/* Define to 1 if you have the `strstr' function. */
#define HAVE_STRSTR 1

/* Define to 1 if you have the <sys/endian.h> header file. */
/* #undef HAVE_SYS_ENDIAN_H */

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 0

/* Define to 1 if you have the <sys/time.h> header file. */
#define HAVE_SYS_TIME_H 0

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 0

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 0

/* Define to 1 if you have the <valgrind/memcheck.h> header file. */
/* #undef HAVE_VALGRIND_MEMCHECK_H */

/* Define to 1 if you have the `vfork' function. */
#define HAVE_VFORK 0

/* Define to 1 if you have the <vfork.h> header file. */
/* #undef HAVE_VFORK_H */

/* Define to 1 if you have the <wchar.h> header file. */
#define HAVE_WCHAR_H 1

/* Define to 1 if you have the <wctype.h> header file. */
#define HAVE_WCTYPE_H 0

/* Define to 1 if `fork' works. */
#define HAVE_WORKING_FORK 0

/* Define to 1 if `vfork' works. */
#define HAVE_WORKING_VFORK 0

/* Define to the sub-directory where libtool stores uninstalled libraries. */
#define LT_OBJDIR ".libs/"

/* Name of package */
#define PACKAGE "espeak-ng"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "https://github.com/espeak-ng/espeak-ng/issues"

/* Define to the full name of this package. */
#define PACKAGE_NAME "eSpeak NG"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "eSpeak NG 1.52-dev"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "espeak-ng"

/* Define to the home page for this package. */
#define PACKAGE_URL "https://github.com/espeak-ng/espeak-ng"

/* Define to the version of this package. */
#define PACKAGE_VERSION "1.52-dev"

/* Define to 1 if all of the C90 standard headers exist (not just the ones
   required in a freestanding environment). This macro is provided for
   backward compatibility; new code need not use it. */
#define STDC_HEADERS 1

/* Version number of package */
#define VERSION "1.52-dev"

/* Define for Solaris 2.5.1 so the uint32_t typedef from <sys/synch.h>,
   <pthread.h>, or <semaphore.h> is not used. If the typedef were allowed, the
   #define below would cause a syntax error. */
/* #undef _UINT32_T */

/* Define for Solaris 2.5.1 so the uint64_t typedef from <sys/synch.h>,
   <pthread.h>, or <semaphore.h> is not used. If the typedef were allowed, the
   #define below would cause a syntax error. */
/* #undef _UINT64_T */

/* Define as a signed integer type capable of holding a process identifier. */
/* #undef pid_t */

/* Define to `unsigned int' if <sys/types.h> does not define. */
/* #undef size_t */

/* Define to `int' if <sys/types.h> does not define. */
/* #undef ssize_t */

/* Define to the type of an unsigned integer type of width exactly 16 bits if
   such a type exists and the standard includes do not define it. */
/* #undef uint16_t */

/* Define to the type of an unsigned integer type of width exactly 32 bits if
   such a type exists and the standard includes do not define it. */
/* #undef uint32_t */

/* Define to the type of an unsigned integer type of width exactly 64 bits if
   such a type exists and the standard includes do not define it. */
/* #undef uint64_t */

/* Define as `fork' if `vfork' does not work. */
/* #undef vfork */

// Makes the Arduino ESpeak C++ class available
#if !defined(USE_CPP_API) && defined(ARDUINO) 
#define USE_CPP_API 1
#endif

// Defines the default data path
#ifndef PATH_ESPEAK_DATA
#  define PATH_ESPEAK_DATA  "/mem/data"
#endif

// Currently we support only little endian systems
#if BYTE_ORDER == LITTLE_ENDIAN
#  define le16toh(X) X
#  define le32toh(X) X
#endif


// Activate big stack allocations moved to heap
#ifndef ESPEAK_STACK_HACK
#  define ESPEAK_STACK_HACK 1
#endif

// Activate big heap allocations to be dynamic
#ifndef ESPEAK_HEAP_HACK
#  define ESPEAK_HEAP_HACK 0
#endif


#define ESPEAK_HEAP_HACK2


// Activate some logging
#ifndef ESPEAK_LOGGING
#  define ESPEAK_LOGGING 1
#endif

#if ESPEAK_STACK_HACK
#  define STACK_T static
#  include <assert.h>
#else 
#  define STACK_T 
#endif


#undef B0