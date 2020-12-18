#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <zlib.h>
#include "archive.h"

// Avoid corruption on win systems.
#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#  include <fcntl.h>
#  include <io.h>
#  define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
#  define SET_BINARY_MODE(file)
#endif

// 256k
#define ZLIB_CHUNK 262144

void compressFile(char *path) {
    FILE *file;
    int ret, flush;
    unsigned have;
    z_stream strm;
    unsigned char in[ZLIB_CHUNK];
    unsigned char out[ZLIB_CHUNK];

    // Open file
    file = fopen(path, "rb");
    if (file == NULL) {
        fprintf(stderr, "%s: Failed to open file\n", __FUNCTION__);
        fclose(file);
        return;
    }

    // Allocate deflate state
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    ret = deflateInit(&strm, Z_BEST_COMPRESSION);
    if (ret != Z_OK) {
        fprintf(stderr, "Failed to deflateInit\n");
        fclose(file);
        return;
    }

    // Compress until EOF
    //do {
    //    strm.avail_in = fread(in, 1, ZLIB_CHUNK, source);
    //}
}

Archive *archive_Read(char *archivePath, ArchiveFlags flags) {
    return NULL;
}

void archive_Create(char *path, uint64_t fileCount, ArchiveFile **files, ArchiveFlags flags) {
    FILE *file = NULL;

    // Open file
    file = fopen(path, "wb");
    if (file) {
        // Write magic
        fwrite(ARCHIVE_MAGIC, sizeof(ARCHIVE_MAGIC), 1, file);

        // Write flags
        fwrite(&flags, sizeof(flags), 1, file);

        // Write file_count
        fwrite(&fileCount, sizeof(fileCount), 1, file);

        // Write ptrs
        int i;
        uint64_t fileTablePtr;
        uint64_t headerSize =  sizeof(ARCHIVE_MAGIC) + sizeof(flags) + sizeof(fileCount);
        uint64_t ptr = headerSize + ((fileCount * sizeof(uint64_t)) * 2) + (ARCHIVE_MAX_PATH_LENGTH * fileCount);
        for (i = 0; i < fileCount; i++) {
            // FILE TABLE
            // Write down the address of the file in the file table
            fwrite(&ptr, sizeof(uint64_t), 1, file);
            // Save position of current table addr
            fileTablePtr = ftell(file);

            // SIZE TABLE
            fseek(file, headerSize + (sizeof(uint64_t) * fileCount) + (i * sizeof(uint64_t)), SEEK_SET);
            fwrite(&files[i]->size, sizeof(uint64_t), 1, file);

            // PATH TABLE
            fseek(file, headerSize + ((sizeof(uint64_t) * fileCount) * 2) + (i * ARCHIVE_MAX_PATH_LENGTH), SEEK_SET);
            fwrite(&files[i]->path, sizeof(files[i]->path), 1, file);

            // FILE SPACE
            // Write file at addr
            fseek(file, ptr, SEEK_SET);
            fwrite(files[i]->contents, files[i]->size, 1, file);
            // Increase ptr new location by file size
            ptr += files[i] -> size;

            // Go back to ptr table
            fseek(file, fileTablePtr, SEEK_SET);
        }

        fclose(file);
    } else {
        fprintf(stderr, "Failed to open file\n");
        fclose(file);
        return;
    }

    // Read flags
    // Check if compression needed else if compression needed
    switch (flags)
    {
        case compressed:
            compressFile(path);
            break;
        
        case encrypted:
            break;

        default:
            break;
    }
}
