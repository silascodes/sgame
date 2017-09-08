#ifndef SGAME_RENDERER_H
#define SGAME_RENDERER_H

#include <string>
#include <map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "graphics/Texture.h"
#include "graphics/Camera.h"

namespace graphics {

    class Renderer {
        public:
            void Init(SDL_Window *window);
            void Shutdown();

            void BeginFrame();
            void EndFrame();

            Texture *LoadTexture(const std::string &name);

            void SetCamera(Camera *cam);
            Camera *GetCamera();

            void SetDrawColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

            void DrawPoint(int x, int y);
            void DrawLine(int x1, int y1, int x2, int y2);
            void DrawRect(int x, int y, int width, int height);
            void DrawFilledRect(int x, int y, int width, int height);
            void DrawText(int x, int y, const char *text);

        private:
            SDL_Renderer *renderer;
            SDL_Color color;
            TTF_Font *font;
            std::map<std::string, Texture*> textures;
            Camera *camera;
            int offsetX, offsetY;
    };

}

#endif //SGAME_RENDERER_H
