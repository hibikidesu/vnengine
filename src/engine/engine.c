#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "engine.h"
#include "renderer.h"
#include "script.h"
#include "../logger.h"

static bool running = false;
static SDL_Event event;

void engine_RenderFrame() {
    if (running == false) {
        return;
    }
    script_CallFunction("main_loop");
    renderer_Present();
}

void engine_ReadEvents() {
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            default:
                break;
        }
    }
}

void engine_Run() {
    running = true;
    while (running) {
        engine_ReadEvents();
        engine_RenderFrame();
    }
}

int engine_Init(EngineConfig *config) {
    int status = 0;

    // Init renderer
    if ((status = renderer_Init(config->title, config->width, config->height))) {
        return status;
    }

    // Init script
    if ((status = script_Init(config))) {
        return status;
    }

    return status;
}

void engine_Free() {
    log_Info("Engine shutting down");
    script_Free();
    renderer_Free();
}
