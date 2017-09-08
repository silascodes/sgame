#include <lua5.3/lauxlib.h>
#include <LuaWrapper/luawrapper.h>
#include "Game.h"

namespace game {

    static int luai_State_ChangeMap(lua_State *lua) {
        State *state = luaW_check<State>(lua, 1);
        const char *name = luaL_checkstring(lua, 2);

        state->ChangeMap(name);

        return 0;
    }

    static luaL_Reg luai_State_table[] = {
        {NULL, NULL}
    };

    static luaL_Reg luai_State_metatable[] = {
        {"ChangeMap", luai_State_ChangeMap},
        {NULL, NULL}
    };

    void luai_State_Register(lua_State *lua) {
        luaW_register<State>(
            lua,
            "State",
            luai_State_table,
            luai_State_metatable,
            NULL
        );
    }

}
