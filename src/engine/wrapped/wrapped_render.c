#include <SDL2/SDL.h>
#include <luajit-2.0/lua.h>
#include <luajit-2.0/lualib.h>
#include <luajit-2.0/lauxlib.h>
#include "wrapped_render.h"
#include "../renderer.h"

int wrapped_RendererShowSurface(lua_State *state) {
    SDL_Surface *surface = (SDL_Surface*)lua_touserdata(state, 1);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer_GetRenderer(), surface);
    if (texture == NULL) {
        lua_pushstring(state, "Failed to create surface from texture");
        lua_error(state);
    }
    SDL_RenderCopy(renderer_GetRenderer(), texture, NULL, NULL);
    SDL_DestroyTexture(texture);
    return 0;
}

int wrapped_RenderDrawLine(lua_State *state) {
    int x1 = luaL_checknumber(state, 1);
    int y1 = luaL_checknumber(state, 2);
    int x2 = luaL_checknumber(state, 3);
    int y2 = luaL_checknumber(state, 4);
    lua_pushnumber(state, (lua_Number)SDL_RenderDrawLine(renderer_GetRenderer(), x1, y1, x2, y2));
    return 1;
}

int wrapped_RenderDrawFillRect(lua_State *state) {
    int x = luaL_checknumber(state, 1);
    int y = luaL_checknumber(state, 2);
    int w = luaL_checknumber(state, 3);
    int h = luaL_checknumber(state, 4);
    lua_pushnumber(state, (lua_Number)SDL_RenderFillRect(renderer_GetRenderer(), &(SDL_Rect){x, y, w, h}));
    return 1;
}

int wrapped_RenderDrawRect(lua_State *state) {
    int x = luaL_checknumber(state, 1);
    int y = luaL_checknumber(state, 2);
    int w = luaL_checknumber(state, 3);
    int h = luaL_checknumber(state, 4);
    lua_pushnumber(state, (lua_Number)SDL_RenderDrawRect(renderer_GetRenderer(), &(SDL_Rect){x, y, w, h}));
    return 1;
}

int wrapped_RenderSetDrawColor(lua_State *state) {
    uint8_t r = luaL_checknumber(state, 1);
    uint8_t g = luaL_checknumber(state, 2);
    uint8_t b = luaL_checknumber(state, 3);
    uint8_t a = luaL_checknumber(state, 4);
    lua_pushnumber(state, (lua_Number)SDL_SetRenderDrawColor(renderer_GetRenderer(), r, g, b, a));
    return 1;
}

int wrapped_RenderClear(lua_State *state) {
    lua_pushnumber(state, (lua_Number)SDL_RenderClear(renderer_GetRenderer()));
    return 1;
}
