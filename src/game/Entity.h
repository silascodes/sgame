#ifndef SGAME_ENTITY_H
#define SGAME_ENTITY_H

#include <lua5.3/lua.h>
#include <Box2D/Box2D.h>
#include <MicroPather/micropather.h>
#include "util/ISerializable.h"
#include "util/Vector2.h"
#include "graphics/Graphics.h"
#include "game/Sprite.h"
#include "Map.h"

#define MOVE_STATE_STATIONARY       0
#define MOVE_STATE_ACCELERATING     1
#define MOVE_STATE_DECELERATING     2
#define MOVE_STATE_CRUISING         3

namespace game {

    void luai_Entity_Register(lua_State *lua);

    class State;

    class Entity : public util::ISerializable {
        public:
            void Init(int entRef, map_object_t *object, State *state, lua_State *lua);
            void Shutdown();

            void Think(int elapsedMs);
            void Render(graphics::Renderer *renderer);

            int GetEntityRef();

            void SetSprite(const std::string &name);
            void SetMoveDestination(float x, float y);
            void SetThinkTime(int ms);

            void RegisterEventHandler(const std::string &name, const std::string &func);

            void Serialize(util::Serializer *serializer);
            void Deserialize(util::Deserializer *deserializer);

        private:
            State *state;
            lua_State *lua;
            std::string name;
            int ref;
            int thinkTime;
            util::Vector2 position;
            float width, height;
            b2Body *physicsBody;
            std::string spriteName;
            game::Sprite *sprite;
            util::Vector2 moveDestination;
            bool still;
            bool moving;
            int moveState;
            util::Vector2 moveTargetVelocity;
            micropather::MPVector<void*> path;
            int nextPathPoint;
            std::map<std::string, std::string> eventHandlers;

            float speed;
            float targetSpeed;
            float maxSpeed;
            float accelerationRate;
            float decelerationRate;
    };

}

#endif //SGAME_ENTITY_H
