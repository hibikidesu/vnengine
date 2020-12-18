#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include "logger.h"

void log_Log(FILE *file, const char *level, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    // Make level str
    char *newFmt = NULL;
    char *lvlBuffer = (char*)malloc((size_t)strlen(level) + 3 + 1); // [LEVEL]<SPACE>\0
    sprintf(lvlBuffer, "[%s]", level);

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
