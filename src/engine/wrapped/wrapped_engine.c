#include <SDL2/SDL.h>
#include <luajit-2.0/lua.h>
#include <luajit-2.0/lualib.h>
#include <luajit-2.0/lauxlib.h>
#include "wrapped_engine.h"
#include "../engine.h"
#include "../../logger.h"

int wrapped_EngineFreeSurface(lua_State *state) {
    SDL_Surface *surface = (SDL_Surface*)lua_touserdata(state, 1);
    SDL_FreeSurface(surface);
    log_Debug("Freed surface");
    return 0;
}

int wrapped_EngineGetScene(lua_State *state) {
    lua_pushnumber(state, (lua_Number)engine_GetScene());
    return 1;
}

int wrapped_EngineSetScene(lua_State *state) {
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
