#ifndef ENGINE_RENDERER
#define ENGINE_RENDERER

#include <stdint.h>

int renderer_SetDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
int renderer_DrawLine(int x1, int y1, int x2, int y2);
int renderer_DrawFillRect(int x, int y, int w, int h);
int renderer_DrawRect(int x, int y, int w, int h);
void renderer_Present();
int renderer_Init(char *windowTitle, int windowW, int windowH);
void renderer_Free();

#endif 