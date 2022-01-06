#pragma once

#include "ECS.h"
#include "Components.h"
#include "Constants.h"
#include "Collision.h"
#include <unordered_set>

class PhysicsSystem : public System {

public:
    explicit PhysicsSystem() = default;

    void AddedToWorld(World* world) override {};

    void tick(World* world) override;

    void RemovedFromWorld(World* world) override {};

    void handleEvent(SDL_Event& event) override {};

private:
};
