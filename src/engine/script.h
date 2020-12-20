#ifndef ENGINE_SCRIPT
#define ENGINE_SCRIPT

#include "engine.h"

int script_CallFunction(const char *functionName);
int script_Init(EngineConfig *config);
void script_Free();

#endif