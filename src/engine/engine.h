#ifndef VNENGINE_ENGINE
#define VNENGINE_ENGINE

typedef struct {
    char title[64];
    int width;
    int height;
    char *script;
    char *scriptDir;
} EngineConfig;

void engine_Run();
int engine_Init(EngineConfig *config);
void engine_Free();

#endif