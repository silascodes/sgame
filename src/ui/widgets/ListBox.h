#ifndef SGAME_LISTBOX_H
#define SGAME_LISTBOX_H

#include "ui/Widget.h"
#include "util/Rect.h"

namespace ui {
    namespace widgets {

        void luai_ListBox_Register(lua_State *lua);

        class ListBox : public Widget {
            public:
                ListBox();
                ~ListBox();

                void RenderSelf(graphics::Renderer *renderer);

                void AddItem(const std::string &item);

                void OnMouseMove(const client::MouseEvent &event);
                void OnMouseOut();
                void OnMouseClick(const client::MouseEvent &event);

            protected:
                class ListItem {
                    public:
                        std::string text;
                        util::Rect boundingBox;
                };

                std::vector<ListItem*> items;
                int hover;
                int selected;
        };

    }
}

#endif //SGAME_LISTBOX_H
