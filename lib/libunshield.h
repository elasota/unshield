#ifndef __unshield_h__
#define __unshield_h__

#include <stdbool.h>
#include <stddef.h>

#define UNSHIELD_LOG_LEVEL_LOWEST    0

#define UNSHIELD_LOG_LEVEL_ERROR     1
#define UNSHIELD_LOG_LEVEL_WARNING   2
#define UNSHIELD_LOG_LEVEL_TRACE     3

#define UNSHIELD_LOG_LEVEL_HIGHEST   4


#ifdef __cplusplus
extern "C" {
#endif

#if defined(_MSC_VER) && defined(LIBUNSHIELD_DYNAMIC_LIBRARY)
#define UNSHIELD_DLLEXPORT __declspec(dllexport)
#else
#define UNSHIELD_DLLEXPORT
#endif
 
typedef struct _Unshield Unshield;


/*
   Logging
 */

UNSHIELD_DLLEXPORT void unshield_set_log_level(int level);


/*
   Open/close functions
 */

UNSHIELD_DLLEXPORT Unshield* unshield_open(const char* filename);
UNSHIELD_DLLEXPORT Unshield* unshield_open_force_version(const char* filename, int version);
UNSHIELD_DLLEXPORT void unshield_close(Unshield* unshield);

typedef struct
{
    void *(*fopen)(const char *filename, const char *modes, void *userdata);
    int (*fseek)(void *file, long int offset, int whence, void *userdata);
    long int (*ftell)(void *file, void *userdata);
    size_t (*fread)(void *ptr, size_t size, size_t n, void *file, void *userdata);
    size_t (*fwrite)(const void *ptr, size_t size, size_t n, void *file, void *userdata);
    int (*fclose)(void *file, void *userdata);
    void *(*opendir)(const char *name, void *userdata);
    int (*closedir)(void *dir, void *userdata);
    struct dirent* (*readdir)(void *dir, void *userdata);
} UnshieldIoCallbacks;

UNSHIELD_DLLEXPORT Unshield* unshield_open2(const char* filename, const UnshieldIoCallbacks* callbacks, void* userdata);
UNSHIELD_DLLEXPORT Unshield* unshield_open2_force_version(const char* filename, int version, const UnshieldIoCallbacks* callbacks, void* userdata);

/*
   Component functions
 */

typedef struct
{
  const char* name;
  unsigned file_group_count;
  const char** file_group_names;
} UnshieldComponent;

UNSHIELD_DLLEXPORT int         unshield_component_count    (Unshield* unshield);
UNSHIELD_DLLEXPORT const char* unshield_component_name     (Unshield* unshield, int index);

/*
   File group functions
 */

typedef struct
{
  const char* name;
  unsigned first_file;
  unsigned last_file;
} UnshieldFileGroup;

UNSHIELD_DLLEXPORT int                 unshield_file_group_count (Unshield* unshield);
UNSHIELD_DLLEXPORT UnshieldFileGroup*  unshield_file_group_get   (Unshield* unshield, int index);
UNSHIELD_DLLEXPORT UnshieldFileGroup*  unshield_file_group_find  (Unshield* unshield, const char* name);
UNSHIELD_DLLEXPORT const char*         unshield_file_group_name  (Unshield* unshield, int index);

/*
   Directory functions
 */

UNSHIELD_DLLEXPORT int         unshield_directory_count    (Unshield* unshield);
UNSHIELD_DLLEXPORT const char* unshield_directory_name     (Unshield* unshield, int index);

/*
   File functions
 */

UNSHIELD_DLLEXPORT int         unshield_file_count         (Unshield* unshield);
UNSHIELD_DLLEXPORT const char* unshield_file_name          (Unshield* unshield, int index);
UNSHIELD_DLLEXPORT bool        unshield_file_is_valid      (Unshield* unshield, int index);
UNSHIELD_DLLEXPORT bool        unshield_file_save          (Unshield* unshield, int index, const char* filename);
UNSHIELD_DLLEXPORT int         unshield_file_directory     (Unshield* unshield, int index);
UNSHIELD_DLLEXPORT size_t      unshield_file_size          (Unshield* unshield, int index);

/** For investigation of compressed data */
UNSHIELD_DLLEXPORT bool unshield_file_save_raw(Unshield* unshield, int index, const char* filename);

/** Maybe it's just gzip without size? */
UNSHIELD_DLLEXPORT bool unshield_file_save_old(Unshield* unshield, int index, const char* filename);

/** Deobfuscate a buffer. Seed is 0 at file start */
UNSHIELD_DLLEXPORT void unshield_deobfuscate(unsigned char* buffer, size_t size, unsigned* seed);

/** Is the archive Unicode-capable? */
UNSHIELD_DLLEXPORT bool unshield_is_unicode(Unshield* unshield);

#ifdef __cplusplus
}
#endif

  
#endif

