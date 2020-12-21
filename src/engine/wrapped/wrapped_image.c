#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <luajit-2.0/lua.h>
#include <luajit-2.0/lualib.h>
#include <luajit-2.0/lauxlib.h>
#include "wrapped_image.h"
#include "../../logger.h"

int wrapped_ImageLoad(lua_State *state) {
    const char *file = luaL_checkstring(state, 1);
    SDL_Surface *surface = IMG_Load(file);
    if (surface == NULL) {
        log_Error("Failed to open file %s", file);
        lua_pushlightuserdata(state, NULL);
    } else {
        lua_pushlightuserdata(state, surface);   
    }
    return 1;
}
