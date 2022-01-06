#pragma once


#include "RenderSystem.h"
#include "PhysicsSystem.h"
#include "EnemySystem.h"
#include "TileSystem.h"
#include "CallbackSystem.h"
#include "MapSystem.h"
#include "AnimationSystem.h"
#include "PlayerSystem.h"
#include "SoundSystem.h"
#include "WinPointSystem.h"
#include "ScoreSystem.h"
#include "Constants.h"

class Scene {
protected:
    World* world = new World();

public:

    virtual void update() {
        world->tick();
    };

    virtual bool isFinished() { return true; }
    virtual bool isWin() {return false;}
    virtual void handleEvents(SDL_Event& event) {
        world->handleEvent(event);
    };

    virtual ~Scene() {
        // std::cout << "Scene deleted" << std::endl;
        delete world;
    };
};