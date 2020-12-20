#include <stdio.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include "script.h"
#include "../logger.h"

static lua_State *g_State = NULL;

int script_CallFunction(const char *functionName) {
    // Get the function from global
    lua_getglobal(g_State, functionName);
    
    // Run the function
    if (lua_pcall(g_State, 0, 0, 0)) {
        log_Error("%s failed %s, was expecting function %s to be present", __FUNCTION__, lua_tostring(g_State, -1), functionName);
        return 1;
    }
    return 0;
}

void script_SetGlobalPath(const char *path) {
    lua_getglobal(g_State, "package");
    lua_pushstring(g_State, path);
    lua_setfield(g_State, -2, "path");
    lua_pop(g_State, 1);
}

int script_Init(const char *script, const char *path) {
    int status = 0;

    // Create state
    g_State = luaL_newstate();
    if (g_State == NULL) {
        log_Error("Failed to create lua state");
        return 1;
    }

    // Load libs
    luaL_openlibs(g_State);

    // Load string
    status = luaL_loadstring(g_State, script);

    script_SetGlobalPath(path);

    // Run script
    if (lua_pcall(g_State, 0, 0, 0)) {
        log_Error("Failed to run main script: %s", lua_tostring(g_State, -1));
        return status;
    }

    return 0;
}

void script_Free() {
    lua_close(g_State);
}
