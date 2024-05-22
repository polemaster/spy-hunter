#include "font.h"
#include <stdio.h>

Font::Font() {
    _texture = NULL;
    _width = 0;
    _height = 0;
    _font_size = 1;
}

void Font::freeMemory() {
	if( _texture != NULL ) {
		SDL_DestroyTexture( _texture );
		_texture = NULL;
	}
}

int Font::getWidth() {
    return (int)(_width * _font_size);
}

int Font::getHeight() {
    return (int)(_height * _font_size);
}

void Font::setFontSize(double font_size) {
    _font_size = font_size;
}

bool Font::loadFont( const char *path, SDL_Renderer *r )
{
    SDL_Surface *font_surface = SDL_LoadBMP(path);
    if ( font_surface == NULL ) {
        printf("Couldn't creat font surface from BMP font.\n");
        return false;
    }
    // make background of the letters transparent
    SDL_SetColorKey( font_surface, SDL_TRUE, SDL_MapRGB( font_surface->format, 0, 0, 0 ) );

    _texture = SDL_CreateTextureFromSurface( r, font_surface );
    if( _texture == NULL ) {
        printf( "Couldn't create texture from surface: %s\n", SDL_GetError() );
        SDL_FreeSurface( font_surface );
        return false;
    }

    // get font sizes
    _width = font_surface->w;
    _height = font_surface->h;

    SDL_FreeSurface( font_surface );

    return true;
}

void Font::renderText( const char *txt, SDL_Renderer *renderer, const int x, const int y, SDL_Color color ) {
    // set the blend mode so that the font will be rendered with alpha transparency
    SDL_SetTextureBlendMode(_texture, SDL_BLENDMODE_BLEND);

    // set the font color
    SDL_SetTextureColorMod(_texture, color.r, color.g, color.b);

    // set current position on the screen
    int curr_x = x;
    int curr_y = y;

    // calculate width and height of the cell
    int cell_w = _width / 16;
    int cell_h = _height / 16;

    // calculate scaled width and height of the cell
    int s_cell_w = _font_size * cell_w;
    int s_cell_h = _font_size * cell_h;

    // loop through the characters in the string
    for (const char *c = txt; *c != '\0'; ++c) {
        // calculate the source rectangle for the current character
        int row = *c / 16;
        int col = *c % 16;
        SDL_Rect src_rect = { col * cell_w, row * cell_h, cell_w, cell_h };

        // calculate the destination rectangle for the current character
        SDL_Rect dest_rect = { curr_x, curr_y, s_cell_w, s_cell_h };

        // render the character
        SDL_RenderCopy(renderer, _texture, &src_rect, &dest_rect);

        // move to the next character on the screen
        curr_x += s_cell_w;
    }
}
