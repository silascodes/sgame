#ifndef SGAME_IINPUTLISTENER_H
#define SGAME_IINPUTLISTENER_H

#include "client/MouseEvent.h"
#include "client/KeyboardEvent.h"

namespace client {

    class IInputListener {
        public:
            virtual ~IInputListener() {}

            virtual bool OnMouseEvent(const client::MouseEvent &event) {
                return true;
            }

            virtual bool OnKeyboardEvent(const client::KeyboardEvent &event) {
                return true;
            }
    };

}

#endif //SGAME_IINPUTLISTENER_H
