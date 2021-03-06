#ifndef VNENGINE_FS_ARCHIVE
#define VNENGINE_FS_ARCHIVE

#include <stdint.h>

#ifndef ARCHIVE_MAX_PATH_LENGTH
#define ARCHIVE_MAX_PATH_LENGTH 256
#endif
#define ARCHIVE_MAGIC "baka"

typedef enum {
    none,
    compressed,
    encrypted
} ArchiveFlags;

typedef struct {
    uint64_t size;
    char path[ARCHIVE_MAX_PATH_LENGTH];
    char *contents;
} ArchiveFile;
typedef struct {
    uint64_t file_count;
    ArchiveFile **files;
} Archive;

Archive *archive_Read(char *archivePath);
void archive_Create(char *path, uint64_t fileCount, ArchiveFile **files, ArchiveFlags flags);
void archive_Free(Archive *archive);

#endif