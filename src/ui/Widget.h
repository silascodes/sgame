#ifndef SGAME_WIDGET_H_H
#define SGAME_WIDGET_H_H

#include <string>
#include <vector>
#include <lua5.3/lua.h>
#include "graphics/Graphics.h"
#include "client/MouseEvent.h"

namespace ui {

    void luai_Widget_Register(lua_State *lua);

    class Widget {
        public:
            virtual ~Widget() { }

            virtual void Init();
            virtual void Shutdown();

            void Render(graphics::Renderer *renderer);
            virtual void RenderSelf(graphics::Renderer *renderer);

            void AddChild(Widget *child);
            void RemoveChild(Widget *child);
            void RemoveAllChildren();

            Widget *GetChildAtPosition(int x, int y);

            bool HasMouseFocus();
            bool HasKeyboardFocus();
            bool IsGrabbed();

            virtual void SetPosition(int x, int y);
            virtual void SetSize(int width, int height);
            virtual void SetText(const std::string &text);

            void SetOnMouseClick(const std::string &function);

            std::string GetOnMouseClick();

            virtual void OnMouseMove(const client::MouseEvent &event);
            virtual void OnMouseOver();
            virtual void OnMouseOut();
            virtual void OnMouseButtonDown();
            virtual void OnMouseButtonUp();
            virtual void OnMouseClick(const client::MouseEvent &event);

        protected:
            int type;
            int x, y;
            int width, height;
            std::string text;
            Widget *parent;
            std::vector<Widget*> children;
            bool mouseFocus;
            bool keyboardFocus;
            bool grabbed;
            std::string onMouseClickHandler;
    };

}

#endif //SGAME_WIDGET_H_H
