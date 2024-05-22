#pragma once
#include "constants.h"

class Font {
public:
    Font();

    bool loadFont( const char *path, SDL_Renderer *r );
    void renderText( const char *txt, SDL_Renderer *r, const int x, const int y, SDL_Color color );
    void freeMemory();

    int getWidth();
    int getHeight();
    void setFontSize( double font_size );
private:
    SDL_Texture *_texture;
    int _width, _height;
    double _font_size;
};
