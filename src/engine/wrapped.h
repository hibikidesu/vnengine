#ifndef ENGINE_WRAPPED
#define ENGINE_WRAPPED

#include <lua.h>

int wrapped_SetDrawColor(lua_State *state);
int wrapped_SDLError(lua_State *state);
int wrapped_DrawFillRect(lua_State *state);
int wrapped_DrawRect(lua_State *state);
int wrapped_SetScene(lua_State *state);

#endif