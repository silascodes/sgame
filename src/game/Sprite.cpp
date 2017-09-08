#include "game/Sprite.h"
#include "Log.h"

namespace game {

    void Sprite::Init(const std::string &name, graphics::Renderer *renderer) {
        Log_Info(name.c_str());

        this->x = 0;
        this->y = 0;

        this->width = 32;
        this->height = 32;

        this->frameTime = 300;

        this->currentTime = 0;
        this->currentFrame = 0;

        this->animations["Stand"].push_back(1);

        this->animations["Test1"].push_back(2);
        this->animations["Test1"].push_back(3);

        this->currentAnimation = "Test1";

        this->texture = renderer->LoadTexture(name);
    }

    void Sprite::Shutdown() {

    }

    void Sprite::Update(int elapsedMs) {
        this->currentTime += elapsedMs;
        if(this->currentTime >= this->frameTime) {
            this->currentTime -= this->frameTime;

            this->currentFrame++;
            if(this->currentFrame >= this->animations[this->currentAnimation].size()) {
                this->currentFrame = 0;
            }
        }
    }

    void Sprite::Render(graphics::Renderer *renderer) {
        SDL_Rect src, dst;
        int curFrameIdx = this->animations[this->currentAnimation][this->currentFrame] - 1;
        util::Vector2 cam = renderer->GetCamera()->GetPosition();

        src.x = curFrameIdx * this->width;
        src.y = 0;
        src.w = 32;
        src.h = 32;

        dst.x = (this->x - cam.x) - 16;
        dst.y = (this->y - cam.y) - 16;
        dst.w = 32;
        dst.h = 32;

        this->texture->Render(&src, &dst);
    }

    void Sprite::SetPosition(int x, int y) {
        this->x = x;
        this->y = y;
    }

    void Sprite::SetAnimation(const std::string &name) {
        this->currentAnimation = name;
    }

}
