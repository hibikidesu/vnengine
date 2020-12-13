#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "engine/engine.h"

int main(int argc, char *argv[]) {
    // Create config
    EngineConfig *config = malloc(sizeof(EngineConfig));
    strcpy(config->title, "VNEngine");
    config -> width = 1280;
    config -> height = 720;
    strcpy(config->startScript, "data/main.lua");

    // Init engine
    engine_Init(config);

    // Cleanup
    engine_Free();

    free(config);
    return 0;
}
