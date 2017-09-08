#include <lua5.3/lauxlib.h>
#include <LuaWrapper/luawrapper.h>
#include "ui/widgets/Button.h"

namespace ui {
    namespace widgets {

        Button *luai_Button_New(lua_State *lua) {
            return new Button();
        }

        static luaL_Reg luai_Button_table[] = {
            {NULL, NULL}
        };

        static luaL_Reg luai_Button_metatable[] = {
            {NULL, NULL}
        };

        void luai_Button_Register(lua_State *lua) {
            luaW_register<Button>(
                lua,
                "Button",
                luai_Button_table,
                luai_Button_metatable,
                luai_Button_New
            );

            luaW_extend<Button, Widget>(lua);
        }

    }
}
