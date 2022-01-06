#include "levels/levelOne.h"

GameScene::GameScene(SDL_Window* window) {
    world->registerSystem<SoundSystem>();
    world->registerSystem<RenderSystem>(window, SNES_RESOLUTION_WIDTH, SNES_RESOLUTION_HEIGHT);
    world->registerSystem<PlayerSystem>([&](){ this->gameOver = true; });
    world->registerSystem<MapSystem>();
    world->registerSystem<CallbackSystem>();
    world->registerSystem<EnemySystem>();
    world->registerSystem<AnimationSystem>();
    world->registerSystem<ScoreSystem>();
    world->registerSystem<FlagSystem>(([&](){ this->win = true; }));
    world->registerSystem<TileSystem>();
    world->registerSystem<PhysicsSystem>();
}

bool GameScene::isFinished() {
    return gameOver;
}

bool GameScene::isWin() {
    return win;
}


