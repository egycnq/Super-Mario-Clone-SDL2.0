#include "levels/levels.h"

class start : public Scene {

public:
    explicit start(SDL_Window* window);

    bool isFinished() override;

    void update() override;

private:
    bool gameOver = false;
    SDL_Event event;

};