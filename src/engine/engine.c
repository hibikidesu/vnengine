#include <stdio.h>
#include <SDL2/SDL.h>
#include "engine.h"
#include "renderer.h"
#include "script.h"

int engine_Init(EngineConfig *config) {
    int status = 0;

    // Init renderer
    if ((status = renderer_Init(config->title, config->width, config->height))) {
        return status;
    }

    // Init script
    if ((status = script_Init(config->script))) {
        return status;
    }

    return status;
}

void engine_Free() {
    printf("Engine shutting down\n");
    script_Free();
    renderer_Free();
}
