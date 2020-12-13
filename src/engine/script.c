#include <stdio.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include "script.h"

static lua_State *g_State = NULL;

int script_Init() {
    // Create state
    g_State = luaL_newstate();
    if (g_State == NULL) {
        fprintf(stderr, "Failed to create lua state\n");
        return 1;
    }

    // Load libs
    luaL_openlibs(g_State);
    return 0;
}

void script_Free() {

}
