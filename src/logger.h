#ifndef VNENGINE_LOGGER
#define VNENGINE_LOGGER

void log_ColorHexStruct(size_t *offsets, size_t size);
void log_Hex(const char *name, const void *data, size_t size);
void log_Log(FILE *file, const char *level, const char *fmt, ...);

#define log_Info(fmt, ...) log_Log(stdout, "INFO", fmt, ##__VA_ARGS__)
#define log_Warn(fmt, ...) log_Log(stdout, "WARN", fmt, ##__VA_ARGS__)
#define log_Error(fmt, ...) log_Log(stderr, "ERROR", fmt, ##__VA_ARGS__)
#if DEBUG == 1
#define log_Debug(fmt, ...) log_Log(stdout, "DEBUG", fmt, ##__VA_ARGS__)
#define log_DebugHex(name, data, size) log_Hex(name, data, size)
#else
#define log_Debug(fmt, ...)
#define log_DebugHex(name, data, size)
#endif

#endif