#pragma once

#include "ECS.h"
#include "Components.h"
#include "Constants.h"

class TileSystem : public System {
public:
    TileSystem() = default;

    virtual void AddedToWorld(World *world) override;

    virtual void tick(World *world) override;

    virtual void handleEvent(SDL_Event &event) override;

    virtual void RemovedFromWorld(World *world) override;

    ~TileSystem() override = default;

private:
    TileMapComponent *tileMap{};

};

