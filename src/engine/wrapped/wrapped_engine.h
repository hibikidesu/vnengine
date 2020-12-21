#ifndef ENGINE_WRAPPED_ENGINE
#define ENGINE_WRAPPED_ENGINE

#include <luajit-2.0/lua.h>

int wrapped_EngineFreeSurface(lua_State *state);;
int wrapped_EngineGetScene(lua_State *state);
int wrapped_EngineSetScene(lua_State *state);

#endif