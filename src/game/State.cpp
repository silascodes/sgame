#include <dirent.h>
#include <lua5.3/lua.h>
#include <lua5.3/lualib.h>
#include <lua5.3/lauxlib.h>
#include <LuaWrapper/luawrapper.h>
#include "Log.h"
#include "util/util.h"
#include "Game.h"
#include "util/Serializer.h"
#include "util/Deserializer.h"
#include "client/Client.h"

namespace game {

    void State::SpawnEntity(map_object_t *object) {
        std::string typeNewFunc = object->type;

        // Try find the entity type object
        lua_getglobal(this->lua, typeNewFunc.c_str());
        if(lua_istable(this->lua, -1) != 1) {
            Log_Error("entity type %s not found", typeNewFunc.c_str());
            return;
        }

        // Find the entity type's New function
        lua_getfield(this->lua, -1, "New");
        if(lua_isfunction(this->lua, -1) != 1) {
            Log_Error("entity type %s New function not found", typeNewFunc.c_str());
            return;
        }

        // Call the New function on the entity type to create the new entity instance
        lua_getglobal(this->lua, typeNewFunc.c_str());
        if(lua_pcall(this->lua, 1, 1, 0) != LUA_OK) {
            Log_Error("failed to call entity type %s New function (%s)", typeNewFunc.c_str(),
                      lua_tostring(this->lua, -1));
            return;
        }

        // Pop and store the new entity instance
        int ent = luaL_ref(this->lua, LUA_REGISTRYINDEX);

        // Call the new entity instances Spawn function
        lua_rawgeti(this->lua, LUA_REGISTRYINDEX, ent);
        lua_getfield(this->lua, -1, "Spawn");
        if(lua_isfunction(this->lua, -1) != 1) {
            Log_Error("entity type %s Spawn function not found", typeNewFunc.c_str());
            return;
        }

        // Create the C++ side Entity
        Entity *cEnt = new Entity();
        cEnt->Init(ent, object, this, this->lua);
        this->entities.push_back(cEnt);

        // Call the Spawn function on the new entity instance
        lua_rawgeti(this->lua, LUA_REGISTRYINDEX, ent);
        luaW_push<Entity>(this->lua, cEnt);
        if(lua_pcall(this->lua, 2, 0, 0) != LUA_OK) {
            Log_Error("failed to call entity type %s Spawn function (%s)", typeNewFunc.c_str(),
                      lua_tostring(this->lua, -1));
            return;
        }
    }

    void State::SpawnEntities() {
        // Spawn all entities on the entities map layer
        map_layer_t *layers = this->map->GetLayers();
        for(int i = 0; i < this->map->GetNumLayers(); i++) {
            if(layers[i].type == MAP_LAYER_OBJECT && strcmp(layers[i].name, "entities") == 0) {
                for(int j = 0; j < layers[i].numObjects; j++) {
                    this->SpawnEntity(&layers[i].objects[j]);
                }
            }
        }
    }

    void State::SpawnWall(map_object_t *object) {
        b2BodyDef bodyDef;
        bodyDef.type = b2_staticBody;
        bodyDef.position.Set(object->x + (object->width / 2.0f), object->y + (object->height / 2.0f));

        b2Body *body = this->CreateBody(&bodyDef);

        b2PolygonShape boxDef;
        boxDef.SetAsBox(object->width / 2.0f, object->height / 2.0f);

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &boxDef;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.3f;

        body->CreateFixture(&fixtureDef);

        this->wallBodies.push_back(body);
    }

    void State::SpawnWalls() {
        // Spawn a wall static body for each entity on the collision layer
        map_layer_t *layers = this->map->GetLayers();
        for(int i = 0; i < this->map->GetNumLayers(); i++) {
            if(layers[i].type == MAP_LAYER_OBJECT && strcmp(layers[i].name, "collision") == 0) {
                for(int j = 0; j < layers[i].numObjects; j++) {
                    this->SpawnWall(&layers[i].objects[j]);
                }
            }
        }
    }

    void State::LoadScript(const std::string &path) {
        Log_Info(path.c_str());

        char *contents = util::FileToString(path);
        if(!contents) {
            Log_Error("failed to load script file %s", path.c_str());
            return;
        }

        if(luaL_loadstring(this->lua, contents) != LUA_OK) {
            Log_Error("failed to parse script file (%s)", lua_tostring(this->lua, -1));
            return;
        }

        if(lua_pcall(this->lua, 0, 0, 0) != LUA_OK) {
            Log_Error("failed to execute script file (%s)", lua_tostring(this->lua, -1));
            return;
        }
    }

    void State::LoadScripts(const std::string &name) {
        Log_Info(name.c_str());

        // Load entity scripts
        struct dirent **fileList;
        int n;
        n = scandir("data/scripts/entities", &fileList, NULL, alphasort);
        if(n < 0) {
            Log_Error("failed to scan for entity scripts");
            return;
        }
        else {
            while(n--) {
                std::string entityFile = fileList[n]->d_name;
                if(entityFile == "." || entityFile == "..") {
                    continue;
                }
                this->LoadScript("data/scripts/entities/" + entityFile);
                free(fileList[n]);
            }
            free(fileList);
        }

        // Load state script
        this->LoadScript((name + "/state.lua").c_str());
    }

    void State::Init(const std::string &name, Game *game) {
        this->game = game;

        this->name = name;

        this->map = NULL;
        this->changeMap = "";

        b2Vec2 gravity(0.0f, 0.0f);
        this->physics = new b2World(gravity);
        this->physics->SetContactListener(this);

        // Instantiate the UI
        this->ui = new ui::Screen();

        // Set up the new states script environment
        this->lua = luaL_newstate();
        if(!this->lua) {
            Log_Error("failed to load Lua script engine");
            return;
        }
        luaL_openlibs(this->lua);
        luaapi_Log_Init(this->lua);

        client::luai_Register(this->lua);
        luaW_push<client::Client>(this->lua, game->GetClient());
        lua_setglobal(this->lua, "client");

        luai_Game_Register(this->lua);
        luaW_push<Game>(this->lua, game);
        lua_setglobal(this->lua, "game");

        luai_State_Register(this->lua);
        luaW_push<State>(this->lua, this);
        lua_setglobal(this->lua, "state");

        luai_Sprite_Register(this->lua);
        luai_Entity_Register(this->lua);

        ui::luai_Register(this->lua);
        luaW_push<ui::Screen>(this->lua, this->ui);
        lua_setglobal(this->lua, "ui");

        // Initialise the states UI
        this->ui->Init(this->lua);
        game->GetClient()->GetInput()->AddListener(this->ui);

        // Try and load the state scripts
        char buf[1024];
        sprintf(buf, "data/scripts/states/%s", this->name.c_str());
        this->LoadScripts(buf);

        // Try and call the states Init() function
        this->CallFunc("Init");
    }

    void State::Shutdown() {
        if(this->map) {
            this->map->Shutdown();
            delete this->map;
        }

        if(this->physics) {
            delete this->physics;
        }

        this->game->GetClient()->GetInput()->RemoveListener(this->ui);

        if(this->ui) {
            this->ui->Shutdown();
            delete this->ui;
        }

        lua_close(this->lua);
    }

    void State::CallFunc(const std::string &name) {
        lua_getglobal(this->lua, name.c_str());
        if(lua_isfunction(this->lua, -1) != 1) {
            Log_Error("states %s function not found", name.c_str());
            return;
        }
        if(lua_pcall(this->lua, 0, 0, 0) != LUA_OK) {
            Log_Error("failed to call states %s function (%s)", name.c_str(), lua_tostring(this->lua, -1));
            return;
        }
    }

    void State::ChangeMap(const std::string &name) {
        this->changeMap = name;
    }

    void State::RunFrame(graphics::Renderer *renderer, int elapsedMs) {
        // If we need to perform a map change, do it
        if(!this->changeMap.empty()) {
            if(this->map) {
                this->map->Shutdown();
                delete this->map;
            }
            this->map = new Map();
            this->map->Init(("data/maps/" + this->changeMap + ".json").c_str(), this);
            this->changeMap.clear();
            this->SpawnWalls();
            this->SpawnEntities();
            this->pather = new micropather::MicroPather(this->map);   // Although you really should set the default params for your game.
        }

        this->camera.SetPosition(util::Vector2(250, 250));
        renderer->SetCamera(&this->camera);

        // (timestep, velocityIterations, positionIterations)
        this->physics->Step(1.0f / 60.0f, 6, 2);

        for(std::vector<Entity *>::iterator i = this->entities.begin(); i != this->entities.end(); i++) {
            (*i)->Think(elapsedMs);
        }

        this->CallFunc("RunFrame");

        if(this->map) {
            this->map->Render(renderer);

            renderer->SetDrawColor(0x00, 0x00, 0xff, 0xff);
            map_layer_t *layers = this->map->GetLayers();
            for(int i = 0; i < this->map->GetNumLayers(); i++) {
                if(layers[i].type == MAP_LAYER_OBJECT && strcmp(layers[i].name, "collision") == 0) {
                    for(int j = 0; j < layers[i].numObjects; j++) {
                        renderer->DrawRect(layers[i].objects[j].x, layers[i].objects[j].y, layers[i].objects[j].width, layers[i].objects[j].height);
                    }
                }
            }
        }

        renderer->SetDrawColor(0xff, 0x00, 0xff, 0xff);
        for(std::vector<b2Body*>::iterator i = this->wallBodies.begin(); i != this->wallBodies.end(); i++) {
            b2Fixture *fixture = (*i)->GetFixtureList();
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

        for(std::vector<Entity *>::iterator i = this->entities.begin(); i != this->entities.end(); i++) {
            (*i)->Render(renderer);
        }

        if(this->ui) {
            renderer->SetCamera(NULL);
            this->ui->Render(renderer);
        }

        //this->CallFunc("CheckConditions");
    }

    b2Body *State::CreateBody(b2BodyDef *body) {
        return this->physics->CreateBody(body);
    }

    void State::Serialize(util::Serializer *serializer) {
        // Write state name
        serializer->WriteString(this->name);

        // Write map state
        if(this->map) {
            serializer->Serialize(this->map);
        }
        else {
            serializer->WriteString("");
        }

        // Write all the entities
        for(std::vector<Entity *>::iterator i = this->entities.begin(); i != this->entities.end(); i++) {
            serializer->Serialize(*i);
        }
    }

    void State::Deserialize(util::Deserializer *deserializer) {
        // Read state name
        deserializer->ReadString(this->name);


    }

    int State::SolvePath(float x1, float y1, float x2, float y2, micropather::MPVector<void*> &path) {
        float totalCost = 0;
        int result = pather->Solve((void*)((int)y1 * (this->map->GetLayers()[0].width * 32) + (int)x1), (void*)((int)y2 * (this->map->GetLayers()[0].width * 32) + (int)x2), &path, &totalCost);
        for(int i = 0; i < path.size(); i++) {
            intptr_t index = (intptr_t)path[i];
            int y = index / (100 * 32);
            int x = index - y * (100 * 32);
            Log_Info("PATH (x: %d, y: %d)", x, y);
        }
        return result;
    }

    void State::BeginContact(b2Contact *contact) {
        Log_Info("Begin Contact");
    }

    void State::EndContact(b2Contact *contact) {
        Log_Info("End Contact");
    }

}
