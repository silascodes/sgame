#include <lua5.3/lauxlib.h>
#include <LuaWrapper/luawrapper.h>
#include "Sprite.h"

namespace game {

    static int luai_Sprite_SetAnimation(lua_State *lua) {
        Sprite *sprite = luaW_check<Sprite>(lua, 1);
        const char *name = luaL_checkstring(lua, 2);

        sprite->SetAnimation(name);

        return 0;
    }

    static luaL_Reg luai_Sprite_table[] = {
        {NULL, NULL}
    };

    static luaL_Reg luai_Sprite_metatable[] = {
        {"SetAnimation", luai_Sprite_SetAnimation},
        {NULL, NULL}
    };

    void luai_Sprite_Register(lua_State *lua) {
        luaW_register<Sprite>(
            lua,
            "Sprite",
            luai_Sprite_table,
            luai_Sprite_metatable,
            NULL
        );
    }

}
