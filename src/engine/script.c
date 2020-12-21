#include <stdio.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include <luajit-2.0/lua.h>
#include <luajit-2.0/lualib.h>
#include <luajit-2.0/lauxlib.h>
#include "script.h"
#include "engine.h"
#include "renderer.h"
#include "../logger.h"

// Wrapped functions
#include "wrapped/wrapped_engine.h"
#include "wrapped/wrapped_image.h"
#include "wrapped/wrapped_render.h"

#define REGISTER_WRAP(name, func) \
    lua_pushstring(g_State, name);    \
    lua_pushcfunction(g_State, func);   \
    lua_settable(g_State, -3);

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
    lua_getglobal(g_State, "controls");
    lua_getfield(g_State, -1, "mouse");

    // Push bool into stack and set as down
    lua_pushboolean(g_State, 0);
    lua_setfield(g_State, -2, "down");

    // Pop tables from stack
    lua_pop(g_State, 2);
}

void script_HandleMouseDown(int x, int y) {
    log_Debug("MOUSE DOWN X:%d Y:%d", x, y);

    // Get global
    lua_getglobal(g_State, "controls");
    lua_getfield(g_State, -1, "mouse");

    // Push x to stack and set
    lua_pushinteger(g_State, (lua_Integer)x);
    lua_setfield(g_State, -2, "x");

    // For y
    lua_pushinteger(g_State, (lua_Integer)y);
    lua_setfield(g_State, -2, "y");

    // Set down bool
    lua_pushboolean(g_State, 1);
    lua_setfield(g_State, -2, "down");

    // Pop controls table and mouse table from stack
    lua_pop(g_State, 2);
}

void script_SetGlobalPath(const char *path) {
    lua_getglobal(g_State, "package");
    lua_pushstring(g_State, path);
    lua_setfield(g_State, -2, "path");
    lua_pop(g_State, 1);
}

void script_WrapEngine() {
    lua_createtable(g_State, 0, 3);
    REGISTER_WRAP("freeSurface", wrapped_EngineFreeSurface);
    REGISTER_WRAP("getScene", wrapped_EngineGetScene);
    REGISTER_WRAP("setScene", wrapped_EngineSetScene);
    lua_setglobal(g_State, "engine");
}

void script_WrapImage() {
    lua_createtable(g_State, 0, 1);
    REGISTER_WRAP("load", wrapped_ImageLoad);
    lua_setglobal(g_State, "image");
}

void script_WrapRender() {
    lua_createtable(g_State, 0, 6);
    REGISTER_WRAP("showSurface", wrapped_RendererShowSurface);
    REGISTER_WRAP("drawLine", wrapped_RenderDrawLine);
    REGISTER_WRAP("drawFillRect", wrapped_RenderDrawFillRect);
    REGISTER_WRAP("drawRect", wrapped_RenderDrawRect);
    REGISTER_WRAP("setDrawColor", wrapped_RenderSetDrawColor);
    REGISTER_WRAP("clear", wrapped_RenderClear);
    lua_setglobal(g_State, "render");
}

void script_WrapControls() {
    // Create controls table
    lua_createtable(g_State, 0, 1);
    
    // Mouse table
    lua_pushstring(g_State, "mouse");
    lua_createtable(g_State, 0, 3);

    lua_pushstring(g_State, "down");
    lua_pushboolean(g_State, 0);
    lua_settable(g_State, -3);

    lua_pushstring(g_State, "x");
    lua_pushnumber(g_State, (lua_Number)0);
    lua_settable(g_State, -3);

    lua_pushstring(g_State, "y");
    lua_pushnumber(g_State, (lua_Number)0);
    lua_settable(g_State, -3);

    lua_settable(g_State, -3);
    lua_setglobal(g_State, "controls");
}

void script_LoadWrapped() {
    script_WrapEngine();
    script_WrapImage();
    script_WrapRender();
    script_WrapControls();
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

    // Load wrappers
    script_LoadWrapped();

    //
    // GAME TABLE
    //
    lua_createtable(g_State, 0, 1);

    // Create window table, 2 items, PUSH WINDOW STRING BEFORE TABLE TO STACK
    // [stack] <game table>, ["window", <window table>], <extra functions>
    lua_pushstring(g_State, "window");
    lua_createtable(g_State, 0, 2);

    lua_pushstring(g_State, "width");
    lua_pushnumber(g_State, (lua_Number)config->width);
    lua_settable(g_State, -3);

    lua_pushstring(g_State, "height");
    lua_pushnumber(g_State, (lua_Number)config->height);
    lua_settable(g_State, -3);

    // Append table to game table
    lua_settable(g_State, -3);

    // Set as global
    lua_setglobal(g_State, "game");

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
