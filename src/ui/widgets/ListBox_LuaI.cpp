#include <lua5.3/lauxlib.h>
#include <LuaWrapper/luawrapper.h>
#include "ui/widgets/ListBox.h"

namespace ui {
    namespace widgets {

        ListBox *luai_ListBox_New(lua_State *lua) {
            return new ListBox();
        }

        static int luai_ListBox_AddItem(lua_State *lua) {
            ListBox *listBox = luaW_check<ListBox>(lua, 1);
            const char *item = luaL_checkstring(lua, 2);

            listBox->AddItem(item);

            return 0;
        }

        static luaL_Reg luai_ListBox_table[] = {
            {NULL, NULL}
        };

        static luaL_Reg luai_ListBox_metatable[] = {
            { "AddItem", luai_ListBox_AddItem },
            {NULL, NULL}
        };

        void luai_ListBox_Register(lua_State *lua) {
            luaW_register<ListBox>(
                lua,
                "ListBox",
                luai_ListBox_table,
                luai_ListBox_metatable,
                luai_ListBox_New
            );

            luaW_extend<ListBox, Widget>(lua);
        }

    }
}
