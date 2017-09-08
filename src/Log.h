#ifndef SGAME_LOG_H
#define SGAME_LOG_H

#include <lua5.3/lua.h>

#define LOG_INFO            1
#define LOG_WARNING         2
#define LOG_ERROR           4
#define LOG_ANY             (LOG_INFO | LOG_WARNING | LOG_ERROR)

typedef void (*log_listener_func_t)(int type, const char* msg);

void Log_Init();
void Log_Shutdown();

int Log_AddListener(int type, log_listener_func_t func);
void Log_RemoveListener(int id);

void Log_WriteLine(int type, const char* func, const char* fmt, ...);

#define Log_Info(...) Log_WriteLine(LOG_INFO, __PRETTY_FUNCTION__, __VA_ARGS__)
#define Log_Warning(...) Log_WriteLine(LOG_WARNING, __PRETTY_FUNCTION__, __VA_ARGS__)
#define Log_Error(...) Log_WriteLine(LOG_ERROR, __PRETTY_FUNCTION__, __VA_ARGS__)

void luaapi_Log_Init(lua_State* l);

#endif //SGAME_LOG_H
