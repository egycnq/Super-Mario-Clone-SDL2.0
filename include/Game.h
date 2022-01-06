#pragma once

#include <iostream>
#include "SDL.h"
#include "SDL_image.h"
#include "ECS.h"
#include "levels/levelOne.h"
#include "levels/levelOneIntro.h"
#include "levels/start.h"
#include "levels/levelTwo.h"
#include "levels/levelTwoIntro.h"
#include "Constants.h"

class Game {

public:

    void init(const char* title, int width, int height, bool fullscreen);

    void handleEvents();

    void update();

    void clean();

    [[nodiscard]] bool running() const;

private:
    bool isRunning;
    SDL_Window* window;
    SDL_Event event;
    Scene* currentScene;
};
