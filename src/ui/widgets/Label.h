#ifndef SGAME_LABEL_H
#define SGAME_LABEL_H

#include "ui/Widget.h"

namespace ui {
    namespace widgets {

        class Label : public Widget {
            public:
                void RenderSelf(graphics::Renderer *renderer);
        };

    }
}

#endif //SGAME_LABEL_H
