#ifndef SGAME_UI_H
#define SGAME_UI_H

#include <lua5.3/lua.h>

#include "ui/Screen.h"
#include "ui/Widget.h"

#include "ui/widgets/Container.h"
#include "ui/widgets/Button.h"
#include "ui/widgets/Label.h"
#include "ui/widgets/ListBox.h"
#include "ui/widgets/Textbox.h"

namespace ui {

    void luai_Register(lua_State *lua);

}

#endif //SGAME_UI_H
