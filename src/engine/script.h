#ifndef ENGINE_SCRIPT
#define ENGINE_SCRIPT

#include "engine.h"
#include <luajit-2.0/lua.h>

lua_State *script_GetState();
int script_CallFunction(const char *functionName);
void script_DumpStack();
void script_SetMouseUp();
void script_HandleMouseDown(int x, int y);
int script_Init(EngineConfig *config);
void script_Free();

#endif