#ifndef VNENGINE_ENGINE
#define VNENGINE_ENGINE

#define SCENE_TITLE 0
#define SCENE_GAME 1

typedef struct {
    char title[64];
    int width;
    int height;
    char *script;
    char *scriptDir;
} EngineConfig;

int engine_GetScene();
void engine_SetScene(int newscene);
void engine_Run();
int engine_Init(EngineConfig *config);
void engine_Free();

#endif