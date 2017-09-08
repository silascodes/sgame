#include <lua5.3/lauxlib.h>
#include <LuaWrapper/luawrapper.h>
#include "Entity.h"

namespace game {

    static int luai_Entity_SetSprite(lua_State *lua) {
        Entity *entity = luaW_check<Entity>(lua, 1);
        const char *name = luaL_checkstring(lua, 2);

        entity->SetSprite(name);

        return 0;
    }

    static int luai_Entity_SetMoveDestination(lua_State *lua) {
        Entity *entity = luaW_check<Entity>(lua, 1);
        float x = luaL_checknumber(lua, 2);
        float y = luaL_checknumber(lua, 3);

        entity->SetMoveDestination(x, y);

        return 0;
    }

    static int luai_Entity_OnEvent(lua_State *lua) {
        Entity *entity = luaW_check<Entity>(lua, 1);
        const char *name = luaL_checkstring(lua, 2);
        const char *func = luaL_checkstring(lua, 3);

        entity->RegisterEventHandler(name, func);

        return 0;
    }

    static int luai_Entity_SetThinkTime(lua_State *lua) {
        Entity *entity = luaW_check<Entity>(lua, 1);
        int ms = luaL_checkinteger(lua, 2);

        entity->SetThinkTime(ms);

        return 0;
    }

    static luaL_Reg luai_Entity_table[] = {
        {NULL, NULL}
    };

    static luaL_Reg luai_Entity_metatable[] = {
        {"SetSprite", luai_Entity_SetSprite},
        {"SetMoveDestination", luai_Entity_SetMoveDestination},
        {"OnEvent", luai_Entity_OnEvent},
        {"SetThinkTime", luai_Entity_SetThinkTime},
        {NULL, NULL}
    };

    void luai_Entity_Register(lua_State *lua) {
        luaW_register<Entity>(
            lua,
            "Entity",
            luai_Entity_table,
            luai_Entity_metatable,
            NULL
        );
    }

}
