#pragma once

#include "ECS.h"
#include "Components.h"

class CallbackSystem : public System {

public:
    void AddedToWorld(World* world) override;

    void tick(World* world) override;

    void handleEvent(SDL_Event& event) override;

    void RemovedFromWorld(World* world) override;

    ~CallbackSystem() override = default;
};