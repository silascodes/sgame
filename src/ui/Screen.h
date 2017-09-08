#ifndef SGAME_SCREEN_H
#define SGAME_SCREEN_H

#include "ui/Widget.h"
#include "client/IInputListener.h"

namespace ui {

    void luai_Screen_Register(lua_State *lua);

    class Screen : public client::IInputListener {
        public:
            void Init(lua_State *scriptState = NULL);
            void Shutdown();

            void Render(graphics::Renderer *renderer);

            Widget *GetRootWidget();

            bool OnMouseEvent(const client::MouseEvent &event);

        private:
            void CallFunc(const std::string &name);

            Widget *root;
            Widget *mouseFocus;
            Widget *keyboardFocus;
            lua_State *scriptState;
    };

}

#endif //SGAME_SCREEN_H
