#include "ui/widgets/Label.h"

namespace ui {
    namespace widgets {

        void Label::RenderSelf(graphics::Renderer *renderer) {
            renderer->SetDrawColor(0xff, 0xff, 0xff, 0xff);
            renderer->DrawText(this->x, this->y, this->text.c_str());
        }

    }
}
