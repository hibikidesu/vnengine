#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "archive.h"

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
    }
}
