#pragma once

#include "ECS.h"
#include <queue>
#include "Components.h"
#include <fstream>
#include <exception>
#include "Constants.h"

struct Tile {
    int x, y, w, h;
    TextureId textureId;
    uint32_t properties;

    bool hasProperty(Properties property) const {
        return properties & property;
    }
};

class Map {

public:
    Map() = delete;

    explicit Map(std::string&& mapPath) {
        std::ifstream infile(mapPath, std::ios::out | std::ios::binary);
        if (!infile) throw std::invalid_argument("Map path not found: " + mapPath);

        TileType texture;

        infile.read(reinterpret_cast<char*>(&mapWidth), sizeof(uint16_t));
        infile.read(reinterpret_cast<char*>(&mapHeight), sizeof(uint16_t));

        for (int x = 0; x < mapWidth; x++) {
            for (int y = 0; y < mapHeight; y++) {
                infile.read(reinterpret_cast<char*>(&texture), sizeof(TileType));
                if (texture.texture != 0) {
                    auto tile = new Tile{x * TILE_SIZE,
                                         y * TILE_SIZE,
                                         TILE_SIZE,
                                         TILE_SIZE,
                                         texture.texture,
                                         texture.properties
                    };
                    tiles.emplace(tile);
                }
            }
        }

        infile.close();
    }

    uint16_t mapWidth{};
    uint16_t mapHeight{};
    std::queue<Tile*> tiles;
};

class MapSystem : public System {
public:

    MapSystem(std::string mapPath = "assets/world1-1");

    void AddedToWorld(World* world) override;

    void tick(World* world) override;

    void RemovedFromWorld(World* world) override;

    ~MapSystem() override;

private:
    Map map;
    CameraComponent* camera{};
};