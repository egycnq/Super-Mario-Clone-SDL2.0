#pragma once

#include "Constants.h"
#include "TextureManager.h"

enum Properties {
    NONE = 0,
    VISIBLE = 1,
    SOLID = 2,
    MASS = 4,
    KINETIC = 8,
    BREAKABLE = 16,
    QUESTION = 32,
    SPAWN = 64,
    COIN = 128,
    ENEMY = 256,
    SPAWN_ONE_UP = 512,
    POLE = 1024,
    FLAG = 2048,
};

struct TileType {
    TileType() = default;

    TextureId texture = TextureId::EMPTY;
    TextureId editor_texture = texture;
    uint32_t properties = NONE;
};

