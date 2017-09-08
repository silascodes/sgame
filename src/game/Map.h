#ifndef SGAME_MAP_H
#define SGAME_MAP_H

#include <MicroPather/micropather.h>
#include <Box2D/Box2D.h>
#include <json-c/json.h>
#include "util/ISerializable.h"
#include "graphics/Graphics.h"

// Map orientations
#define MAP_ORTHOGONAL      1
#define MAP_ISOMETRIC       2
#define MAP_STAGGERED       3
#define MAP_HEXAGONAL       4

// Map render orders
#define MAP_RIGHT_DOWN      1
#define MAP_RIGHT_UP        2
#define MAP_LEFT_DOWN       3
#define MAP_LEFT_UP         4

// Map layer types
#define MAP_LAYER_TILE      1
#define MAP_LAYER_OBJECT    2
#define MAP_LAYER_IMAGE     3

// Map layer compression types
#define MAP_LAYER_ZLIB      1
#define MAP_LAYER_GZIP      2

// Map layer encoding types
#define MAP_LAYER_BASE64    1

// Map object draw orders
#define MAP_OBJECT_TOP_DOWN 1
#define MAP_OBJECT_INDEX    2

namespace game {

    typedef struct {
        int id;
        int width, height;
        char *name;
        char *type;
        int visible;
        int x, y;
        float rotation;
        int gid;
    } map_object_t;

    typedef struct {
        int width, height;
        char *name;
        int type;
        int visible;
        int x, y;
        int offsetX, offsetY;
        float opacity;
        int drawOrder;
        int compression;
        int encoding;
        char *encodedData;
        int *data;
        int numObjects;
        map_object_t *objects;
    } map_layer_t;

    typedef struct {
        int tile;
        char *name;
    } map_terrain_t;

    typedef struct {

    } map_tile_t;

    typedef struct {
        int firstGid;
        char *image;
        char *name;
        int tileCount;
        int tileWidth, tileHeight;
        int imageWidth, imageHeight;
        int margin;
        int spacing;
        int columns;
        int numTerrains;
        map_terrain_t *terrains;
        int numTiles;
        map_tile_t *tiles;
    } map_tileset_t;

    int Map_Tileset_GetOffsetX(map_tileset_t *tileset, int tile);
    int Map_Tileset_GetOffsetY(map_tileset_t *tileset, int tile);

    class State;

    class Map : public util::ISerializable, public micropather::Graph {
        public:
            void Init(const std::string &name, State *state);
            void Shutdown();

            void Render(graphics::Renderer *renderer);

            void Serialize(util::Serializer *serializer);
            void Deserialize(util::Deserializer *deserializer);

            map_layer_t *GetLayers();
            int GetNumLayers();

            float LeastCostEstimate(void *stateStart, void *stateEnd);
            void AdjacentCost(void *state, MP_VECTOR<micropather::StateCost> *adjacent);
            void PrintStateInfo(void *state);

        private:
            void LoadData(json_object *root);
            void LoadTilesets(json_object *tilesets);
            void LoadTerrains(map_tileset_t *tileset, json_object *terrains);
            void LoadLayers(json_object *layers);
            void LoadLayerDataArray(map_layer_t *layer, json_object *array);
            void LoadObjects(map_layer_t *layer, json_object *objects);

            std::string name;
            int version;
            int width, height;
            int tileWidth, tileHeight;
            int nextObjectId;
            int orientation;
            int renderOrder;
            unsigned char bgR, bgG, bgB;
            int numLayers;
            map_layer_t *layers;
            int numTilesets;
            map_tileset_t *tilesets;
            b2Body *groundBody;
    };

}

#endif //SGAME_MAP_H
