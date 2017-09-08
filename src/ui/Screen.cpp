#include "ui/Screen.h"
#include "ui/widgets/Container.h"
#include "Log.h"

namespace ui {

    void Screen::Init(lua_State *scriptState) {
        Log_Info("");

        this->root = new widgets::Container();

        this->mouseFocus = NULL;
        this->keyboardFocus = NULL;

        this->scriptState = scriptState;
    }

    void Screen::Shutdown() {
        Log_Info("");

        delete this->root;
    }

    void Screen::Render(graphics::Renderer *renderer) {
        // Start rendering at the root widget
        this->root->Render(renderer);
    }

    Widget *Screen::GetRootWidget() {
        return this->root;
    }

    bool Screen::OnMouseEvent(const client::MouseEvent &event) {
        bool grabbed = false;

        Widget *widget = this->GetRootWidget()->GetChildAtPosition(event.GetX(), event.GetY());
        if(widget) {
            switch(event.GetType()) {
                case client::MouseEventType::Move:
                    if(widget != this->mouseFocus) {
                        // If there was a widget that previously had mouse focus, send it the mouse out event
                        if(this->mouseFocus) {
                            this->mouseFocus->OnMouseOut();
                        }

                        // Set as mouse focused widget, send mouse over event
                        this->mouseFocus = widget;
                        this->mouseFocus->OnMouseOver();
                    }

                    widget->OnMouseMove(event);
                    break;

                case client::MouseEventType::ButtonDown:
                    widget->OnMouseButtonDown();
                    break;

                case client::MouseEventType::ButtonUp:
                    grabbed = widget->IsGrabbed();

                    widget->OnMouseButtonUp();

                    // If the widget was grabbed, send a click event too
                    if(grabbed) {
                        widget->OnMouseClick(event);
                        this->CallFunc(widget->GetOnMouseClick());
                    }
                    break;
            }
        }

        return true;
    }

    void Screen::CallFunc(const std::string &name) {
        if(!this->scriptState || name.empty()) {
            return;
        }

        lua_getglobal(this->scriptState, name.c_str());
        if(lua_isfunction(this->scriptState, -1) != 1) {
            Log_Error("states %s function not found", name.c_str());
            return;
        }
        if(lua_pcall(this->scriptState, 0, 0, 0) != LUA_OK) {
            Log_Error("failed to call states %s function (%s)", name.c_str(), lua_tostring(this->scriptState, -1));
            return;
        }
    }

}
