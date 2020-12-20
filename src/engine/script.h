#ifndef ENGINE_SCRIPT
#define ENGINE_SCRIPT

#include "engine.h"

int script_CallFunction(const char *functionName);
void script_SetMouseUp();
void script_HandleMouseDown(int x, int y);
int script_Init(EngineConfig *config);
void script_Free();

#endif