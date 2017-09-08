#include <string>
#include <SDL2/SDL_image.h>
#include "graphics/Texture.h"
#include "Log.h"

namespace graphics {

    void Texture::Init(SDL_Renderer *renderer, const std::string &name) {
        this->renderer = renderer;

        this->tex = IMG_LoadTexture(renderer, name.c_str());
        if(!this->tex) {
            Log_Error("failed to load texture %s", name.c_str());
            return;
        }
    }

    void Texture::Shutdown() {
        SDL_DestroyTexture(this->tex);
    }

    void Texture::Render(SDL_Rect *src, SDL_Rect *dst) {
        dst->x;
        dst->y;

        SDL_RenderCopy(this->renderer, this->tex, src, dst);
    }

}
