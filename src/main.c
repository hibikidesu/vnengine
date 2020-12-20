#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "engine/engine.h"
#include "fs/fs.h"
#include "fs/archive.h"
#include "logger.h"

EngineConfig *createConfig(char *title, int width, int height, char *scriptPath, char *scriptDir) {
    EngineConfig *config = malloc(sizeof(EngineConfig));
    strcpy(config->title, title);
    config -> width = width;
    config -> height = height;
    config->scriptDir = scriptDir;

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
    // Create config
    EngineConfig *config = createConfig("VNEngine", 1280, 720, "data/main.lua", "./data/?.lua");
    if (config == NULL) {
        return 1;
    }
    log_DebugHex("Engine Config", config, sizeof(EngineConfig));

    // Init engine
    engine_Init(config);

    // Run loop
    engine_Run();

    // Cleanup
    engine_Free();

    free(config->script);
    free(config);
    return 0;
}
