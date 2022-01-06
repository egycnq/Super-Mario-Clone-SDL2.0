#include "levels/levels.h"

class GameScene2 : public Scene {

public:
    explicit GameScene2(SDL_Window* window);

    bool isFinished() override;
    bool isWin() override;

private:
    bool gameOver = false;
    bool win = false;

};