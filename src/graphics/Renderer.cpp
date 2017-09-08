#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "graphics/Renderer.h"
#include "Log.h"

namespace graphics {

    void Renderer::Init(SDL_Window *window) {
        Log_Info("");

        this->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (!this->renderer) {
            Log_Error("failed to create renderer");
            SDL_DestroyWindow(window);
            SDL_Quit();
            return;
        }

        this->color.r = 0xff;
        this->color.g = 0xff;
        this->color.b = 0xff;
        this->color.a = 0xff;
        SDL_SetRenderDrawColor(this->renderer, this->color.r, this->color.g, this->color.b, this->color.a);

        if (TTF_Init() == -1) {
            Log_Error("failed to init SDL TTF (%s)", TTF_GetError());
            return;
        }

        this->font = TTF_OpenFont("data/fonts/ttf-bitstream-vera-1.10/VeraBd.ttf", 16);
        if (!this->font) {
            Log_Error("failed to load font (%s)", TTF_GetError());
            return;
        }

        SDL_RenderSetLogicalSize(this->renderer, 1280, 720);

        this->camera = NULL;
    }

    void Renderer::Shutdown() {
        Log_Info("");

        TTF_CloseFont(this->font);
        TTF_Quit();

        SDL_DestroyRenderer(this->renderer);
    }

    void Renderer::BeginFrame() {
        this->SetDrawColor(0x00, 0x00, 0x00, 0xff);
        SDL_RenderClear(this->renderer);
    }

    void Renderer::EndFrame() {
        SDL_RenderPresent(this->renderer);
    }

    Texture *Renderer::LoadTexture(const std::string &name) {
        try {
            Texture *tex = this->textures.at(name);
            return tex;
        }
        catch(std::out_of_range e) {
            Texture *tex = new Texture();
            tex->Init(this->renderer, name);

            this->textures[name] = tex;

            return tex;
        }
    }

    void Renderer::SetCamera(Camera *cam) {
        this->camera = cam;

        if(this->camera) {
            util::Vector2 camPos = this->camera->GetPosition();
            this->offsetX = camPos.x;
            this->offsetY = camPos.y;
        }
        else {
            this->offsetX = 0;
            this->offsetY = 0;
        }
    }

    Camera *Renderer::GetCamera() {
        return this->camera;
    }

    void Renderer::SetDrawColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
        this->color.r = r;
        this->color.g = g;
        this->color.b = b;
        this->color.a = a;

        SDL_SetRenderDrawColor(this->renderer, r, g, b, a);
    }

    void Renderer::DrawPoint(int x, int y) {
        SDL_RenderDrawPoint(
            this->renderer,
            x - this->offsetX,
            y - this->offsetY
        );
    }

    void Renderer::DrawLine(int x1, int y1, int x2, int y2) {
        SDL_RenderDrawLine(
            this->renderer,
            x1 - this->offsetX,
            y1 - this->offsetY,
            x2 - this->offsetX,
            y2 - this->offsetY
        );
    }

    void Renderer::DrawRect(int x, int y, int width, int height) {
        SDL_Rect rect;
        rect.x = x - this->offsetX;
        rect.y = y - this->offsetY;
        rect.w = width;
        rect.h = height;

        SDL_RenderDrawRect(this->renderer, &rect);
    }

    void Renderer::DrawFilledRect(int x, int y, int width, int height) {
        SDL_Rect rect;
        rect.x = x - this->offsetX;
        rect.y = y - this->offsetY;
        rect.w = width;
        rect.h = height;

        SDL_RenderFillRect(this->renderer, &rect);
    }

    void Renderer::DrawText(int x, int y, const char *text) {
        if (!text) {
            return;
        }

        if (strlen(text) == 0) {
            return;
        }

        SDL_Surface *surf = TTF_RenderUTF8_Solid(this->font, text, this->color);
        if (!surf) {
            Log_Error("failed to render text");
            return;
        }

        SDL_Texture *tex = SDL_CreateTextureFromSurface(this->renderer, surf);
        if (!tex) {
            Log_Error("failed to convert text to texture");
            return;
        }

        SDL_Rect src, dst;
        src.x = 0;
        src.y = 0;
        SDL_QueryTexture(tex, NULL, NULL, &src.w, &src.h);

        dst.x = x - this->offsetX;
        dst.y = y - this->offsetY;
        dst.w = src.w;
        dst.h = src.h;

        SDL_RenderCopy(this->renderer, tex, &src, &dst);
    }

}
