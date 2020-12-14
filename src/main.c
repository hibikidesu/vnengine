#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "engine/engine.h"
#include "fs/fs.h"

EngineConfig *createConfig(char *title, int width, int height, char *scriptPath) {
    EngineConfig *config = malloc(sizeof(EngineConfig));
    strcpy(config->title, title);
    config -> width = width;
    config -> height = height;

    // Load script
    int status = 0;
    char *script = fs_ReadFile(scriptPath, &status);
    if (status) {
        fprintf(stderr, "Failed to read main script file\n");
        free(config);
        return NULL;
    }
    config -> script = script;

    return config;
}

int main(int argc, char *argv[]) {
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
