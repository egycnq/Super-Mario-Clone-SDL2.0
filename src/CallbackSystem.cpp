#include "CallbackSystem.h"

void CallbackSystem::AddedToWorld(World* world) {
}

void CallbackSystem::tick(World* world) {
    world->find<CallbackComponent>([](Entity* entity){
        auto callback = entity->get<CallbackComponent>();
        if (callback->time > 0) {
            callback->time--;
            if (callback->time == 0) callback->callback();
        } else {
            entity->remove<CallbackComponent>();
        }
    });
}

void CallbackSystem::handleEvent(SDL_Event& event) {
    System::handleEvent(event);
}

void CallbackSystem::RemovedFromWorld(World* world) {
}
