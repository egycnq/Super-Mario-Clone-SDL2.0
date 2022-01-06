#include "levels/start.h"
#include "SDL_events.h"

start::start(SDL_Window* window) {
    world->registerSystem<SoundSystem>();
    world->registerSystem<RenderSystem>(window, SNES_RESOLUTION_WIDTH, SNES_RESOLUTION_HEIGHT);
    world->registerSystem<MapSystem>("assets/intro");
    world->registerSystem<CallbackSystem>();
    world->registerSystem<EnemySystem>();
    world->registerSystem<AnimationSystem>();
    world->registerSystem<TileSystem>();
    world->registerSystem<PhysicsSystem>();
    auto text = world->create();
    text->assign<TextComponent>("Press Any Key to Start");
    auto width = 72;
    text->assign<TransformComponent>(SNES_RESOLUTION_WIDTH / 2 - width / 2,
                                          SNES_RESOLUTION_HEIGHT / 2 +20, width,
                                          8);
}


void start::update() {
    Scene::update();
    SDL_PollEvent(&event);
    if(event.type ==SDL_KEYDOWN) gameOver = true;
}
bool start::isFinished() {
    return gameOver;
}


