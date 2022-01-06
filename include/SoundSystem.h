#pragma once

#include "ECS.h"
#include <SDL_mixer.h>
#include "SoundManger.h"
#include "Components.h"
#include "memory"

class SoundSystem : public System {

public:
    explicit SoundSystem() = default;

    void AddedToWorld(World* world) override;

    void tick(World* world) override;

    void handleEvent(SDL_Event& event) override;

    void RemovedFromWorld(World* world) override;

    ~SoundSystem() override = default;

private:
    std::shared_ptr<SoundManager> soundManager{};

};