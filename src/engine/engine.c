#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "engine.h"
#include "renderer.h"
#include "script.h"
#include "../logger.h"
#include "title_screen.h"

static bool running = false;
static SDL_Event event;
static int scene = 0;

int engine_GetScene() {
    return scene;
}

void engine_SetScene(int newscene) {
    scene = newscene;
}

void engine_RenderFrame() {
    if (running == false) {
        return;
    }
    
    switch (engine_GetScene()) {
        case SCENE_TITLE:
            titleScreen_Render();
            break;
        default:
            break;
    }
    
    renderer_Present();
    script_SetMouseUp();
}

void engine_ReadEvents() {
    int x, y;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            // Mouse button down event
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    SDL_GetMouseState(&x, &y);
                    script_HandleMouseDown(x, y);
                }
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

    if ((status = IMG_Init(IMG_INIT_PNG)) == 0) {
        log_Error("Failed to SDL_image, %s", SDL_GetError());
        return status;
    }

    // Init script
    if ((status = script_Init(config))) {
        return status;
    }

    if ((status = titleScreen_Init())) {
        log_Error("Failed to init title");
        return status;
    }

    return status;
}

void engine_Free() {
    log_Info("Engine shutting down");
    script_CallFunction("shutdown");
    IMG_Quit();
    script_Free();
    titleScreen_Free();
    renderer_Free();
}
