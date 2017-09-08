#include "ui/widgets/ListBox.h"

namespace ui {
    namespace widgets {

        ListBox::ListBox() : Widget() {
            this->items.clear();

            this->hover = -1;
            this->selected = -1;
        }

        ListBox::~ListBox() {
            for(std::vector<ListItem*>::iterator i = this->items.begin(); i != this->items.end(); i++) {
                delete (*i);
            }

            this->items.clear();
        }

        void ListBox::RenderSelf(graphics::Renderer *renderer) {
            renderer->SetDrawColor(0xdd, 0xdd, 0xdd, 0xff);
            renderer->DrawRect(this->x, this->y, this->width, this->height);

            for(int i = 0; i < this->items.size(); i++) {
                if(this->selected == i) {
                    renderer->SetDrawColor(0x66, 0x66, 0x66, 0xff);
                    renderer->DrawFilledRect(this->items[i]->boundingBox.x, this->items[i]->boundingBox.y, this->items[i]->boundingBox.width, this->items[i]->boundingBox.height);
                }
                else if(this->hover == i) {
                    renderer->SetDrawColor(0x22, 0x22, 0x22, 0xff);
                    renderer->DrawFilledRect(this->items[i]->boundingBox.x, this->items[i]->boundingBox.y, this->items[i]->boundingBox.width, this->items[i]->boundingBox.height);
                }
                renderer->SetDrawColor(0x99, 0x99, 0x99, 0xff);
                renderer->DrawText(this->items[i]->boundingBox.x + 2, this->items[i]->boundingBox.y + 2, this->items[i]->text.c_str());
            }
        }

        void ListBox::AddItem(const std::string &item) {
            ListItem *listItem = new ListItem();

            listItem->text = item;

            listItem->boundingBox.x = 20;
            listItem->boundingBox.width = this->width - 20;
            listItem->boundingBox.height = 24;

            if(this->items.empty()) {
                listItem->boundingBox.y = 20;
            }
            else {
                listItem->boundingBox.y = this->items.back()->boundingBox.y + 28;
            }

            this->items.push_back(listItem);
        }

        void ListBox::OnMouseMove(const client::MouseEvent &event) {
            Widget::OnMouseMove(event);

            // Recalculate which item is being hovered over (if any)
            this->hover = -1;
            for(int i = 0; i < this->items.size(); i++) {
                if(this->items[i]->boundingBox.ContainsPoint(util::Vector2(event.GetX(), event.GetY()))) {
                    this->hover = i;
                }
            }
        }

        void ListBox::OnMouseOut() {
            Widget::OnMouseOut();

            // Clear hovered item when mouse moves out of our scope
            this->hover = -1;
        }

        void ListBox::OnMouseClick(const client::MouseEvent &event) {
            Widget::OnMouseClick(event);

            // Change the selected item
            this->selected = -1;
            for(int i = 0; i < this->items.size(); i++) {
                if(this->items[i]->boundingBox.ContainsPoint(util::Vector2(event.GetX(), event.GetY()))) {
                    this->selected = i;
                }
            }
        }

    }
}
