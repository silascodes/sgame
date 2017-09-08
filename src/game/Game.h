#ifndef SGAME_GAME_H
#define SGAME_GAME_H

#include <string>
#include <vector>
#include <lua5.3/lua.h>
#include "client/Client.h"
#include "State.h"
#include "graphics/Graphics.h"

namespace game {

    void luai_Game_Register(lua_State *lua);

    class Game {
        public:
            void Init(client::Client *client, const std::string &initialState);
            void Shutdown();

            void RunFrame(graphics::Renderer *renderer, int elapsedMs);

            void PushState(const std::string &name);
            void PopState();

            void LoadGame(const std::string &name);
            void SaveGame(const std::string &name);

            client::Client *GetClient();

        private:
            void PerformPushState();
            void PerformPopState();

            client::Client *client;
            std::vector<State *> states;
            std::string statePush;
            bool statePop;
    };

}

#endif //SGAME_GAME_H
