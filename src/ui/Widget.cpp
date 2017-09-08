#include "Log.h"
#include "ui/Widget.h"

namespace ui {

    void Widget::Init() {
        this->x = 0;
        this->y = 0;
        this->width = 0;
        this->height = 0;

        this->mouseFocus = false;
        this->keyboardFocus = false;
        this->grabbed = false;
    }

    void Widget::Shutdown() {
        // TODO clean up event handlers and widgets???
    }

    void Widget::Render(graphics::Renderer *renderer) {
        // Render this widget
        this->RenderSelf(renderer);

        // Render this widgets children
        for(std::vector<Widget*>::iterator i = this->children.begin(); i != this->children.end(); i++) {
            (*i)->Render(renderer);
        }
    }

    void Widget::RenderSelf(graphics::Renderer *renderer) {
        // Base widget does not render anything
    }

    void Widget::AddChild(Widget *child) {
        child->parent = this;

        this->children.push_back(child);
    }

    void Widget::RemoveChild(Widget *child) {
        child->parent = NULL;

        this->children.erase(std::remove(this->children.begin(), this->children.end(), child), this->children.end());
    }

    void Widget::RemoveAllChildren() {
        for(std::vector<Widget*>::iterator i = this->children.begin(); i != this->children.end(); i++) {
            (*i)->parent = NULL;
        }

        this->children.clear();
    }

    Widget *Widget::GetChildAtPosition(int x, int y) {
        if(x >= this->x && y >= this->y && x <= this->x + this->width && y <= this->y + this->height) {
            for(std::vector<Widget*>::iterator i = this->children.begin(); i != this->children.end(); i++) {
                Widget *search = (*i)->GetChildAtPosition(x, y);
                if(search) {
                    return search;
                }
            }
            return this;
        }

        return NULL;
    }

    bool Widget::HasMouseFocus() {
        return this->mouseFocus;
    }

    bool Widget::HasKeyboardFocus() {
        return this->keyboardFocus;
    }

    bool Widget::IsGrabbed() {
        return this->grabbed;
    }

    void Widget::SetPosition(int x, int y) {
        this->x = x;
        this->y = y;
    }

    void Widget::SetSize(int width, int height) {
        this->width = width;
        this->height = height;
    }

    void Widget::SetText(const std::string &text) {
        this->text = text;
    }

    void Widget::SetOnMouseClick(const std::string &function) {
        this->onMouseClickHandler = function;
    }

    std::string Widget::GetOnMouseClick() {
        return this->onMouseClickHandler;
    }

    void Widget::OnMouseMove(const client::MouseEvent &event) {

    }

    void Widget::OnMouseOver() {
        this->mouseFocus = true;
    }

    void Widget::OnMouseOut() {
        this->mouseFocus = false;
        this->grabbed = false;
    }

    void Widget::OnMouseButtonDown() {
        this->grabbed = true;
    }

    void Widget::OnMouseButtonUp() {
        this->grabbed = false;
    }

    void Widget::OnMouseClick(const client::MouseEvent &event) {

    }

}
