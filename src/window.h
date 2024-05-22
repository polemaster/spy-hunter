#pragma once
#include "constants.h"

class Window {
public:
    Window();

    void handleEvent(const SDL_Event *e, SDL_Renderer *r);
    bool isGood();
    bool isMinimized();
    SDL_Renderer *getNewRenderer();
    int getWidth();
    int getHeight();
    bool initialize();
    void freeMemory();
private:
    SDL_Window *_window;
    int _width, _height;
    bool _good, _minimized;
};
