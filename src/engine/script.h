#ifndef ENGINE_SCRIPT
#define ENGINE_SCRIPT

int script_CallFunction(const char *functionName);
int script_Init(const char *script, const char *path);
void script_Free();

#endif