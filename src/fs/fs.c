#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "fs.h"

char *fs_ReadFile(char *filePath, int *status) {
    char *buffer = NULL;
    uint64_t length;
    FILE *file = fopen(filePath, "r");

    if (file) {
        // Check length
        fseek(file, 0, SEEK_END);
        length = ftell(file);

        // Back to beginning of file
        fseek(file, 0, SEEK_SET);
        buffer = malloc(length);
        if (buffer) {
            fread(buffer, 1, length, file);
        }
        fclose(file);
        *status = 0;
    } else {
        *status = 1;
    }

    return buffer;
}
