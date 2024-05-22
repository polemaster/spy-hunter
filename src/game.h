#pragma once
#include "window.h"
#include "font.h"
#include "paths.h"
#include "logic.h"
#include "constants.h"
#include "timer.h"

class Game {
public:
    Game();
    ~Game();

    void run();
private:
    Window window;
    Path paths;
    Font font;
    Logic logic;
    SDL_Renderer *renderer;
    SDL_Texture *images[ALL_IMGS]; // textures of cars, bushes and images
    int sizes[ALL_IMGS * 2]; // for storing sizes of textures
    int index; // for keeping track of current sizes[] element

    void safeLoad(SDL_Texture **texture, const char *path, bool &success);
    bool initGame();
    void renderGameInfo(const int time, const int score);
    void renderMenu();
    void printMenuText(const char *txt, int *x, int *y, const int height);
    void renderObjects(const int a, const int b);
    void renderRoad();
    void renderLegend();
};
