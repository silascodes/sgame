#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <lua5.3/lua.h>
#include <lua5.3/lauxlib.h>
#include <malloc.h>
#include "Log.h"

typedef struct log_listener_s {
    int id;
    int type;
    log_listener_func_t func;
    struct log_listener_s* next;
} log_listener_t;

static log_listener_t* log_listeners;
static int log_nextId;

void Log_Init() {
    // Start with no listeners
    log_listeners = NULL;

    // First id is always 1, as we use 0 for error/invalid
    log_nextId = 1;
}

void Log_Shutdown() {
    // Free all the log listeners
    log_listener_t* search = log_listeners;
    log_listener_t* listener;
    while(search) {
        listener = search;
        search = search->next;
        free(listener);
    }
}

int Log_AddListener(int type, log_listener_func_t func) {
    // If listener would receive no messages, don't bother
    if(type == 0) {
        Log_Warning("attempted to add log listener that listens for no log messages (blank type)");
        return 0;
    }

    // Allocate memory for the listener object
    log_listener_t* listener = (log_listener_t*)malloc(sizeof(log_listener_t));
    if(!listener) {
        Log_Error("failed to allocate memory for log listener");
        return 0;
    }
    memset(listener, 0, sizeof(log_listener_t));

    // Set up the listener
    listener->id = log_nextId;
    listener->type = type;
    listener->func = func;

    // Move next free id along
    log_nextId++;
    if(log_nextId == 0) {
        log_nextId++;
    }

    // Link the new listener into our list of listeners
    if(!log_listeners) {
        log_listeners = listener;
    }
    else {
        log_listener_t* search = log_listeners;
        while(search->next) {
            search = search->next;
        }
        search->next = listener;
    }

    // Return the id of the new listener
    return listener->id;
}

void Log_RemoveListener(int id) {
    // Zero is an invalid id, so just bail
    if(id == 0) {
        return;
    }

    // Search the listeners for one with this id
    log_listener_t* search = log_listeners;
    log_listener_t* prev = NULL;
    while(search) {
        // Found it!
        if(search->id == id) {
            // Relink our list to remove the listener
            if(prev) {
                prev->next = search->next;
            }
            else {
                log_listeners = search->next;
            }

            // Free the listener object
            free(search);
        }
        // Not this one...
        else {
            prev = search;
            search = search->next;
        }
    }
}

static void Log_BroadcastMessage(int type, const char* msg) {
    log_listener_t* listener = log_listeners;
    while(listener) {
        // If the listener wants this type of message, call its function
        if(listener->type & type) {
            listener->func(type, msg);
        }

        listener = listener->next;
    }
}

void Log_WriteLine(int type, const char* func, const char* fmt, ...) {
    time_t rawTime;
    struct tm* timeInfo;

    // Get current local time
    time(&rawTime);
    timeInfo = localtime(&rawTime);

    // Print time to nice string with hour, minutes and seconds
    char timeStr[11];
    memset(timeStr, 0, 11);
    strftime(timeStr, 10, "%H:%M:%S", timeInfo);

    // Get a nice 2 character string for the message type
    char typeStr[3];
    typeStr[2] = 0;
    switch(type) {
        case LOG_WARNING:
            strcpy(typeStr, "WW");
            break;

        case LOG_ERROR:
            strcpy(typeStr, "EE");
            break;

        default:
            strcpy(typeStr, "  ");
    }

    // Print out the variable arguments to a buffer string
    char buf1[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf1, 1024, fmt, args);
    va_end(args);

    // Print the header and the message to a string buffer
    char buf2[1024];
    snprintf(buf2, 1024, "[%s][%s][%s] %s", typeStr, timeStr, func, buf1);

    // Send message to stdout
    printf("%s\n", buf2);

    // Send message to any listeners
    Log_BroadcastMessage(type, buf2);
}

static int luaapi_Log_Info(lua_State *lua) {
    const char* msg = luaL_checkstring(lua, 1);
    if(!msg) {
        return 0;
    }

    Log_Info(msg);

    return 1;
}

static int luaapi_Log_Warning(lua_State *lua) {
    const char* msg = luaL_checkstring(lua, 1);
    if(!msg) {
        return 0;
    }

    Log_Warning(msg);

    return 1;
}

static int luaapi_Log_Error(lua_State *lua) {
    const char* msg = luaL_checkstring(lua, 1);
    if(!msg) {
        return 0;
    }

    Log_Error(msg);

    return 1;
}

void luaapi_Log_Init(lua_State* l) {
    lua_register(l, "Log_Info", luaapi_Log_Info);
    lua_register(l, "Log_Warning", luaapi_Log_Warning);
    lua_register(l, "Log_Error", luaapi_Log_Error);
}
