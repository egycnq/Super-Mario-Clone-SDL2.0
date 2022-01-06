#pragma once

#include "ECS.h"
#include "Components.h"
#include "Collision.h"

class FlagSystem : public System {

public:
    explicit FlagSystem(std::function<void(void)> gameOverCallback);

    void AddedToWorld(World* world) override;

    ~FlagSystem() override = default;
private:
    void handleEvent(SDL_Event& event) override;

    void RemovedFromWorld(World* world) override;
    std::function<void(void)> gameOverCallback;
    void tick(World* world) override;


};