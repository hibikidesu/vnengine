#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "archive.h"

Archive *archive_Read(char *archivePath, ArchiveFlags flags) {
    Archive *archive = NULL;
    FILE *file = NULL;
    uint64_t length = 0;
    char readMagic[4];

    archive = malloc(sizeof(Archive));

    // Read archive
    file = fopen(archivePath, "rb");
    if (file) {
        // Check length
        fseek(file, 0, SEEK_END);
        length = ftell(file);
        if (length < sizeof(Archive)) {
            fprintf(stderr, "Archive too small\n");
            fclose(file);
            return NULL;
        }

        // Back to beginning of file
        fseek(file, 0, SEEK_SET);

        // Read magic
        fread(readMagic, 1, 4, file);
        if (strncmp(readMagic, "baka", 4) != 0) {
            fprintf(stderr, "Archive magic does not match, found \"%s\"\n", readMagic);
            fclose(file);
            return NULL;
        }
        printf("match\n");

        fclose(file);

    } else {
        fprintf(stderr, "Failed to open archive\n");
        fclose(file);
        return NULL;
    }

    return archive;
}
