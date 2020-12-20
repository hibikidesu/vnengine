#include <stdio.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include "script.h"
#include "engine.h"
#include "renderer.h"
#include "../logger.h"
#include "wrapped.h"

#define REGISTER_WRAP(name, func) \
    lua_pushstring(g_State, name);    \
    lua_pushcfunction(g_State, func);   \
    lua_settable(g_State, -3);

static lua_State *g_State = NULL;

void kvIntToStack(const char *key, const int value) {
    lua_pushstring(g_State, key);
    lua_pushnumber(g_State, (lua_Number)value);
}

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

    //
    // GAME TABLE
    //
    lua_createtable(g_State, 0, 2);

    // Create window table, 2 items, PUSH WINDOW STRING BEFORE TABLE TO STACK
    // [stack] <game table>, ["window", <window table>], <extra functions>
    lua_pushstring(g_State, "window");
    lua_createtable(g_State, 0, 2);

    kvIntToStack("width", config->width);
    lua_settable(g_State, -3);

    kvIntToStack("height", config->height);
    lua_settable(g_State, -3);

    // Append table to game table
    lua_settable(g_State, -3);

    REGISTER_WRAP("setScene", wrapped_SetScene);
    REGISTER_WRAP("drawRect", wrapped_DrawRect);
    REGISTER_WRAP("drawFillRect", wrapped_DrawFillRect);
    REGISTER_WRAP("setDrawLine", wrapped_DrawLine);
    REGISTER_WRAP("SDL_Error", wrapped_SDLError);
    REGISTER_WRAP("setDrawColor", wrapped_SetDrawColor);

    // Set as global
    lua_setglobal(g_State, "game");

    //
    // MOUSE TABLE
    //
    lua_createtable(g_State, 0, 3);
    
    lua_pushstring(g_State, "down");
    lua_pushboolean(g_State, 0);
    lua_settable(g_State, -3);

    kvIntToStack("x", 0);
    lua_settable(g_State, -3);

    kvIntToStack("y", 0);
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
