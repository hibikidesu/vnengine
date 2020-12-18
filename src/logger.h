#ifndef VNENGINE_LOGGER
#define VNENGINE_LOGGER

void log_Log(FILE *file, const char *level, const char *fmt, ...);

#define log_Info(fmt, ...) log_Log(stdout, "INFO", fmt, ##__VA_ARGS__)
#define log_Warn(fmt, ...) log_Log(stdout, "WARN", fmt, ##__VA_ARGS__)
#define log_Error(fmt, ...) log_Log(stderr, "ERROR", fmt, ##__VA_ARGS__)
#if DEBUG == 1
#define log_Debug(fmt, ...) log_Log(stdout, "DEBUG", fmt, ##__VA_ARGS__)
#else
#define log_Debug(fmt, ...)
#endif

#endif