#ifndef SGAME_STATE_H
#define SGAME_STATE_H

#include <string>
#include <vector>
#include <lua5.3/lua.h>
#include <Box2D/Box2D.h>
#include <MicroPather/micropather.h>
#include "util/ISerializable.h"
#include "Map.h"
#include "ui/UI.h"
#include "Entity.h"

namespace game {

    class Game;

    void luai_State_Register(lua_State *lua);

    class State : public util::ISerializable, public b2ContactListener {
        public:
            void Init(const std::string &name, Game *game);
            void Shutdown();

            void ChangeMap(const std::string &name);

            void RunFrame(graphics::Renderer *renderer, int elapsedMs);

            b2Body *CreateBody(b2BodyDef *body);

            void Serialize(util::Serializer *serializer);
            void Deserialize(util::Deserializer *deserializer);

            int SolvePath(float x1, float y1, float x2, float y2, micropather::MPVector<void*> &path);

            void BeginContact(b2Contact *contact);
            void EndContact(b2Contact *contact);

        private:
            void LoadScript(const std::string &path);
            void LoadScripts(const std::string &name);

            void CallFunc(const std::string &name);

            void SpawnEntities();
            void SpawnEntity(map_object_t *object);
            void SpawnWalls();
            void SpawnWall(map_object_t *object);

            Game *game;
            std::string name;
            lua_State *lua;
            Map *map;
            std::string changeMap;
            std::vector<Entity *> entities;
            ui::Screen *ui;
            b2World *physics;
            micropather::MicroPather *pather;
            graphics::Camera camera;
            std::vector<b2Body*> wallBodies;
    };

}

#endif //SGAME_STATE_H
