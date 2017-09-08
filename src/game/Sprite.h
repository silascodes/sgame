#ifndef SGAME_SPRITE_H
#define SGAME_SPRITE_H

#include <vector>
#include <map>
#include <string>
#include <lua5.3/lua.h>
#include "graphics/Graphics.h"

namespace game {

    void luai_Sprite_Register(lua_State *lua);

    class Sprite {
        public:
            void Init(const std::string &name, graphics::Renderer *renderer);
            void Shutdown();

            void Update(int elapsedMs);
            void Render(graphics::Renderer *renderer);

            void SetPosition(int x, int y);
            void SetAnimation(const std::string &name);

        private:
            int x, y;
            int width, height;
            int frameTime;
            int currentTime;
            int currentFrame;
            std::string currentAnimation;
            std::map<std::string, std::vector<int>> animations;
            graphics::Texture *texture;
    };

}

#endif //SGAME_SPRITE_H
