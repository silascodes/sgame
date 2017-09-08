#include <lua5.3/lauxlib.h>
#include <LuaWrapper/luawrapper.h>
#include "Game.h"

namespace game {

    static int luai_Game_PushState(lua_State *lua) {
        Game *game = luaW_check<Game>(lua, 1);
        const char *name = luaL_checkstring(lua, 2);

        game->PushState(name);

        return 0;
    }

    static int luai_Game_PopState(lua_State *lua) {
        Game *game = luaW_check<Game>(lua, 1);

        game->PopState();

        return 0;
    }

    static luaL_Reg luai_Game_table[] = {
        {NULL, NULL}
    };

    static luaL_Reg luai_Game_metatable[] = {
        {"PushState", luai_Game_PushState},
        {"PopState",  luai_Game_PopState},
        {NULL, NULL}
    };

    void luai_Game_Register(lua_State *lua) {
        luaW_register<Game>(
            lua,
            "Game",
            luai_Game_table,
            luai_Game_metatable,
            NULL
        );
    }

}
