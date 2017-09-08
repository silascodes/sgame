#include "ui/widgets/Button.h"

namespace ui {
    namespace widgets {

        void Button::RenderSelf(graphics::Renderer *renderer) {
            unsigned char fillColour = 0x99;
            unsigned char borderColour = 0xdd;

            if(this->IsGrabbed()) {
                fillColour = 0x44;
                borderColour = 0x88;
            }
            else if(this->HasMouseFocus()) {
                fillColour = 0xbb;
                borderColour = 0xff;
            }

            renderer->SetDrawColor(fillColour, fillColour, fillColour, 0xff);
            renderer->DrawFilledRect(this->x, this->y, this->width, this->height);

            renderer->SetDrawColor(borderColour, borderColour, borderColour, 0xff);
            renderer->DrawRect(this->x, this->y, this->width, this->height);

            renderer->SetDrawColor(0x00, 0x00, 0x00, 0xff);
            renderer->DrawText(this->x + 5, this->y + 5, this->text.c_str());
        }

    }
}
