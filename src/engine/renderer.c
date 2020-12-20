#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "renderer.h"
#include "../logger.h"

static SDL_Window *g_Window = NULL;
static SDL_Renderer *g_Renderer = NULL;

int renderer_SetDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    return SDL_SetRenderDrawColor(g_Renderer, r, g, b, a);
}

int renderer_DrawLine(int x1, int y1, int x2, int y2) {
    return SDL_RenderDrawLine(g_Renderer, x1, y1, x2, y2);
}

int renderer_DrawFillRect(int x, int y, int w, int h) {
    return SDL_RenderFillRect(g_Renderer, &(SDL_Rect){x, y, w, h});
}

int renderer_DrawRect(int x, int y, int w, int h) {
    return SDL_RenderDrawRect(g_Renderer, &(SDL_Rect){x, y, w, h});
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
