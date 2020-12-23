#include <stdio.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include <luajit-2.0/lua.h>
#include <luajit-2.0/lualib.h>
#include <luajit-2.0/lauxlib.h>
#include "title_screen.h"
#include "script.h"
#include "../logger.h"

void update_Layout() {
    lua_State *state = script_GetState();
    lua_getglobal(state, "title_layout");

    // Check if exists
    if (lua_istable(state, -1) == 0) {
        log_Error("Cant find title_layout");
        lua_pop(state, 1);
        return;
    }

    lua_pushnil(state);
    while (lua_next(state, 1) != 0) {
        lua_getfield(state, -1, "type");
        const char *s = lua_tostring(state, -1);
        lua_pop(state, 1);
        log_Debug("type: %s", s);
        lua_pop(state, 1);
    }

    lua_pop(state, 1);
}

void titleScreen_Render() {

}

int titleScreen_Init() {
    update_Layout();
    return 0;
}

void titleScreen_Free() {

}
