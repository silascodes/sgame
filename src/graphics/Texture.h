#ifndef SGAME_TEXTURE_H
#define SGAME_TEXTURE_H

namespace graphics {

    class Texture {
        public:
            void Init(SDL_Renderer *renderer, const std::string &name);
            void Shutdown();

            void Render(SDL_Rect *src, SDL_Rect *dst);

        private:
            SDL_Renderer *renderer;
            SDL_Texture *tex;
    };

}

#endif //SGAME_TEXTURE_H
