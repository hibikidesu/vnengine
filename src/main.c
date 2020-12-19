#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "engine/engine.h"
#include "fs/fs.h"
#include "fs/archive.h"
#include "logger.h"

EngineConfig *createConfig(char *title, int width, int height, char *scriptPath) {
    EngineConfig *config = malloc(sizeof(EngineConfig));
    strcpy(config->title, title);
    config -> width = width;
    config -> height = height;

    // Load script
    char *buffer = NULL;
    uint64_t length = fs_ReadFile(scriptPath, &buffer);
    if (length == 0) {
        log_Error("Failed to read main script file\n");
        free(config);
        return NULL;
    }
    config -> script = buffer;

    return config;
}

int main(int argc, char *argv[]) {
    ArchiveFile **files = malloc(sizeof(ArchiveFile) * 2);
    files[0] = malloc(sizeof(ArchiveFile));
    files[1] = malloc(sizeof(ArchiveFile));

    // Read file
    char *buf = NULL;
    uint64_t len = fs_ReadFile("Makefile", &buf);

    files[0] -> contents = buf;
    files[0] -> size = len;
    strcpy(files[0]->path, "/Makefile");

    char *x = "baka u";
    files[1] -> contents = malloc(strlen(x) + 1);
    strncpy(files[1]->contents, x, strlen(x) + 1);
    files[1] -> size = strlen(x) + 1;
    strcpy(files[1]->path, "/sex");

    archive_Create("encrypted.dat", 2, files, encrypted);
    free(files[0]->contents);
    free(files[0]);
    free(files[1]->contents);
    free(files[1]);
    free(files);

    files = NULL;
    Archive *archive = archive_Read("encrypted.dat");
    if (archive == NULL) {
        log_Info("Archive is null");
    }
    uint64_t i;
    for (i = 0; i < archive->file_count; i++) {
        free(archive->files[i]->contents);
        free(archive->files[i]);
    }
    free(files);
    free(archive);
    return 0;

    // Create config
    EngineConfig *config = createConfig("VNEngine", 1280, 720, "data/main.lua");
    if (config == NULL) {
        return 1;
    }

    // Init engine
    engine_Init(config);

    // Cleanup
    engine_Free();

    free(config->script);
    free(config);
    return 0;
}
