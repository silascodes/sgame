#ifndef SGAME_CONTAINER_H
#define SGAME_CONTAINER_H

#include "ui/Widget.h"
#include "graphics/Graphics.h"

namespace ui {
    namespace widgets {

        void luai_Container_Register(lua_State *lua);

        class Container : public Widget {
            public:
                Container();
        };

    }
}

#endif //SGAME_CONTAINER_H
