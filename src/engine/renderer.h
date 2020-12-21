#ifndef ENGINE_RENDERER
#define ENGINE_RENDERER

#include <SDL2/SDL.h>
#include <stdint.h>

SDL_Renderer *renderer_GetRenderer();
void renderer_Present();
int renderer_Init(char *windowTitle, int windowW, int windowH);
void renderer_Free();

#endif 