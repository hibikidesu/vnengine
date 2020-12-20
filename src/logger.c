#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include "logger.h"
#include "utils/ansi.h"

// Cache for struct coloring
#define LOG_STRUCT_MAX_SIZE 8
static size_t log_Struct_Size = 0;
static size_t log_Struct[LOG_STRUCT_MAX_SIZE];

void log_ColorHexStruct(size_t *offsets, size_t size) {
    int i;
    size_t amount;
    if (size > LOG_STRUCT_MAX_SIZE) {
        amount = LOG_STRUCT_MAX_SIZE;
    } else {
        amount = size;
    }
    for (i = 0; i < amount; i++) {
        memcpy(&log_Struct[i], &offsets[i], sizeof(size_t));
        log_Struct_Size++;
    }
}

void log_Hex(const char *name, const void *data, size_t size) {
    char ascii[17];
    size_t i;
    size_t read = 0;
    size_t structItemLength = 0;
    int currentStructItem = 0;
    printf("%s:\n", name);

    while (read < size) {
        // Print offset
        printf("    %08lX: ", read);

        // Clear ascii buffer
        memset(ascii, '\0', 17);

        // Read 16 bytes
        for (i = 0; i < 16; i++) {
            if (read < size) {
                // COLOR CHECK
                // Check if going over current struct item length read
                if (structItemLength >= log_Struct[currentStructItem] && currentStructItem < 9) {
                    currentStructItem++;
                    structItemLength = 0;
                }

                printf("\x1B[%dm\x1B[%dm%02X %s", 30 + currentStructItem, 47 - currentStructItem, ((unsigned char*)data)[read], ANSI_RESET);
                // ascii check
                if (((unsigned char*)data)[read] >= ' ' && ((unsigned char*)data)[read] <= '~') {
                    ascii[i] = ((unsigned char*)data)[read];
                } else {
                    ascii[i] = '.';
                }
                structItemLength++;
            } else {
                printf("   ");
            }
            read++;
        }

        // Print ascii
        printf("%s\n", ascii);
    }
    printf("\n");

    // Clear cache
    log_Struct_Size = 0;
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
