#ifndef ENGINE_WRAPPED_RENDER
#define ENGINE_WRAPPED_RENDER

#include <luajit-2.0/lua.h>

int wrapped_RendererQueryTexture(lua_State *state);
int wrapped_RendererShowTexture(lua_State *state);
int wrapped_RenderDrawLine(lua_State *state);
int wrapped_RenderDrawFillRect(lua_State *state);
int wrapped_RenderDrawRect(lua_State *state);
int wrapped_RenderSetDrawColor(lua_State *state);
int wrapped_RenderClear(lua_State *state);

#endif