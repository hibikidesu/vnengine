#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "archive.h"

Archive *archive_Read(char *archivePath, ArchiveFlags flags) {
    Archive *archive = NULL;
    FILE *file = NULL;
    uint64_t length = 0;
    char readMagic[5];

    archive = malloc(sizeof(Archive));

    // Read archive
    file = fopen(archivePath, "rb");
    if (file) {
        // Check length
        fseek(file, 0, SEEK_END);
        length = ftell(file);
        if (length < sizeof(Archive)) {
            fprintf(stderr, "Archive too small\n");
            free(archive);
            fclose(file);
            return NULL;
        }

        // Back to beginning of file
        fseek(file, 0, SEEK_SET);

        // Read magic
        fread(readMagic, 1, 5, file);
        if (strncmp(readMagic, ARCHIVE_MAGIC, 5) != 0) {
            fprintf(stderr, "Archive magic does not match, found \"%s\"\n", readMagic);
            free(archive);
            fclose(file);
            return NULL;
        }
        printf("match\n");

        fclose(file);

    } else {
        fprintf(stderr, "Failed to open archive\n");
        free(archive);
        fclose(file);
        return NULL;
    }

    return archive;
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
        uint64_t ptr = sizeof(ARCHIVE_MAGIC) + sizeof(flags) + sizeof(fileCount) + (fileCount * sizeof(uint64_t));
        for (i = 0; i < fileCount; i++) {
            fwrite(&ptr, sizeof(uint64_t), 1, file);
        }

        fclose(file);
    } else {
        fprintf(stderr, "Failed to open file\n");
        fclose(file);
    }
}
