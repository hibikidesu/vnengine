#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "renderer.h"
#include "../logger.h"

static SDL_Window *g_Window = NULL;
static SDL_Renderer *g_Renderer = NULL;

SDL_Renderer *renderer_GetRenderer() {
    return g_Renderer;
}

void renderer_Present() {
    SDL_RenderPresent(g_Renderer);
}

int renderer_Init(char *windowTitle, int windowW, int windowH) {
    // Init SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        log_Error("Failed to init SDL %s", SDL_GetError());
        return 1;
    }

    // Init TTF
    if (TTF_Init()) {
        log_Error("Failed to init TTF %s", SDL_GetError());
        return 1;
    }

    // Setup Window
    g_Window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowW, windowH, SDL_WINDOW_SHOWN);
    if (g_Window == NULL) {
        log_Error("Failed to create window %s", SDL_GetError());
        return 1;
    }

    // Setup renderer
    g_Renderer = SDL_CreateRenderer(g_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (g_Renderer == NULL) {
        log_Error("Failed to create renderer %s", SDL_GetError());
        return 1;
    }

    return 0;
}

void renderer_Free() {
    SDL_DestroyRenderer(g_Renderer);
    SDL_DestroyWindow(g_Window);
    g_Renderer = NULL;
    g_Window = NULL;
    TTF_Quit();
    SDL_Quit();
}
