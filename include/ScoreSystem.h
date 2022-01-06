#pragma once

#include "ECS.h"
#include "Components.h"
#include "Constants.h"

class ScoreSystem : public System {

public:
    void AddedToWorld(World* world) override;

    void tick(World* world) override;

    void handleEvent(SDL_Event& event) override;

    void RemovedFromWorld(World* world) override;

    ~ScoreSystem() override = default;

private:
    Entity* scoreEntity;
    Entity* coinsEntity;
    Entity* timeLeftEntity;

    int score = 0;
    int coins = 0;
    int time = 255 * 60;
};