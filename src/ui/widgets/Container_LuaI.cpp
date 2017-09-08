#include <lua5.3/lauxlib.h>
#include <LuaWrapper/luawrapper.h>
#include "ui/widgets/Container.h"

namespace ui {
    namespace widgets {

        Container *luai_Container_New(lua_State *lua) {
            return new Container();
        }

        static luaL_Reg luai_Container_table[] = {
            {NULL, NULL}
        };

        static luaL_Reg luai_Container_metatable[] = {
            {NULL, NULL}
        };

        void luai_Container_Register(lua_State *lua) {
            luaW_register<Container>(
                lua,
                "Container",
                luai_Container_table,
                luai_Container_metatable,
                luai_Container_New
            );

            luaW_extend<Container, Widget>(lua);
        }

    }
}
