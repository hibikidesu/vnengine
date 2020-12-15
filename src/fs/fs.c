#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "fs.h"

uint64_t fs_ReadFile(char *filePath, char **contents) {
    uint64_t length = 0;
    FILE *file = fopen(filePath, "r");

    if (file) {
        // Check length
        fseek(file, 0, SEEK_END);
        length = ftell(file);

        // Back to beginning of file
        fseek(file, 0, SEEK_SET);
        *contents = malloc(sizeof(char) * length);
        if (*contents != NULL) {
            fread(*contents, 1, length, file);
        }
        fclose(file);
    }

    return length;
}
