#ifndef ENGINE_WRAPPED
#define ENGINE_WRAPPED

#include <luajit-2.0/lua.h>

int wrapped_ShowSurface(lua_State *state);
int wrapped_IMGLoad(lua_State *state);
int wrapped_FreeSurface(lua_State *state);
int wrapped_RenderClear(lua_State *state);
int wrapped_SetDrawColor(lua_State *state);
int wrapped_SDLError(lua_State *state);
int wrapped_DrawLine(lua_State *state);
int wrapped_DrawFillRect(lua_State *state);
int wrapped_DrawRect(lua_State *state);
int wrapped_GetScene(lua_State *state);
int wrapped_SetScene(lua_State *state);

#endif