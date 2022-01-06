#include "levels/levels.h"

class IntroScene2 : public Scene {

public:
    explicit IntroScene2(SDL_Window* window);

    bool isFinished() override;

    void update() override;

private:
    int timer = 0;
};