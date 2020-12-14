#ifndef VNENGINE_ENGINE
#define VNENGINE_ENGINE

typedef struct {
    char title[64];
    int width;
    int height;
    char *script;
} EngineConfig;

int engine_Init(EngineConfig *config);
void engine_Free();

#endif