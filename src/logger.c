#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include "logger.h"

void log_Hex(char *name, const void *data, size_t size) {
    char ascii[17];
    size_t i;
    size_t read = 0;
    ascii[16] = '\0';
    printf("%s:\n", name);

    while (read < size) {
        // Print offset
        printf("    %08lX: ", read);

        // Read 16 bytes
        for (i = 0; i < 16; i++) {
            if (read < size) {
                printf("%02X ", ((unsigned char*)data)[read]);
                // ascii check
                if (((unsigned char*)data)[read] >= ' ' && ((unsigned char*)data)[read] <= '~') {
                    ascii[i] = ((unsigned char*)data)[read];
                } else {
                    ascii[i] = '.';
                }
            } else {
                printf("   ");
            }
            read++;
        }

        // Print ascii
        printf("%s\n", ascii);
    }
    printf("\n");
}

void log_Log(FILE *file, const char *level, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    // Make level str
    char *newFmt = NULL;
    char *lvlBuffer = (char*)malloc((size_t)strlen(level) + 3 + 1); // [LEVEL]<SPACE>\0
    sprintf(lvlBuffer, "[%s] ", level);

    newFmt = malloc((size_t)strlen(lvlBuffer) + (size_t)strlen(fmt) + 2);
    strcpy(newFmt, lvlBuffer);
    strcat(newFmt, fmt);
    strcat(newFmt, "\n");
    free(lvlBuffer);

    // Print
    vfprintf(file, newFmt, args);
    va_end(args);
    free(newFmt);
}
