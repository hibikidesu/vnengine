#include <stdio.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include "script.h"
#include "engine.h"
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

int script_Init(EngineConfig *config) {
    int status = 0;

    // Create state
    g_State = luaL_newstate();
    if (g_State == NULL) {
        log_Error("Failed to create lua state");
        return 1;
    }

    // Load libs
    luaL_openlibs(g_State);

    // Load string and set path of scripts
    status = luaL_loadstring(g_State, config->script);
    script_SetGlobalPath(config->scriptDir);

    // Set variables
    lua_createtable(g_State, 0, 2);

    lua_pushstring(g_State, "width");
    lua_pushnumber(g_State, (lua_Number)config->width);
    lua_settable(g_State, -3);

    lua_pushstring(g_State, "height");
    lua_pushnumber(g_State, (lua_Number)config->height);
    lua_settable(g_State, -3);

    // Pop from stack, set as global
    lua_setglobal(g_State, "window");

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
