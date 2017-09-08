#ifndef SGAME_BUTTON_H
#define SGAME_BUTTON_H

#include "ui/Widget.h"

namespace ui {
    namespace widgets {

        void luai_Button_Register(lua_State *lua);

        class Button : public Widget {
            public:
                void RenderSelf(graphics::Renderer *renderer);
        };

    }
}

#endif //SGAME_BUTTON_H
