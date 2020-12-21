#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <luajit-2.0/lua.h>
#include <luajit-2.0/lualib.h>
#include <luajit-2.0/lauxlib.h>
#include "wrapped_image.h"
#include "../../logger.h"
#include "../renderer.h"

int wrapped_ImageLoad(lua_State *state) {
    const char *file = luaL_checkstring(state, 1);
    SDL_Texture *texture = IMG_LoadTexture(renderer_GetRenderer(), file);
    if (texture == NULL) {
        log_Error("Failed to open file %s", file);
        lua_pushlightuserdata(state, NULL);
    } else {
        lua_pushlightuserdata(state, texture);   
    }
    return 1;
}

int wrapped_ImageFree(lua_State *state) {
    SDL_Texture *texture = (SDL_Texture*)lua_touserdata(state, 1);
    SDL_DestroyTexture(texture);
    return 0;
}
