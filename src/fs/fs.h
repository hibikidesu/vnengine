#ifndef VNENGINE_FS
#define VNENGINE_FS

#include <stdint.h>

uint64_t fs_ReadFile(char *filePath, char **contents);

#endif