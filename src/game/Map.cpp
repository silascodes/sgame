#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <json-c/json.h>
#include "Log.h"
#include "util/util.h"
#include "Map.h"
#include "graphics/Graphics.h"
#include "util/Serializer.h"
#include "util/Deserializer.h"
#include "game/State.h"
#include "util/Rect.h"

namespace game {

    void Map::LoadObjects(map_layer_t *layer, json_object *objects) {
        json_object *object;

        layer->numObjects = json_object_array_length(objects);
        layer->objects = (map_object_t *) malloc(sizeof(map_object_t) * layer->numObjects);
        if(!layer->objects) {
            Log_Error("failed to allocate memory for map objects");
            return;
        }

        for(int i = 0; i < layer->numObjects; i++) {
            object = json_object_array_get_idx(objects, i);

            if(json_object_get_type(object) == json_type_object) {
                layer->objects[i].id = Json_GetObjectProp_Int(object, "id", 0);
                layer->objects[i].gid = Json_GetObjectProp_Int(object, "gid", 0);
                layer->objects[i].width = Json_GetObjectProp_Int(object, "width", 0);
                layer->objects[i].height = Json_GetObjectProp_Int(object, "height", 0);
                layer->objects[i].x = Json_GetObjectProp_Int(object, "x", 0);
                layer->objects[i].y = Json_GetObjectProp_Int(object, "y", 0);
                layer->objects[i].visible = Json_GetObjectProp_Bool(object, "visible", 0);
                layer->objects[i].rotation = Json_GetObjectProp_Float(object, "rotation", 0);

                const char *name = Json_GetObjectProp_String(object, "name", "");
                layer->objects[i].name = (char *) malloc(strlen(name));
                if(!layer->objects[i].name) {
                    Log_Error("failed to allocate memory for object name");
                }
                else {
                    strcpy(layer->objects[i].name, name);
                }

                const char *type = Json_GetObjectProp_String(object, "type", "");
                layer->objects[i].type = (char *) malloc(strlen(type));
                if(!layer->objects[i].type) {
                    Log_Error("failed to allocate memory for object type");
                }
                else {
                    strcpy(layer->objects[i].type, type);
                }
            }
        }
    }

    void Map::LoadLayerDataArray(map_layer_t *layer, json_object *array) {
        json_object *entry;
        int entryCount = json_object_array_length(array);

        layer->data = (int *) malloc(sizeof(int) * entryCount);
        if(!layer->data) {
            Log_Error("failed to allocate memory for layer data");
            return;
        }

        for(int i = 0; i < entryCount; i++) {
            entry = json_object_array_get_idx(array, i);
            layer->data[i] = json_object_get_int(entry);
        }
    }

    void Map::LoadLayers(json_object *layers) {
        json_object *layer;
        int errors = 0;

        this->numLayers = json_object_array_length(layers);
        this->layers = (map_layer_t *) malloc(sizeof(map_layer_t) * this->numLayers);
        if(!this->layers) {
            Log_Error("failed to allocate memory for map layers");
            return;
        }

        for(int i = 0; i < this->numLayers; i++) {
            layer = json_object_array_get_idx(layers, i);

            if(json_object_get_type(layer) == json_type_object) {
                errors = 0;

                this->layers[i].width = Json_GetObjectProp_Int(layer, "width", 0);
                this->layers[i].height = Json_GetObjectProp_Int(layer, "height", 0);
                this->layers[i].x = Json_GetObjectProp_Int(layer, "x", 0);
                this->layers[i].y = Json_GetObjectProp_Int(layer, "y", 0);
                this->layers[i].offsetX = Json_GetObjectProp_Int(layer, "offsetX", 0);
                this->layers[i].offsetY = Json_GetObjectProp_Int(layer, "offsetY", 0);
                this->layers[i].visible = Json_GetObjectProp_Bool(layer, "visible", 0);
                this->layers[i].opacity = Json_GetObjectProp_Float(layer, "opacity", 0);

                const char *name = Json_GetObjectProp_String(layer, "name", "");
                this->layers[i].name = (char *) malloc(strlen(name) + 1);
                if(!this->layers[i].name) {
                    Log_Error("failed to allocate memory for layer name");
                    errors++;
                }
                else {
                    memset(this->layers[i].name, 0, strlen(name) + 1);
                    strcpy(this->layers[i].name, name);
                }

                const char *type = Json_GetObjectProp_String(layer, "type", "");
                if(strcmp("tilelayer", type) == 0) {
                    this->layers[i].type = MAP_LAYER_TILE;
                }
                else if(strcmp("objectgroup", type) == 0) {
                    this->layers[i].type = MAP_LAYER_OBJECT;
                }
                else if(strcmp("imagelayer", type) == 0) {
                    this->layers[i].type = MAP_LAYER_IMAGE;
                }
                else {
                    Log_Error("unknown map layer type %s", type);
                    errors++;
                }

                const char *drawOrder = Json_GetObjectProp_String(layer, "draworder", "");
                if(strcmp("topdown", drawOrder) == 0) {
                    this->layers[i].drawOrder = MAP_OBJECT_TOP_DOWN;
                }
                else if(strcmp("index", drawOrder) == 0) {
                    this->layers[i].drawOrder = MAP_OBJECT_INDEX;
                }
                else if(strlen(drawOrder) > 0) {
                    Log_Error("unknown map layer draw order %s", drawOrder);
                    errors++;
                }

                const char *compression = Json_GetObjectProp_String(layer, "compression", "");
                if(strcmp("zlib", compression) == 0) {
                    this->layers[i].compression = MAP_LAYER_ZLIB;
                }
                else if(strcmp("gzip", compression) == 0) {
                    this->layers[i].compression = MAP_LAYER_GZIP;
                }
                else if(strlen(compression) > 0) {
                    Log_Error("unknown map layer compression %s", compression);
                    errors++;
                }

                const char *encoding = Json_GetObjectProp_String(layer, "encoding", "");
                if(strcmp("base64", encoding) == 0) {
                    this->layers[i].encoding = MAP_LAYER_BASE64;
                }
                else if(strlen(encoding) > 0) {
                    Log_Error("unknown map layer encoding %s", encoding);
                    errors++;
                }

                if(this->layers[i].type == MAP_LAYER_TILE) {
                    json_object *val = Json_GetObjectProp(layer, "data");
                    if(json_object_get_type(val) == json_type_string) {
                        const char *data = json_object_get_string(val);
                        this->layers[i].encodedData = (char *) malloc(json_object_get_string_len(val));
                        strcpy(this->layers[i].encodedData, data);
                    }
                    else if(json_object_get_type(val) == json_type_array) {
                        this->LoadLayerDataArray(&this->layers[i], val);
                    }
                    else {
                        Log_Error("map layer data is of unexpected type %s",
                                  json_type_to_name(json_object_get_type(val)));
                        errors++;
                    }
                }
                else if(this->layers[i].type == MAP_LAYER_OBJECT) {
                    json_object *objects = Json_GetObjectProp_Array(layer, "objects", NULL);
                    if(objects) {
                        this->LoadObjects(&this->layers[i], objects);
                    }
                    else {
                        Log_Error("no map layer objects found");
                        errors++;
                    }
                }

                if(errors > 0) {
                    // TODO: error!
                }
            }
        }
    }

    void Map::LoadTerrains(map_tileset_t *tileset, json_object *terrains) {
        json_object *terrain;
        int terrainCount = json_object_array_length(terrains);

        tileset->terrains = (map_terrain_t *) malloc(sizeof(map_terrain_t) * terrainCount);
        if(!tileset->terrains) {
            Log_Error("failed to allocate memory for map tilesets");
            return;
        }

        for(int i = 0; i < terrainCount; i++) {
            terrain = json_object_array_get_idx(terrains, i);

            if(json_object_get_type(terrain) == json_type_object) {
                tileset->terrains[i].tile = Json_GetObjectProp_Int(terrain, "tile", 0);

                const char *name = Json_GetObjectProp_String(terrain, "name", "");
                tileset->terrains[i].name = (char *) malloc(strlen(name));
                if(!tileset->terrains[i].name) {
                    Log_Error("failed to allocate memory for terrain name");
                }
                else {
                    strcpy(tileset->terrains[i].name, name);
                }
            }
        }
    }

    void Map::LoadTilesets(json_object *tilesets) {
        json_object *tileset;

        this->numTilesets = json_object_array_length(tilesets);
        this->tilesets = (map_tileset_t *) malloc(sizeof(map_tileset_t) * this->numTilesets);
        if(!this->tilesets) {
            Log_Error("failed to allocate memory for map tilesets");
            return;
        }

        for(int i = 0; i < this->numTilesets; i++) {
            tileset = json_object_array_get_idx(tilesets, i);

            if(json_object_get_type(tileset) == json_type_object) {
                this->tilesets[i].firstGid = Json_GetObjectProp_Int(tileset, "firstgid", 0);
                this->tilesets[i].tileCount = Json_GetObjectProp_Int(tileset, "tilecount", 0);
                this->tilesets[i].tileWidth = Json_GetObjectProp_Int(tileset, "tilewidth", 0);
                this->tilesets[i].tileHeight = Json_GetObjectProp_Int(tileset, "tileheight", 0);
                this->tilesets[i].imageWidth = Json_GetObjectProp_Int(tileset, "imagewidth", 0);
                this->tilesets[i].imageHeight = Json_GetObjectProp_Int(tileset, "imageheight", 0);
                this->tilesets[i].margin = Json_GetObjectProp_Int(tileset, "margin", 0);
                this->tilesets[i].spacing = Json_GetObjectProp_Int(tileset, "spacing", 0);
                this->tilesets[i].columns = Json_GetObjectProp_Int(tileset, "columns", 0);

                const char *image = Json_GetObjectProp_String(tileset, "image", "");
                this->tilesets[i].image = (char *) malloc(strlen(image));
                if(!this->tilesets[i].image) {
                    Log_Error("failed to allocate memory for tileset image");
                }
                else {
                    strcpy(this->tilesets[i].image, image);
                }

                const char *name = Json_GetObjectProp_String(tileset, "name", "");
                this->tilesets[i].name = (char *) malloc(strlen(name));
                if(!this->tilesets[i].name) {
                    Log_Error("failed to allocate memory for tileset name");
                }
                else {
                    strcpy(this->tilesets[i].name, name);
                }

                json_object *terrains = Json_GetObjectProp_Array(tileset, "terrains", NULL);
                if(terrains) {
                    this->LoadTerrains(&this->tilesets[i], terrains);
                }
                else {
                    Log_Error("no map tilesets found");
                }
            }
        }
    }

    void Map::LoadData(json_object *root) {
        int mapErrors = 0;

        this->version = Json_GetObjectProp_Int(root, "version", 0);
        this->nextObjectId = Json_GetObjectProp_Int(root, "nextobjectid", 0);

        // Map width (in tiles, int)
        this->width = Json_GetObjectProp_Int(root, "width", 0);
        if(this->width < 1) {
            Log_Error("map width is %d, minimum map width is 1", this->width);
            mapErrors++;
        }

        // Map height (in tiles, int)
        this->height = Json_GetObjectProp_Int(root, "height", 0);
        if(this->height < 1) {
            Log_Error("map height is %d, minimum map height is 1", this->height);
            mapErrors++;
        }

        // Tile width (in pixels, int)
        this->tileWidth = Json_GetObjectProp_Int(root, "tilewidth", 0);
        if(this->tileWidth < 1) {
            Log_Error("map tile width is %d, minimum map tile width is 1", this->tileWidth);
            mapErrors++;
        }

        // Tile height (in pixels, int)
        this->tileHeight = Json_GetObjectProp_Int(root, "tileheight", 0);
        if(this->tileHeight < 1) {
            Log_Error("map tile height is %d, minimum map tile height is 1", this->tileHeight);
            mapErrors++;
        }

        // Map orientation (orthogonal/isometric/etc, string, convert to int)
        const char *orientation = Json_GetObjectProp_String(root, "orientation", "");
        if(strcmp("orthogonal", orientation) == 0) {
            this->orientation = MAP_ORTHOGONAL;
        }
        else if(strcmp("isometric", orientation) == 0) {
            this->orientation = MAP_ISOMETRIC;
        }
        else if(strcmp("staggered", orientation) == 0) {
            this->orientation = MAP_STAGGERED;
        }
        else if(strcmp("hexagonal", orientation) == 0) {
            this->orientation = MAP_HEXAGONAL;
        }
        else {
            Log_Error("unknown map orientation %s", this->orientation);
            mapErrors++;
        }

        // Map render order (right-down/left-up/etc, string, convert to int)
        const char *renderOrder = Json_GetObjectProp_String(root, "renderorder", "");
        if(strcmp("right-down", renderOrder) == 0) {
            this->renderOrder = MAP_RIGHT_DOWN;
        }
        else if(strcmp("right-up", renderOrder) == 0) {
            this->renderOrder = MAP_RIGHT_UP;
        }
        else if(strcmp("left-down", renderOrder) == 0) {
            this->renderOrder = MAP_LEFT_DOWN;
        }
        else if(strcmp("left-up", renderOrder) == 0) {
            this->renderOrder = MAP_LEFT_UP;
        }
        else {
            Log_Error("unknown map render order %s", this->renderOrder);
            mapErrors++;
        }

        // Map background colour
        const char *colorString = Json_GetObjectProp_String(root, "backgroundcolor", "#000000");
        if(strlen(colorString) == 7) {
            char color[3] = {0, 0, 0};

            color[0] = colorString[1];
            color[1] = colorString[2];
            this->bgR = (unsigned char) strtol(color, NULL, 16);

            color[0] = colorString[3];
            color[1] = colorString[4];
            this->bgG = (unsigned char) strtol(color, NULL, 16);

            color[0] = colorString[5];
            color[1] = colorString[6];
            this->bgB = (unsigned char) strtol(color, NULL, 16);
        }

        // Map layers
        json_object *layers = Json_GetObjectProp_Array(root, "layers", NULL);
        if(layers) {
            this->LoadLayers(layers);
        }
        else {
            Log_Error("no map layers found");
            mapErrors++;
        }

        // Map tilesets
        json_object *tilesets = Json_GetObjectProp_Array(root, "tilesets", NULL);
        if(tilesets) {
            this->LoadTilesets(tilesets);
        }
        else {
            Log_Error("no map tilesets found");
            mapErrors++;
        }

        /*if(strcmp("properties", key) == 0) {
            Log_Warning("TODO: implement map properties");
        }
        else if(strcmp("propertytypes", key) == 0) {
            Log_Warning("TODO: implement map propertytypes");
        }*/

        if(mapErrors > 0) {
            Log_Error("map contains errors, loading failed");
            //        Map_Free(map);
        }
    }

    int Map_Tileset_GetOffsetX(map_tileset_t *tileset, int tile) {
        int tilesPerRow = tileset->imageWidth / tileset->tileWidth;
        int x = tile % tilesPerRow;
        return (x - 1) * tileset->tileWidth;
    }

    int Map_Tileset_GetOffsetY(map_tileset_t *tileset, int tile) {
        int tilesPerRow = tileset->imageWidth / tileset->tileWidth;
        int y = tile / tilesPerRow;
        return y * tileset->tileWidth;
    }

    void Map::Init(const std::string &name, State *state) {
        Log_Info(name.c_str());

        this->name = name;

        // Load the entire map JSON file into memory
        char *mapJsonRaw = util::FileToString(name);
        if(!mapJsonRaw) {
            Log_Error("failed to load map from file");
            return;
        }

        // DEBUG:
        //printf("%s\n", mapJsonRaw);

        // Parse the raw JSON
        json_object *mapJson = json_tokener_parse(mapJsonRaw);
        free(mapJsonRaw);
        if(!mapJson) {
            Log_Error("failed to parse map");
            return;
        }

        // Pass the parsed root object off to our loading functions
        this->LoadData(mapJson);

        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(-10000.0f, -10000.0f);

        this->groundBody = state->CreateBody(&bodyDef);

        b2PolygonShape boxDef;
        boxDef.SetAsBox(10000.0f, 10000.0f);

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &boxDef;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 1.0f;

        this->groundBody->CreateFixture(&fixtureDef);
    }

    void Map::Shutdown() {

    }

    void Map::Render(graphics::Renderer *renderer) {
        util::Vector2 cam = renderer->GetCamera()->GetPosition();

        // draw frame

        graphics::Texture *tiles = renderer->LoadTexture("data/tilesets/map2.png");

        for(int i = 0; i < this->numLayers; i++) {
            if(this->layers[i].type == MAP_LAYER_TILE) {
                for(int y = 0; y < this->height; y++) {
                    for(int x = 0; x < this->width; x++) {
                        int tile = this->layers[i].data[y * this->width + x];
                        SDL_Rect src, dst;

                        src.x = Map_Tileset_GetOffsetX(&this->tilesets[0], tile);
                        src.y = Map_Tileset_GetOffsetY(&this->tilesets[0], tile);
                        src.w = this->tilesets[0].tileWidth;
                        src.h = this->tilesets[0].tileHeight;

                        dst.x = x * this->tilesets[0].tileWidth;
                        dst.y = y * this->tilesets[0].tileHeight;
                        dst.w = src.w;
                        dst.h = src.h;

                        dst.x -= cam.x;
                        dst.y -= cam.y;

                        tiles->Render(&src, &dst);
                    }
                }
            }
        }
    }

    void Map::Serialize(util::Serializer *serializer) {
        serializer->WriteString(this->name);
    }

    void Map::Deserialize(util::Deserializer *deserializer) {
        std::string name;
        deserializer->ReadString(name);
        //this->Init(name);
    }

    map_layer_t *Map::GetLayers() {
        return this->layers;
    }

    int Map::GetNumLayers() {
        return this->numLayers;
    }

    float Map::LeastCostEstimate(void *stateStart, void *stateEnd) {
        intptr_t index1 = (intptr_t)stateStart;
        int y1 = index1 / (this->layers[0].width * 32);
        int x1 = index1 - y1 * (this->layers[0].width * 32);
        util::Vector2 start(x1, y1);

        intptr_t index2 = (intptr_t)stateEnd;
        int y2 = index2 / (this->layers[0].width * 32);
        int x2 = index2 - y2 * (this->layers[0].width * 32);
        util::Vector2 end(x2, y2);

        util::Vector2 diff = start - end;

        return diff.Magnitude();
    }

    void Map::AdjacentCost(void *state, MP_VECTOR<micropather::StateCost> *adjacent) {
        const int dx[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
        const int dy[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };

        intptr_t index = (intptr_t)state;
        int y = index / (this->layers[0].width * 32);
        int x = index - y * (this->layers[0].width * 32);

        for(int i = 0; i < 8; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            util::Rect moverRect(nx - 16.0f, ny - 16.0f, 32.0f, 32.0f);

            int pass = 1;
            if(nx < 0 || nx >= (this->layers[0].width * 32)) {
                pass = 0;
            }
            if(ny < 0 || ny >= (this->layers[0].height * 32)) {
                pass = 0;
            }

            for(int i = 0; pass && i < this->numLayers; i++) {
                if(this->layers[i].type == MAP_LAYER_OBJECT && strcmp(this->layers[i].name, "collision") == 0) {
                    for(int j = 0; pass && j < this->layers[i].numObjects; j++) {
                        map_object_t *obj = &this->layers[i].objects[j];
                        util::Rect objBox(obj->x, obj->y, obj->width, obj->height);
                        /*if(nx >= x1 && nx <= x2 && ny >= y1 && ny <= y2) {
                            pass = 0;
                        }*/
                        if(objBox.OverlapsRect(moverRect)) {
                            pass = 0;
                        }
                    }
                }
            }

            if(pass > 0) {
                float cost = this->LeastCostEstimate((void*)(y * (this->layers[0].width * 32) + x), (void*)(ny * (this->layers[0].width * 32) + nx));
                micropather::StateCost nodeCost = { (void*)(ny * (this->layers[0].width * 32) + nx), cost };
                adjacent->push_back(nodeCost);
            }
        }
    }

    void Map::PrintStateInfo(void *state) {

    }

}
