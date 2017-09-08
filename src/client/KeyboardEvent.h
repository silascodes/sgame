#ifndef SGAME_KEYBOARDEVENT_H
#define SGAME_KEYBOARDEVENT_H

namespace client {

    typedef enum {
        KeyDown,
        KeyUp,
    } KeyboardEventType;

    class KeyboardEvent {
        public:
            KeyboardEvent(const KeyboardEventType type) {
                this->type = type;
            }

            KeyboardEventType GetType() const {
                return this->type;
            }

        private:
            KeyboardEventType type;

    };

}

#endif //SGAME_KEYBOARDEVENT_H
