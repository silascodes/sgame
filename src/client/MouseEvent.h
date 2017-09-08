#ifndef SGAME_MOUSEEVENT_H
#define SGAME_MOUSEEVENT_H

#include "util/Vector2.h"

namespace client {

    typedef enum {
        Move,
        ButtonDown,
        ButtonUp,
        WheelMove,
    } MouseEventType;

    typedef enum {
        Mouse1,
        Mouse2,
        Mouse3,
        Mouse4,
        Mouse5,
    } MouseButton;

    class MouseEvent {
        public:
            MouseEvent(const MouseEventType type, const int x, const int y, const int wheelX = 0, const int wheelY = 0) {
                this->type = type;
                this->x = x;
                this->y = y;
                this->wheelX = wheelX;
                this->wheelY = wheelY;
            }

            MouseEvent(const MouseEventType type, const int x, const int y, MouseButton button) {
                this->type = type;
                this->x = x;
                this->y = y;
                this->button = button;
            }

            MouseEventType GetType() const {
                return this->type;
            }

            int GetX() const {
                return this->x;
            }

            int GetY() const {
                return this->y;
            }

            int GetWheelX() const {
                return this->wheelX;
            }

            int GetWheelY() const {
                return this->wheelY;
            }

            MouseButton GetButton() const {
                return this->button;
            }

        private:
            MouseEventType type;
            int x, y;
            int wheelX, wheelY;
            MouseButton button;
    };

}

#endif //SGAME_MOUSEEVENT_H
