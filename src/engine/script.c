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
        return 1;
    }
    return 0;
}

void script_SetMouseUp() {
    // Get global table
    lua_getglobal(g_State, "mouse");

    // Push bool into stack and set as down
    lua_pushboolean(g_State, 0);
    lua_setfield(g_State, -2, "down");

    // Pop table from stack
    lua_pop(g_State, 1);
}

void script_HandleMouseDown(int x, int y) {
    log_Debug("X:%d Y:%d", x, y);

    // Get global
    lua_getglobal(g_State, "mouse");

    // Push x to stack and set
    lua_pushinteger(g_State, (lua_Integer)x);
    lua_setfield(g_State, -2, "x");

    // For y
    lua_pushinteger(g_State, (lua_Integer)y);
    lua_setfield(g_State, -2, "y");

    // Set down bool
    lua_pushboolean(g_State, 1);
    lua_setfield(g_State, -2, "down");

    // Pop mouse table from stack
    lua_pop(g_State, 1);
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

    // Game table, 1 item
    lua_createtable(g_State, 0, 1);

    // Create window table, 2 items, PUSH WINDOW STRING BEFORE TABLE TO STACK
    lua_pushstring(g_State, "window");
    lua_createtable(g_State, 0, 2);

    lua_pushstring(g_State, "width");
    lua_pushnumber(g_State, (lua_Number)config->width);
    lua_settable(g_State, -3);

    lua_pushstring(g_State, "height");
    lua_pushnumber(g_State, (lua_Number)config->height);
    lua_settable(g_State, -3);

    // Create game table
    lua_settable(g_State, -3);
    lua_setglobal(g_State, "game");

    // Create mouse event table
    lua_createtable(g_State, 0, 3);
    
    lua_pushstring(g_State, "down");
    lua_pushboolean(g_State, 0);
    lua_settable(g_State, -3);

    lua_pushstring(g_State, "x");
    lua_pushinteger(g_State, 0);
    lua_settable(g_State, -3);

    lua_pushstring(g_State, "y");
    lua_pushinteger(g_State, 0);
    lua_settable(g_State, -3);

    lua_setglobal(g_State, "mouse");

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
