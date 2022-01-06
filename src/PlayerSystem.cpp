#include "PlayerSystem.h"

int dirX = 0;
bool jump = false;
bool duck = false;
bool sprint = false;
int left = 0;
int right = 0;
int lookingLeft = 0;

const int SMALL_MARIO_COLLIDER_HEIGHT = TILE_SIZE - 2;
const int SUPER_MARIO_COLLIDER_HEIGHT = (TILE_SIZE * 2) - 4;

constexpr int RUNNING_ANIMATION_SPEED = 4;

void shrink(World* world, Entity* player) {
    world->create()->assign<SoundComponent>(Sound::Id::SHRINK);
    player->assign<AnimationComponent>(
            std::vector<TextureId>{
                    TextureId::MARIO_STAND,
                    TextureId::MARIO_GROWING,
                    TextureId::MARIO_STAND,
                    TextureId::MARIO_GROWING,
                    TextureId::SUPER_MARIO_STAND,
                    TextureId::MARIO_STAND,
                    TextureId::SUPER_MARIO_STAND,
                    TextureId::MARIO_STAND
            }, 8, false, false, false);

    player->assign<FrozenComponent>();
    player->assign<BlinkingComponent>(10);
    player->remove<SuperMarioComponent>();

    player->assign<CallbackComponent>([player]() {
        player->remove<FrozenComponent>();
        player->assign<CallbackComponent>([player]() { player->remove<BlinkingComponent>(); }, 150);
    }, 64);
}

void createDebris(World* world, TransformComponent* brickTransform) {
    auto debris1 = world->create();
    debris1->assign<GravityComponent>();
    debris1->assign<KineticComponent>(-2.0f, -16.0f);
    debris1->assign<TileComponent>();
    debris1->assign<TextureComponent>(TextureId::BRICK_DEBRIS_1);
    debris1->assign<TransformComponent>(brickTransform->x, brickTransform->y, 8, 8);

    auto debris2 = world->create();
    debris2->assign<GravityComponent>();
    debris2->assign<KineticComponent>(2.0f, -16.0f);
    debris2->assign<TileComponent>();
    debris2->assign<TextureComponent>(TextureId::BRICK_DEBRIS_2);
    debris2->assign<TransformComponent>(brickTransform->x + 8, brickTransform->y, 8, 8);

    auto debris3 = world->create();
    debris3->assign<GravityComponent>();
    debris3->assign<KineticComponent>(-2.0f, -10.0f);
    debris3->assign<TileComponent>();
    debris3->assign<TextureComponent>(TextureId::BRICK_DEBRIS_3);
    debris3->assign<TransformComponent>(brickTransform->x, brickTransform->y + 8, 8, 8);

    auto debris4 = world->create();
    debris4->assign<GravityComponent>();
    debris4->assign<KineticComponent>(2.0f, -10.0f);
    debris4->assign<TileComponent>();
    debris4->assign<TextureComponent>(TextureId::BRICK_DEBRIS_4);
    debris4->assign<TransformComponent>(brickTransform->x + 8, brickTransform->y + 8, 8, 8);

    world->create()->assign<SoundComponent>(Sound::Id::BLOCK_BREAK);
}

void PlayerSystem::onGameOver(World* world, Entity* player) {
    if (player->has<SuperMarioComponent>()) {
        //player time ran out while being Super Mario
        player->remove<SuperMarioComponent>();
        auto texture = player->get<TextureComponent>();
        auto transform = player->get<TransformComponent>();
        transform->h = SMALL_MARIO_COLLIDER_HEIGHT;
        transform->y += SUPER_MARIO_COLLIDER_HEIGHT - SMALL_MARIO_COLLIDER_HEIGHT;
        texture->h = TILE_SIZE;
        texture->offSetY = -1;
    }
    player->assign<DeadComponent>();
    world->create()->assign<SoundComponent>(Sound::Id::DEATH);
    auto music = world->findFirst<MusicComponent>();
    if (music) world->destroy(music);

    auto kinetic = player->get<KineticComponent>();
    player->get<TextureComponent>()->id = TextureId::MARIO_DEAD;
    player->assign<DeadComponent>();

    player->remove<AnimationComponent>();
    kinetic->speedY = 0.0f;
    kinetic->speedX = 0.0f;
    kinetic->accY = 0.0f;
    kinetic->accX = 0.0f;

    world->findAny<WalkComponent, KineticComponent>([=](Entity* walker) {
        if (walker != player) {
            walker->remove<WalkComponent>();
            walker->remove<KineticComponent>();
            walker->remove<AnimationComponent>();
            walker->remove<CallbackComponent>();
        }
    });

    player->assign<CallbackComponent>([=]() {
        player->remove<SolidComponent>();
        kinetic->speedY = -10.0f;
        kinetic->speedX = 0.0f;
        kinetic->accY = 0.0f;
        kinetic->accX = 0.0f;

        player->assign<CallbackComponent>([&]() { gameOverCallback(); }, 200);
    }, 50);
}

void PlayerSystem::setAnimation(ANIMATION_STATE state) {
    if (player->has<BlinkingComponent>()) {
        auto blink = player->get<BlinkingComponent>();
        blink->current++;
        if ((blink->current / blink->speed) % 2) {
            player->remove<AnimationComponent>();
            currentState = BLINKING;
            player->get<TextureComponent>()->id = TextureId::EMPTY;
            return;
        }
    }
    if (currentState == state) return;
    player->remove<AnimationComponent>();

    switch (state) {
        case RUNNING: {
            if (player->has<SuperMarioComponent>()) {
                player->assign<AnimationComponent>(
                        std::vector<TextureId>{
                                TextureId::SUPER_MARIO_RUN_1,
                                TextureId::SUPER_MARIO_RUN_2,
                                TextureId::SUPER_MARIO_RUN_3},
                        RUNNING_ANIMATION_SPEED
                );
            } else {
                player->assign<AnimationComponent>(
                        std::vector<TextureId>{
                                TextureId::MARIO_RUN_1,
                                TextureId::MARIO_RUN_2,
                                TextureId::MARIO_RUN_3},
                        RUNNING_ANIMATION_SPEED
                );
            }

            break;
        }
        case STANDING:
            player->assign<TextureComponent>(
                    player->has<SuperMarioComponent>() ?
                    TextureId::SUPER_MARIO_STAND :
                    TextureId::MARIO_STAND);
            break;
        case JUMPING:
            player->assign<TextureComponent>(
                    player->has<SuperMarioComponent>() ?
                    TextureId::SUPER_MARIO_JUMP :
                    TextureId::MARIO_JUMP);
            break;
        case DRIFTING:
            player->assign<TextureComponent>(
                    player->has<SuperMarioComponent>() ?
                    TextureId::SUPER_MARIO_DRIFT :
                    TextureId::MARIO_DRIFT);
            break;
        case DUCKING:
            if (player->has<SuperMarioComponent>()) {
                player->assign<TextureComponent>(TextureId::SUPER_MARIO_DUCK);
            }
            break;
        case BLINKING:
            break;
    }

    if (player->has<SuperMarioComponent>()) {
        player->get<TextureComponent>()->h = TILE_SIZE * 2;
        player->get<TextureComponent>()->offSetY = -3;
    } else {
        player->get<TextureComponent>()->h = TILE_SIZE;
        player->get<TextureComponent>()->offSetY = -1;
    }
    player->get<TextureComponent>()->w = TILE_SIZE;
    player->get<TextureComponent>()->offSetX = -2;
    currentState = state;
}

void PlayerSystem::tick(World* world) {
    if (world->findFirst<GameOverComponent>()) {
        world->findFirst<GameOverComponent>()->remove<GameOverComponent>();
        onGameOver(world, player);
    }
    if (player->has<DeadComponent>()) return;

    auto transform = player->get<TransformComponent>();
    auto texture = player->get<TextureComponent>();
    if(transform->y >=220.0f) onGameOver(world,std::move(player));
    if (player->has<FrozenComponent>()) {
        if (texture) {
            if ((texture->id == TextureId::SUPER_MARIO_STAND ||
                 player->get<TextureComponent>()->id == TextureId::MARIO_GROWING) &&
                transform->h != SUPER_MARIO_COLLIDER_HEIGHT) {
                texture->h = TILE_SIZE * 2;
                transform->h = SUPER_MARIO_COLLIDER_HEIGHT;
                transform->y -= SUPER_MARIO_COLLIDER_HEIGHT - SMALL_MARIO_COLLIDER_HEIGHT;
                texture->offSetY = -3;
            }
            if (texture->id == TextureId::MARIO_STAND
                && transform->h != SMALL_MARIO_COLLIDER_HEIGHT) {
                texture->h = TILE_SIZE;
                texture->offSetY = -1;
                transform->h = SMALL_MARIO_COLLIDER_HEIGHT;
                transform->y += SUPER_MARIO_COLLIDER_HEIGHT - SMALL_MARIO_COLLIDER_HEIGHT;
            }
        }
        return;
    }

    auto kinetic = player->get<KineticComponent>();

    // Avoid walking back
    if (transform->left() < camera->left()) {
        transform->setLeft(camera->left());
        kinetic->accX = 0;
        kinetic->accX = 0;
        kinetic->speedX = 0;
    }

    if (duck && player->has<SuperMarioComponent>()) {
        setAnimation(DUCKING);
        kinetic->accX = 0;
    } else {
        if (player->has<BottomCollisionComponent>()) {
            kinetic->accX = (float) dirX * MARIO_ACCELERATION_X * 1.7f;
            if (sprint) kinetic->accX *= 1.5;
            if (jump) {
                player->get<KineticComponent>()->accY = -MARIO_JUMP_ACCELERATION;
                world->create()->assign<SoundComponent>(Sound::Id::JUMP);
            }
            if ((bool) std::abs(kinetic->speedX) || (bool) std::abs(kinetic->accX)) {
                if ((kinetic->speedX > 0 && kinetic->accX < 0) ||
                    (kinetic->speedX < 0 && kinetic->accX > 0)) {
                    setAnimation(DRIFTING);
                } else {
                    setAnimation(RUNNING);
                }
            } else {
                setAnimation(STANDING);
            }

        } else {
            kinetic->accX = (float) (dirX) * (MARIO_ACCELERATION_X);
            if (sprint) kinetic->accX *= 1.5;
            setAnimation(JUMPING);
        }
    }

    auto bounce = false;
    for (auto enemy : world->find<EnemyComponent, TransformComponent>()) {
        if (!AABBCollision(enemy->get<TransformComponent>(), transform)) continue;
        if (kinetic->speedY > 0) {
            enemy->assign<CrushedComponent>();
            transform->setBottom(enemy->get<TransformComponent>()->top());
            bounce = true;
        } else {
            if (enemy->get<EnemyComponent>()->type == Enemy::TURTLE_SHELL) continue; // ?
            if (!player->has<BlinkingComponent>()) {
                if (player->has<SuperMarioComponent>()) {
                    currentState = ANIMATION_STATE::STANDING;
                    shrink(world, player);
                } else {
                    onGameOver(world, player);
                }
                return;
            }
        }
    }

    if (bounce) {
        kinetic->accY = -0.2f;
        kinetic->speedY = -MARIO_BOUNCE;
    }

    // Break bricks
    if (player->has<SuperMarioComponent>()) {
        for (auto breakable : world->find<BreakableComponent, TransformComponent, BottomCollisionComponent>()) {
            if (!breakable->has<QuestionBlockComponent>() &&
                AABBCollision(breakable->get<TransformComponent>(), transform)) {
                createDebris(world, breakable->get<TransformComponent>());
                breakable->remove<SolidComponent>();
                breakable->remove<TextureComponent>();
                breakable->assign<BreakableComponent>();
                breakable->get<BreakableComponent>()->hit = true;
            }
        }
    }

    // Eat mushrooms
    for (auto collectible : world->find<CollectibleComponent, TransformComponent>()) {
        if (AABBCollision(collectible->get<TransformComponent>(), player->get<TransformComponent>())) {
            currentState = ANIMATION_STATE::STANDING;
            eatMushroom(world, collectible->get<TextureComponent>()->id == TextureId::ONE_UP);
            world->destroy(collectible);
        }
    }

    if (left || right) lookingLeft = left;
    player->get<TextureComponent>()->flipH = lookingLeft;
    jump = false;
    player->remove<BottomCollisionComponent>();
    player->remove<LeftCollisionComponent>();
    player->remove<RightCollisionComponent>();
    player->remove<TopCollisionComponent>();
}

void PlayerSystem::eatMushroom(World* world, bool oneup) {
    if (oneup) {
        world->create()->assign<SoundComponent>(Sound::Id::ONE_UP);
        world->create()->assign<FloatingPointsComponent>(Points::ONEUP,
                                                         player->get<TransformComponent>()->getCenterX(),
                                                         player->get<TransformComponent>()->y
        );
        return;
    } else {
        world->create()->assign<SoundComponent>(Sound::Id::MUSHROOM_EAT);
        world->create()->assign<FloatingPointsComponent>(Points::ONETHOUSAND,
                                                         player->get<TransformComponent>()->getCenterX(),
                                                         player->get<TransformComponent>()->y
        );
        world->create()->assign<AddScoreComponent>(1000);
    }

    if (player->has<SuperMarioComponent>()) return;
    player->assign<SuperMarioComponent>();
    player->assign<AnimationComponent>(
            std::vector<TextureId>{
                    TextureId::MARIO_STAND,
                    TextureId::MARIO_GROWING,
                    TextureId::MARIO_STAND,
                    TextureId::MARIO_GROWING,
                    TextureId::SUPER_MARIO_STAND,
                    TextureId::MARIO_STAND,
                    TextureId::MARIO_GROWING,
                    TextureId::SUPER_MARIO_STAND,
                    TextureId::MARIO_STAND,
                    TextureId::SUPER_MARIO_STAND
            }, 6, false, false, false);

    player->assign<FrozenComponent>();
    player->assign<CallbackComponent>([&]() {
        player->remove<FrozenComponent>();
    }, 60);
}

void PlayerSystem::handleEvent(SDL_Event& event) {
    if (event.type != SDL_KEYDOWN && event.type != SDL_KEYUP) return;

    switch (event.type) {
        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_LEFT:
                    left = true;
                    break;
                case SDL_SCANCODE_LCTRL:
                    sprint = true;
                    break;
                case SDL_SCANCODE_RIGHT:
                    right = true;
                    break;
                case SDL_SCANCODE_SPACE:
                    jump = true;
                    break;
                case SDL_SCANCODE_DOWN:
                    duck = true;
                    break;
                default:
                    break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_LEFT:
                    left = false;
                    break;
                case SDL_SCANCODE_LCTRL:
                    sprint = false;
                    break;
                case SDL_SCANCODE_RIGHT:
                    right = false;
                    break;
                case SDL_SCANCODE_DOWN:
                    duck = false;
                    break;
                default:
                    break;
            }
    }

    dirX = right - left;
}

void PlayerSystem::RemovedFromWorld(World* world) {
    camera = nullptr;
    world->destroy(player);
}

void PlayerSystem::AddedToWorld(World* world) {
    camera = world->findFirst<CameraComponent>()->get<CameraComponent>();
    player = world->create();
    player->assign<PlayerComponent>();

    player->assign<GravityComponent>();
    player->assign<SolidComponent>();
    player->assign<KineticComponent>();
    player->assign<TransformComponent>(40, 140, TILE_SIZE - 4, SMALL_MARIO_COLLIDER_HEIGHT);
}
