#include <lua5.3/lauxlib.h>
#include <LuaWrapper/luawrapper.h>
#include "ui/Screen.h"

namespace ui {

    static int luai_Screen_GetRootWidget(lua_State *lua) {
        Screen *screen = luaW_check<Screen>(lua, 1);

        luaW_push<Widget>(lua, screen->GetRootWidget());

        return 1;
    }

    static luaL_Reg luai_Screen_table[] = {
        { NULL, NULL }
    };

    static luaL_Reg luai_Screen_metatable[] = {
        { "GetRootWidget", luai_Screen_GetRootWidget },
        { NULL, NULL }
    };

    void luai_Screen_Register(lua_State *lua) {
        luaW_register<Screen>(
            lua,
            "Screen",
            luai_Screen_table,
            luai_Screen_metatable,
            NULL
        );
    }

}
