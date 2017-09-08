#include <Box2D/Box2D.h>
#include <lua5.3/lua.h>
#include <LuaWrapper/luawrapper.h>
#include "Entity.h"
#include "State.h"
#include "Log.h"
#include "util/util.h"

namespace game {

    void Entity::Init(int entRef, map_object_t *object, State *state, lua_State *lua) {
        this->state = state;
        this->lua = lua;

        this->name = object->name;

        this->ref = entRef;

        this->sprite = NULL;

        this->position.x = object->x;
        this->position.y = object->y;
        this->width = object->width;
        this->height = object->height;
        this->moveDestination.x = this->position.x;
        this->moveDestination.y = this->position.y;

        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(this->position.x, this->position.y);

        this->physicsBody = state->CreateBody(&bodyDef);

        b2PolygonShape boxDef;
        boxDef.SetAsBox(this->width / 2.0f, this->height / 2.0f);

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &boxDef;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.5f;

        this->physicsBody->CreateFixture(&fixtureDef);

        this->still = true;
        this->moving = false;
        this->moveState = MOVE_STATE_STATIONARY;
        this->moveTargetVelocity.x = 0;
        this->moveTargetVelocity.y = 0;

        this->speed = 0.0f;
        this->targetSpeed = 0.0f;
        /*this->maxSpeed = 1.2f;
        this->accelerationRate = 0.6f;
        this->decelerationRate = 0.6f;*/
        /*this->maxSpeed = 40.0f;
        this->accelerationRate = 20.0f;
        this->decelerationRate = 20.0f;*/
        this->maxSpeed = 40.0f * 32.0f;
        this->accelerationRate = 20.0f * 32.0f;
        this->decelerationRate = 20.0f * 32.0f;
    }

    void Entity::Shutdown() {

    }

    void Entity::Think(int elapsedMs) {
        b2Vec2 pos = this->physicsBody->GetPosition();
        this->position.x = pos.x;
        this->position.y = pos.y;
        float secFraction = elapsedMs / 1000.0f;

        this->thinkTime -= elapsedMs;

        if(this->moving) {
            int x = 0;
            int y = 0;
            if(this->path.size() > 0) {
                intptr_t index = (intptr_t) this->path[this->nextPathPoint];
                y = index / (100 * 32);
                x = index - y * (100 * 32);
            }

            float radius = 0.5f;
            util::Vector2 ccenter(this->position);
            ccenter -= util::Vector2(x, y);
            util::Vector2 cdest(this->position);
            cdest -= this->moveDestination;

            /*if(FLOAT_EQUAL(this->position.x, this->moveDestination.x) &&
               FLOAT_EQUAL(this->position.y, this->moveDestination.y)) {*/
            if(cdest.Magnitude() <= radius) {
                Log_Info("Hit destination");
                this->speed = 0;
                this->targetSpeed = 0;
                this->moveState = MOVE_STATE_STATIONARY;
                this->physicsBody->SetLinearVelocity(b2Vec2(0, 0));
                this->physicsBody->SetAngularVelocity(0);
                this->moving = false;
                this->path.clear();

                std::map<std::string, std::string>::iterator eventIter = this->eventHandlers.find("ReachedDestination");
                if(eventIter != this->eventHandlers.end()) {
                    lua_rawgeti(this->lua, LUA_REGISTRYINDEX, this->ref);
                    lua_getfield(this->lua, -1, eventIter->second.c_str());
                    if(lua_isfunction(this->lua, -1) != 1) {
                        Log_Error("entity %s ReachedDestination event handler function not found", this->name.c_str());
                        return;
                    }

                    lua_rawgeti(this->lua, LUA_REGISTRYINDEX, this->ref);
                    if(lua_pcall(this->lua, 1, 0, 0) != LUA_OK) {
                        Log_Error("failed to call entity %s ReachedDestination event handler function (%s)",
                                  this->name.c_str(),
                                  lua_tostring(this->lua, -1));
                        return;
                    }
                }
            }
                /*else if(FLOAT_EQUAL(this->position.x, x) &&
                   FLOAT_EQUAL(this->position.y, y)) {*/
            else if(ccenter.Magnitude() <= radius) {
                Log_Info("Hit path waypoint");
                if(this->nextPathPoint < this->path.size() - 1) {
                    this->nextPathPoint++;
                }
            }
            else {
                switch(this->moveState) {
                    case MOVE_STATE_ACCELERATING:
                        this->speed += (this->accelerationRate * secFraction);
                        if(this->speed >= this->targetSpeed) {
                            this->speed = this->targetSpeed;
                            this->moveState = MOVE_STATE_CRUISING;
                            Log_Info("Accelerating > Cruising");
                        }
                        break;

                    case MOVE_STATE_DECELERATING:
                        this->speed -= (this->decelerationRate * secFraction);
                        if(this->speed <= this->targetSpeed) {
                            this->speed = this->targetSpeed;
                            if(this->targetSpeed > 0.0f) {
                                this->moveState = MOVE_STATE_CRUISING;
                                Log_Info("Decelerating > Cruising");
                            }
                            else {
                                this->moveState = MOVE_STATE_STATIONARY;
                                this->physicsBody->SetLinearVelocity(b2Vec2(0, 0));
                                this->physicsBody->SetAngularVelocity(0);
                                this->speed = 0;
                                this->targetSpeed = 0;
                                this->moving = false;
                                Log_Info("Decelerating > Stationary");
                            }
                        }
                        break;

                    case MOVE_STATE_CRUISING:
                        // if its time to start decelerating, do it
                        break;

                    case MOVE_STATE_STATIONARY:
                        this->targetSpeed = this->maxSpeed;
                        this->moveState = MOVE_STATE_ACCELERATING;
                        this->path.clear();
                        this->state->SolvePath(this->position.x, this->position.y, this->moveDestination.x,
                                               this->moveDestination.y, this->path);
                        this->nextPathPoint = 0;
                        Log_Info("Stationary > Accelerating");
                        break;
                }
            }

            //util::Vector2 desiredVelocity = this->position - this->moveDestination;
            util::Vector2 desiredVelocity = this->position - util::Vector2(x, y);
            desiredVelocity.Normalize();
            desiredVelocity = -desiredVelocity;

            util::Vector2 stopVec = this->position - this->moveDestination;
            float distanceLeft = stopVec.Magnitude();
            stopVec.Normalize();
            stopVec = -stopVec;

            float stopDistance = 0.0f;
            float sdSpeed = this->speed;
            while(sdSpeed > 0.0f) {
                sdSpeed -= this->decelerationRate;
                if(sdSpeed > 0.0f) {
                    util::Vector2 distVec = stopVec * sdSpeed;
                    stopDistance += distVec.Magnitude();
                }
            }

            /*if(stopDistance >= distanceLeft) {
                this->moveState = MOVE_STATE_DECELERATING;
                this->targetSpeed = 0.0f;
            }*/

            /*util::Vector2 curVel = this->physicsBody->GetLinearVelocity();
            curVel.Normalize();
            curVel = curVel * this->speed;*/

            desiredVelocity = desiredVelocity * (this->speed * secFraction);
            this->physicsBody->SetLinearVelocity(desiredVelocity.GetBox2D());
            //this->position = this->position + desiredVelocity;

            /*switch(this->moveState) {
                case MOVE_STATE_STATIONARY:
                    this->moveTargetVelocity = 0.0f;
                    break;

                case MOVE_STATE_ACCELERATING:
                    this->moveTargetVelocity = this->moveTargetVelocity + 0.1f;
                    if(this->moveTargetVelocity.x > 10.0f) {
                        this->moveTargetVelocity.x = 0.0f;
                    }
                    if(this->moveTargetVelocity.y > 10.0f) {
                        this->moveTargetVelocity.y = 0.0f;
                    }
                    if(this->moveTargetVelocity.x == 10.0f && this->moveTargetVelocity.y == 10.0f) {
                        this->moveState = MOVE_STATE_CRUISING;
                    }
                    break;

                case MOVE_STATE_DECELERATING:
                    this->moveTargetVelocity = this->moveTargetVelocity - 0.1f;
                    if(this->moveTargetVelocity.x < 0.0f) {
                        this->moveTargetVelocity.x = 0.0f;
                    }
                    if(this->moveTargetVelocity.y < 0.0f) {
                        this->moveTargetVelocity.y = 0.0f;
                    }
                    if(this->moveTargetVelocity.x == 0.0f && this->moveTargetVelocity.y == 0.0f) {
                        this->moveState = MOVE_STATE_STATIONARY;
                    }
                    break;

                case MOVE_STATE_CRUISING:
                    this->moveTargetVelocity = 0.0f;
                    break;
            }

            if(this->moveTargetVelocity.x != 0.0f || this->moveTargetVelocity.y != 0.0f) {
                float mass = this->physicsBody->GetMass();
                util::Vector2 curVel = this->physicsBody->GetLinearVelocity();
                util::Vector2 moveVec;
                moveVec.x = this->moveTargetVelocity.x - curVel.x;
                moveVec.y = this->moveTargetVelocity.y - curVel.y;
                moveVec = moveVec * mass;

                this->physicsBody->ApplyLinearImpulse(moveVec.GetBox2D(), this->physicsBody->GetWorldCenter(), true);
            }*/

            /*if(!FLOAT_EQUAL(this->position.x, this->moveDestination.x) ||
               !FLOAT_EQUAL(this->position.y, this->moveDestination.y)) {
                if(this->still) {
                    Log_Info("%d is moving again", this->ref);
                    this->still = false;
                }

                float target = 0.1f;
                float mass = this->physicsBody->GetMass();
                util::Vector2 curVel = this->physicsBody->GetLinearVelocity();
                util::Vector2 moveVec = ((this->position + curVel) - this->moveDestination) * mass;
                moveVec.Normalize();
                moveVec = -moveVec;

                b2Vec2 mv;
                mv.x = this->moveTargetVelocity.x - curVel.x;
                mv.y = this->moveTargetVelocity.y - curVel.y;

                this->physicsBody->ApplyLinearImpulse(mv, this->physicsBody->GetWorldCenter(), true);
            }
            else {
                this->physicsBody->SetLinearVelocity(b2Vec2(0, 0));
                this->physicsBody->SetAngularVelocity(0);
                this->position = this->moveDestination;
                if(!this->still) {
                    this->still = true;
                    this->moving = false;
                    Log_Info("%d has come to a stop", this->ref);
                }
            }*/
        }

        if(this->sprite) {
            this->sprite->SetPosition(this->position.x, this->position.y);
            this->sprite->Update(elapsedMs);
        }

        if(this->thinkTime <= 0) {
            lua_rawgeti(this->lua, LUA_REGISTRYINDEX, this->ref);
            lua_getfield(this->lua, -1, "Think");
            if(lua_isfunction(this->lua, -1) != 1) {
                Log_Error("entity %s Think function not found", this->name.c_str());
                return;
            }

            lua_rawgeti(this->lua, LUA_REGISTRYINDEX, this->ref);
            lua_pushinteger(this->lua, elapsedMs);
            if(lua_pcall(this->lua, 2, 0, 0) != LUA_OK) {
                Log_Error("failed to call entity %s Think function (%s)", this->name.c_str(),
                          lua_tostring(this->lua, -1));
                return;
            }
        }
    }

    void Entity::Render(graphics::Renderer *renderer) {
        if(!this->sprite && !this->spriteName.empty()) {
            this->sprite = new Sprite();
            this->sprite->Init(this->spriteName, renderer);
        }

        if(this->sprite) {
            this->sprite->Render(renderer);
        }

        renderer->SetDrawColor(0xff, 0xff, 0xff, 0xff);
        renderer->DrawLine(this->position.x, this->position.y, this->moveDestination.x, this->moveDestination.y);

        if(this->path.size() > 0) {
            int lastx = -1;
            int lasty = -1;
            for(int i = 0; i < this->path.size(); i++) {
                intptr_t index = (intptr_t) this->path[i];
                int y = index / (100 * 32);
                int x = index - y * (100 * 32);
                if(lastx == -1) {
                    lastx = x;
                }
                if(lasty == -1) {
                    lasty = y;
                }
                renderer->SetDrawColor(0xff, 0x00, 0x00, 0xff);
                renderer->DrawLine(lastx, lasty, x, y);
                if(i == this->nextPathPoint) {
                    renderer->SetDrawColor(0xff, 0xff, 0x00, 0xff);
                }
                else {
                    renderer->SetDrawColor(0x00, 0xff, 0x00, 0xff);
                }
                renderer->DrawFilledRect(x - 1, y - 1, 2, 2);
                lastx = x;
                lasty = y;
            }
        }

        renderer->SetDrawColor(0xff, 0x00, 0xff, 0xff);
        renderer->DrawFilledRect(this->position.x - 1, this->position.y - 1, 2, 2);
        renderer->DrawFilledRect(this->physicsBody->GetPosition().x - 1, this->physicsBody->GetPosition().y - 1, 2, 2);

        /*b2Fixture *fixture = this->physicsBody->GetFixtureList();
        int x = this->position.x;
        int y = this->position.y;
        while(fixture) {
            b2PolygonShape *shape = (b2PolygonShape*)fixture->GetShape();
            for(int i = 0; i < shape->GetVertexCount(); i++) {
                b2Vec2 vert = shape->GetVertex(i);
                renderer->DrawLine(x, y, vert.x, vert.y);
                x = vert.x;
                y = vert.y;
            }
            fixture = fixture->GetNext();
        }*/

        b2Fixture *fixture = this->physicsBody->GetFixtureList();
        int x = this->position.x;
        int y = this->position.y;
        while(fixture) {
            b2AABB aabb = fixture->GetAABB(0);
            b2Vec2 c = aabb.GetCenter();
            b2Vec2 e = aabb.GetExtents();
            renderer->DrawLine(c.x - e.x, c.y - e.y, c.x + e.x, c.y - e.y);
            renderer->DrawLine(c.x + e.x, c.y - e.y, c.x + e.x, c.y + e.y);
            renderer->DrawLine(c.x + e.x, c.y + e.y, c.x - e.x, c.y + e.y);
            renderer->DrawLine(c.x - e.x, c.y + e.y, c.x - e.x, c.y - e.y);

            fixture = fixture->GetNext();
        }
    }

    int Entity::GetEntityRef() {
        return ref;
    }

    void Entity::SetSprite(const std::string &name) {
        if(this->sprite) {
            delete this->sprite;
        }

        this->spriteName = name;
    }

    void Entity::SetMoveDestination(float x, float y) {
        this->moveDestination.x = x;
        this->moveDestination.y = y;

        this->moving = true;
        /*this->still = false;

        this->moveState = MOVE_STATE_ACCELERATING;
        this->moveTargetVelocity = this->position - this->moveDestination;
        this->moveTargetVelocity.Normalize();
        this->moveTargetVelocity = -this->moveTargetVelocity;
        this->moveTargetVelocity = this->moveTargetVelocity * 5.0f;*/
    }

    void Entity::SetThinkTime(int ms) {
        this->thinkTime = ms;
    }

    void Entity::RegisterEventHandler(const std::string &name, const std::string &func) {
        this->eventHandlers[name] = func;
    }

    void Entity::Serialize(util::Serializer *serializer) {

    }

    void Entity::Deserialize(util::Deserializer *deserializer) {

    }

}
