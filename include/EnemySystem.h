#pragma once

#include "ECS.h"
#include <Components.h>
#include "Collision.h"

class EnemySystem : public System {

public:
    explicit EnemySystem() = default;

    void AddedToWorld(World* world) override;

    void tick(World* world) override;

    void RemovedFromWorld(World* world) override;

    ~EnemySystem() override = default;
};

