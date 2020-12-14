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

void archive_Create(char *path, int fileCount, char *files[], ArchiveFlags flags) {
    FILE *file = NULL;
    Archive *archive = malloc(sizeof(Archive));
    strncpy(archive->magic, ARCHIVE_MAGIC, 5);
    archive -> flags = flags;
    archive -> file_count = fileCount;

    // Open file
    file = fopen(path, "wb");
    if (file) {
        fwrite(archive, sizeof(archive), 1, file);
        fclose(file);
    } else {
        fprintf(stderr, "Failed to open file\n");
        fclose(file);
    }
    
    free(archive);
}
