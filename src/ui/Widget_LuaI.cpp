#include <lua5.3/lauxlib.h>
#include <LuaWrapper/luawrapper.h>
#include "ui/Widget.h"

namespace ui {

    static int luai_Widget_AddChild(lua_State *lua) {
        Widget *widget = luaW_check<Widget>(lua, 1);
        Widget *child = luaW_check<Widget>(lua, 2);

        widget->AddChild(child);

        return 0;
    }

    static int luai_Widget_RemoveChild(lua_State *lua) {
        Widget *widget = luaW_check<Widget>(lua, 1);
        Widget *child = luaW_check<Widget>(lua, 2);

        widget->RemoveChild(child);

        return 0;
    }

    static int luai_Widget_RemoveAllChildren(lua_State *lua) {
        Widget *widget = luaW_check<Widget>(lua, 1);

        widget->RemoveAllChildren();

        return 0;
    }

    static int luai_Widget_SetPosition(lua_State *lua) {
        Widget *widget = luaW_check<Widget>(lua, 1);
        int x = luaL_checkinteger(lua, 2);
        int y = luaL_checkinteger(lua, 3);

        widget->SetPosition(x, y);

        return 0;
    }

    static int luai_Widget_SetSize(lua_State *lua) {
        Widget *widget = luaW_check<Widget>(lua, 1);
        int width = luaL_checkinteger(lua, 2);
        int height = luaL_checkinteger(lua, 3);

        widget->SetSize(width, height);

        return 0;
    }

    static int luai_Widget_SetText(lua_State *lua) {
        Widget *widget = luaW_check<Widget>(lua, 1);
        const char *text = luaL_checkstring(lua, 2);

        widget->SetText(text);

        return 0;
    }

    static int luai_Widget_SetOnMouseClick(lua_State *lua) {
        Widget *widget = luaW_check<Widget>(lua, 1);
        const char *function = luaL_checkstring(lua, 2);

        widget->SetOnMouseClick(function);

        return 0;
    }

    static luaL_Reg luai_Widget_table[] = {
        { NULL, NULL }
    };

    static luaL_Reg luai_Widget_metatable[] = {
        { "AddChild", luai_Widget_AddChild },
        { "RemoveChild", luai_Widget_RemoveChild },
        { "RemoveAllChildren", luai_Widget_RemoveAllChildren },
        { "SetPosition", luai_Widget_SetPosition },
        { "SetSize", luai_Widget_SetSize },
        { "SetText", luai_Widget_SetText },
        { "SetOnMouseClick", luai_Widget_SetOnMouseClick },
        { NULL, NULL }
    };

    void luai_Widget_Register(lua_State *lua) {
        luaW_register<Widget>(
            lua,
            "Widget",
            luai_Widget_table,
            luai_Widget_metatable,
            NULL
        );
    }

}
