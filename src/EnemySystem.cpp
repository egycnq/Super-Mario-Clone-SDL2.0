#include "EnemySystem.h"

void EnemySystem::AddedToWorld(World* world) {
}

void stepOnTurtle(Entity* enemy) {
    enemy->get<EnemyComponent>()->type = Enemy::Type::TURTLE_SHELL;
    enemy->remove<AnimationComponent>();
    enemy->remove<WalkComponent>();
    enemy->get<TextureComponent>()->id = TextureId::TURTLE_SHELL_1;
    enemy->get<TextureComponent>()->offSetY = 0;
    enemy->get<TextureComponent>()->h = TILE_SIZE;
    enemy->assign<CallbackComponent>([=]() {
        enemy->assign<AnimationComponent>(std::vector<TextureId>{
                                                  TextureId::TURTLE_SHELL_1,
                                                  TextureId::TURTLE_SHELL_2,
                                          },
                                          10,
                                          false);

        enemy->assign<CallbackComponent>([=]() {
            enemy->get<TextureComponent>()->id = TextureId::TURTLE_1;
            enemy->get<TextureComponent>()->offSetY = -TILE_SIZE;
            enemy->get<TextureComponent>()->h = TILE_SIZE * 2;
            enemy->get<TextureComponent>()->flipH = false;
            enemy->get<EnemyComponent>()->type = Enemy::Type::TURTLE;
            enemy->get<KineticComponent>()->accX = 0.0f;
            enemy->get<KineticComponent>()->speedX = 0.0f;
            enemy->assign<AnimationComponent>(std::vector<TextureId>{
                                                      TextureId::TURTLE_1,
                                                      TextureId::TURTLE_2,
                                              },
                                              10,
                                              false);
            enemy->assign<WalkComponent>(-.4f);
        }, 100);

    }, 200);
}

void flipEnemy(Entity* enemy) {
    if (enemy->has<EnemyComponent>()) {
        enemy->remove<EnemyComponent>();
        enemy->remove<SolidComponent>();
        enemy->remove<AnimationComponent>();
        enemy->assign<TileComponent>();
        enemy->get<TextureComponent>()->flipV = true;
        enemy->get<KineticComponent>()->speedY = -8;
    }
}

void turtleShellInteractions(World* world, Entity* shell) {
    if (shell->hasAny<LeftCollisionComponent, RightCollisionComponent>()) {
        world->find<EnemyComponent, KineticComponent>([&](Entity* other) {
            if (shell == other) return;
            auto transform = other->get<TransformComponent>();
            if (AABBCollision(shell->get<TransformComponent>(), other->get<TransformComponent>())) {
                world->create()->assign<FloatingPointsComponent>(Points::ONEHOUNDRED, transform->getCenterX(),
                                                                 transform->y);
                world->create()->assign<AddScoreComponent>(100);
                flipEnemy(other);
                shell->remove<LeftCollisionComponent>();
                shell->remove<RightCollisionComponent>();
            }
        });
    }

    if (shell->has<LeftCollisionComponent>()) {
        shell->get<KineticComponent>()->accX = 3.0f;
        shell->get<KineticComponent>()->speedX = 3.0f;
    }
    if (shell->has<RightCollisionComponent>()) {
        shell->get<KineticComponent>()->accX = -3.0f;
        shell->get<KineticComponent>()->speedX = -3.0f;
    }
}

void EnemySystem::tick(World* world) {
    world->find<EnemyComponent, KineticComponent>([&](Entity* shell) {
        if (shell->has<EnemyComponent>() && shell->get<EnemyComponent>()->type == Enemy::Type::TURTLE_SHELL) {
            turtleShellInteractions(world, shell);
        }
    });

    world->find<EnemyComponent, TransformComponent, CrushedComponent>([&](Entity* enemy) {
        auto enemyTransform = enemy->get<TransformComponent>();
        world->create()->assign<SoundComponent>(Sound::Id::STOMP);
        switch (enemy->get<EnemyComponent>()->type) {
            case Enemy::Type::GOOMBA: {
                enemy->clearComponents();
                enemy->assign<TileComponent>();
                enemy->assign<DestroyDelayedComponent>(50);
                enemy->assign<TextureComponent>(TextureId::GOOMBA_CRUSHED);
                enemy->assign<TransformComponent>(*enemyTransform);
                world->create()->assign<FloatingPointsComponent>(Points::ONEHOUNDRED, enemyTransform->getCenterX(),
                                                                 enemyTransform->y);
                world->create()->assign<AddScoreComponent>(100);
            }
                break;
            case Enemy::Type::TURTLE:
                stepOnTurtle(enemy);
                world->create()->assign<FloatingPointsComponent>(Points::TWOHOUNDRED, enemyTransform->getCenterX(),
                                                                 enemyTransform->y);
                world->create()->assign<AddScoreComponent>(200);
                break;
            case Enemy::Type::TURTLE_SHELL: {
                if (enemy->get<KineticComponent>()->accX == 0) {
                    auto player = world->findFirst<PlayerComponent>()->get<TransformComponent>()->getCenterX();
                    auto turtle = enemy->get<TransformComponent>()->getCenterX();
                    auto direction = turtle > player ? 3.0f : -3.0f;
                    enemy->get<KineticComponent>()->accX = direction;
                } else {
                    enemy->get<KineticComponent>()->accX = 0.0f;
                    enemy->get<KineticComponent>()->speedX = 0.0f;
                }
            }
        }
    });

    //WALKABLE (bounce walls)
    world->find<WalkComponent, LeftCollisionComponent>([&](Entity* entity) {
        auto walkComponent = entity->get<WalkComponent>();
        walkComponent->speed = std::abs(walkComponent->speed);
        if (entity->has<TextureComponent>() && entity->has<EnemyComponent>()) {
            entity->get<TextureComponent>()->flipH = true;
        }
    });

    // Flip Enemy by hitting block beneath
    world->find<BottomCollisionComponent, TransformComponent, KineticComponent, EnemyComponent>([&](Entity* entity) {
        auto transform = entity->get<TransformComponent>();
        auto tileMap = world->findFirst<TileMapComponent>()->get<TileMapComponent>();
        auto bottomTile = tileMap->get(
                (int) (transform->getCenterX() / TILE_SIZE),
                (int) (transform->getCenterY() / TILE_SIZE) + 1
        );
        if (bottomTile && bottomTile->has<BreakableComponent>()) {
            if (bottomTile->get<BreakableComponent>()->hit) {
                world->create()->assign<FloatingPointsComponent>(Points::ONEHOUNDRED, transform->getCenterX(),
                                                                 transform->y);
                world->create()->assign<AddScoreComponent>(100);
                flipEnemy(entity);
            }
        }
    });

    world->find<WalkComponent, RightCollisionComponent>([&](Entity* entity) {
        auto walkComponent = entity->get<WalkComponent>();
        walkComponent->speed = -std::abs(walkComponent->speed);
        if (entity->has<TextureComponent>() && entity->has<EnemyComponent>()) {
            entity->get<TextureComponent>()->flipH = false;
        }
    });
    world->find<WalkComponent, KineticComponent>([&](Entity* entity) {
        entity->get<KineticComponent>()->speedX = entity->get<WalkComponent>()->speed;
    });

    world->find<EnemyComponent>([&](Entity* entity) {
        entity->remove<BottomCollisionComponent>();
        entity->remove<TopCollisionComponent>();
        entity->remove<LeftCollisionComponent>();
        entity->remove<RightCollisionComponent>();
        entity->remove<CrushedComponent>();
    });
}

void EnemySystem::RemovedFromWorld(World* world) {
}
