#include <lua5.3/lauxlib.h>
#include <LuaWrapper/luawrapper.h>
#include "Client.h"

namespace client {

    static int luai_Client_Quit(lua_State *lua) {
        Client *client = luaW_check<Client>(lua, 1);

        client->Quit();

        return 0;
    }

    static luaL_Reg luai_Client_table[] = {
        {NULL, NULL}
    };

    static luaL_Reg luai_Client_metatable[] = {
        {"Quit", luai_Client_Quit},
        {NULL, NULL}
    };

    void luai_Register(lua_State *lua) {
        luaW_register<Client>(
            lua,
            "Client",
            luai_Client_table,
            luai_Client_metatable,
            NULL
        );
    }

}
