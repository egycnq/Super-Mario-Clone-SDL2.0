#include "AnimationSystem.h"

void AnimationSystem::AddedToWorld(World* world) {
}

void AnimationSystem::tick(World* world) {
    world->find<AnimationComponent, TextureComponent>([&](Entity* entity) {
        auto animation = entity->get<AnimationComponent>();
        auto texture = entity->get<TextureComponent>();
        animation->counter++;
        if (animation->counter >= animation->duration) {
            if (animation->flipH) texture->flipH = !texture->flipH;
            if (animation->flipV) texture->flipV = !texture->flipV;
            animation->counter = 0;
            animation->currentTexture = (animation->currentTexture + 1) % animation->textures.size();
            texture->id = animation->textures[animation->currentTexture];
            if (!animation->loop && animation->currentTexture == animation->textures.size() - 1) {
                entity->remove<AnimationComponent>();
            }
        }
    });
}

void AnimationSystem::handleEvent(SDL_Event& event) {
    System::handleEvent(event);
}

void AnimationSystem::RemovedFromWorld(World* world) {
}
