#include <lua5.3/lua.h>
#include "UI.h"

namespace ui {

    void luai_Register(lua_State *lua) {
        luai_Screen_Register(lua);
        luai_Widget_Register(lua);

        widgets::luai_Container_Register(lua);
        widgets::luai_Button_Register(lua);
        widgets::luai_ListBox_Register(lua);
    }

}
