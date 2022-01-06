#include "levels/levels.h"

class GameScene : public Scene {

public:
    explicit GameScene(SDL_Window* window);

    bool isFinished() override;
    bool isWin() override;

private:
    bool gameOver = false;
    bool win = false;

};