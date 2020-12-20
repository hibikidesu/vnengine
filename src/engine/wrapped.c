#include <SDL2/SDL.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include "wrapped.h"
#include "renderer.h"
#include "engine.h"

int wrapped_SetDrawColor(lua_State *state) {
    uint8_t r = luaL_checknumber(state, 1);
    uint8_t g = luaL_checknumber(state, 2);
    uint8_t b = luaL_checknumber(state, 3);
    uint8_t a = luaL_checknumber(state, 4);
    lua_pushnumber(state, (lua_Number)renderer_SetDrawColor(r, g, b, a));
    return 1;
}

int wrapped_SDLError(lua_State *state) {
    lua_pushstring(state, SDL_GetError());
    return 1;
}

int wrapped_DrawLine(lua_State *state) {
    int x1 = luaL_checknumber(state, 1);
    int y1 = luaL_checknumber(state, 2);
    int x2 = luaL_checknumber(state, 3);
    int y2 = luaL_checknumber(state, 4);
    lua_pushnumber(state, (lua_Number)renderer_DrawLine(x1, y1, x2, y2));
    return 1;
}

int wrapped_DrawFillRect(lua_State *state) {
    int x = luaL_checknumber(state, 1);
    int y = luaL_checknumber(state, 2);
    int w = luaL_checknumber(state, 3);
    int h = luaL_checknumber(state, 4);
    lua_pushnumber(state, (lua_Number)renderer_DrawFillRect(x, y, w, h));
    return 1;
}

int wrapped_DrawRect(lua_State *state) {
    int x = luaL_checknumber(state, 1);
    int y = luaL_checknumber(state, 2);
    int w = luaL_checknumber(state, 3);
    int h = luaL_checknumber(state, 4);
    lua_pushnumber(state, (lua_Number)renderer_DrawRect(x, y, w, h));
    return 1;
}

int wrapped_SetScene(lua_State *state) {
    int value = luaL_checknumber(state, 1);
    if (value < 0) {
        // Fail
        lua_pushboolean(state, 0);
    } else {
        engine_SetScene(value);
        lua_pushboolean(state, 1);
    }
    return 1;
}
