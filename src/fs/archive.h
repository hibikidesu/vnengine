#ifndef VNENGINE_FS_ARCHIVE
#define VNENGINE_FS_ARCHIVE

#include <stdint.h>

#ifndef ARCHIVE_MAX_PATH_LENGTH
#define ARCHIVE_MAX_PATH_LENGTH 256
#endif
#define ARCHIVE_MAGIC "baka"

typedef enum {
    none        = 0x01,
    compressed  = 0x02,
    encrypted   = 0x04
} ArchiveFlags;

typedef struct {
    char path[ARCHIVE_MAX_PATH_LENGTH];
    uint64_t size;
    char *contents;
} ArchiveFile;
typedef struct {
    char magic[5];
    char flags;
    uint64_t file_count;
    ArchiveFile **files;
} Archive;

Archive *archive_Read(char *archivePath, ArchiveFlags flags);
void archive_Create(char *path, uint64_t fileCount, char *files[], ArchiveFlags flags);

#endif