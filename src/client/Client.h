#ifndef SGAME_CLIENT_H
#define SGAME_CLIENT_H

#include <SDL2/SDL.h>
#include "graphics/Renderer.h"
#include "ui/UI.h"
#include "Input.h"

namespace client {

    void luai_Register(lua_State *lua);

    class Client {
        public:
            void Init();
            void Shutdown();

            bool BeginFrame();
            void EndFrame();

            int64_t GetTicks();
            int64_t GetFrequency();

            graphics::Renderer *GetRenderer();
            client::Input *GetInput();

            void Quit();

        private:
            void PumpEvents();

            bool quitRequested;
            SDL_Window *window;
            graphics::Renderer *renderer;
            Input *input;
            ui::Screen *ui;
    };

}

#endif //SGAME_CLIENT_H
