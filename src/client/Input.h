#ifndef SGAME_INPUT_H
#define SGAME_INPUT_H

#include <vector>
#include <SDL2/SDL.h>
#include "IInputListener.h"

#define INPUT_EVENT_KEY_DOWN         1
#define INPUT_EVENT_KEY_UP           2

namespace client {

    typedef struct {
        int type;
    } input_event_t;

    class Input {
        public:
            void Init();
            void Shutdown();

            void AddListener(IInputListener *listener);
            void RemoveListener(IInputListener *listener);

            void HandleEvent(SDL_Event *event);

        private:
            MouseButton TranslateSDLMouseButton(unsigned int button);

            void HandleMouseEvent(SDL_Event *event);
            void HandleKeyboardEvent(SDL_Event *event);

            int keyMap[256];
            std::vector<IInputListener*> listeners;

    };

}

#endif //SGAME_INPUT_H
